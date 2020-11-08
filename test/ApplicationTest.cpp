#include <gtest/gtest.h>

extern "C" {
#include "application.h"
#include <linked_list.h>
#include "command.h"
}

char *now() {
    std::time_t t = std::time(0);   // get time now
    std::tm *now = std::localtime(&t);

    char buf[256] = {0};
    sprintf(buf, "%d-%d-%d", now->tm_year, now->tm_mon + 1, now->tm_mday);
    return strdup(buf);
}

TEST(ApplicationTest, employers_should_be_able_to_publish_a_job) {
    char *employerName = "";
    char *jobName = "高级前端开发";
    Application *pApplication = newApplication();
    Job *pJob = newJob(jobName);
    pJob->jobType = JReq;
    execute(pApplication, PUBLISH, newEmployer(employerName), pJob, NULL, NULL, NULL);
    LinkedList *jobs = getJobs(pApplication, employerName, PUBLISHED);
    Job *job = static_cast<Job *>(getItem(jobs, 0));

    ASSERT_STREQ("高级前端开发", job->name);
    ASSERT_EQ(JReq, job->jobType);
}

TEST(ApplicationTest, employers_should_only_be_able_to_see_jobs_published_by_them) {
    char *employerAlibaba = "Alibaba";
    char *employerTencent = "Tencent";
    char *seniorJavaDevJob = "高级Java开发";
    char *juniorJavaDevJob = "Java开发";

    Application *pApplication = newApplication();
    Job *pJob = newJob(seniorJavaDevJob);
    pJob->jobType = JReq;
    execute(pApplication, PUBLISH, newEmployer(employerAlibaba), pJob, NULL, NULL, NULL);

    Job *pJobJuniorJavaDev = newJob(juniorJavaDevJob);
    execute(pApplication, PUBLISH, newEmployer(employerTencent), pJobJuniorJavaDev, NULL, NULL, NULL);
    LinkedList *jobs = getJobs(pApplication, employerAlibaba, PUBLISHED);
    ASSERT_EQ(1, len(jobs));
    Job *job = (Job *) getItem(jobs, 0);
    ASSERT_STREQ("高级Java开发", job->name);
}

TEST(ApplicationTest, employers_should_be_able_to_publish_ATS_jobs) {
    char *employerAlibaba = "Alibaba";
    char *seniorJavaDevJob = "高级Java开发";
    Application *pApplication = newApplication();

    Job *pJob = newJob(seniorJavaDevJob);
    pJob->jobType = ATS;
    execute(pApplication, PUBLISH, newEmployer(employerAlibaba), pJob, NULL, NULL, NULL);
    LinkedList *jobs = getJobs(pApplication, employerAlibaba, PUBLISHED);
    Job *job = (Job *) getItem(jobs, 0);
    ASSERT_STREQ("高级Java开发", job->name);
    ASSERT_EQ(ATS, job->jobType);
}

TEST(ApplicationTest, jobseekers_should_be_able_to_save_jobs_published_by_employers_for_later_review) {
    char *employerAlibaba = "Alibaba";
    char *jobSeekerName = "Jacky";
    char *jobName = "高级Java开发";
    Application *pApplication = newApplication();

    Job *pJob = newJob(jobName);
    pJob->jobType = JReq;
    execute(pApplication, PUBLISH, newEmployer(employerAlibaba), pJob, NULL, NULL, NULL);
    execute(pApplication, SAVE, NULL, pJob, newJobSeeker(jobSeekerName), NULL, NULL);

    LinkedList *jobs = getJobs(pApplication, jobSeekerName, PUBLISHED);
    Job *job = (Job *) getItem(jobs, 0);

    ASSERT_STREQ("高级Java开发", job->name);
    ASSERT_EQ(JReq, job->jobType);
}

TEST(Application, jobseekers_should_be_able_to_apply_for_an_ATS_job_some_employer_published_without_a_resume) {
    char *employerAlibaba = "Alibaba";
    char *jobSeekerName = "Jacky";
    char *seniorJavaDevJob = "高级Java开发";
    char *juniorJavaDevJob = "Java开发";

    Application *pApplication = newApplication();

    Job *pJobSeniorJavaDev = newJob(seniorJavaDevJob);
    pJobSeniorJavaDev->jobType = ATS;
    execute(pApplication, PUBLISH, newEmployer(employerAlibaba), pJobSeniorJavaDev, NULL, NULL, NULL);

    Job *pJobJuniorJavaDev = newJob(juniorJavaDevJob);
    pJobJuniorJavaDev->jobType = ATS;
    execute(pApplication, PUBLISH, newEmployer(employerAlibaba), pJobJuniorJavaDev, NULL, NULL, NULL);

    execute(pApplication, APPLY, newEmployer(employerAlibaba), pJobJuniorJavaDev, newJobSeeker(jobSeekerName), NULL,
            "2020-01-01");
    execute(pApplication, APPLY, newEmployer(employerAlibaba), pJobSeniorJavaDev, newJobSeeker(jobSeekerName), NULL,
            "2020-01-01");
    LinkedList *jobs = getJobs(pApplication, jobSeekerName, APPLIED);

    JobApplication *pJobApplication1 = (JobApplication *) getItem(jobs, 0);

    ASSERT_STREQ("Java开发", pJobApplication1->job->name);
    ASSERT_EQ(ATS, pJobApplication1->job->jobType);
    ASSERT_STREQ("2020-01-01", pJobApplication1->applicationTime);
    ASSERT_STREQ("Alibaba", pJobApplication1->employer->name);

    JobApplication *pJobApplication2 = (JobApplication *) getItem(jobs, 1);
    ASSERT_STREQ("高级Java开发", pJobApplication2->job->name);
    ASSERT_EQ(ATS, pJobApplication2->job->jobType);
    ASSERT_STREQ("2020-01-01", pJobApplication2->applicationTime);
    ASSERT_STREQ("Alibaba", pJobApplication2->employer->name);
}

TEST(ApplicationTest, jobseekers_should_not_be_able_to_apply_for_an_JReq_job_some_employer_published_without_a_resume) {
    char *employerAlibaba = "Alibaba";
    char *jobSeekerName = "Jacky";
    char *seniorJavaDevJob = "高级Java开发";

    Application *pApplication = newApplication();

    Job *pJob = newJob(seniorJavaDevJob);
    pJob->jobType = JReq;

    execute(pApplication, PUBLISH, newEmployer(employerAlibaba), pJob, NULL, NULL, NULL);
    int result = execute(pApplication, APPLY, newEmployer(employerAlibaba), pJob,
                         newJobSeeker(jobSeekerName), NULL, now());
    ASSERT_EQ(401, result);
}

TEST(ApplicationTest,
     jobseekers_should_not_be_able_to_apply_for_an_JReq_job_some_employer_published_with_someone_else_s_resume) {
    char *employerAlibaba = "Alibaba";
    char *jobSeekerName = "Jacky";
    char *seniorJavaDevJob = "高级Java开发";
    char *resumeApplicantName = "Jacky Chen";

    Application *pApplication = newApplication();
    Job *pJob = newJob(seniorJavaDevJob);
    pJob->jobType = JReq;

    execute(pApplication, PUBLISH, newEmployer(employerAlibaba), pJob, NULL, NULL, NULL);
    int result = execute(pApplication, APPLY, newEmployer(employerAlibaba), pJob,
                         newJobSeeker(jobSeekerName),
                         newResume(resumeApplicantName), NULL);
    ASSERT_EQ(402, result);
}

TEST(ApplicationTest, employers_should_be_able_to_find_applicants_of_a_job) {
    char *employerAlibaba = "Alibaba";
    char *jobSeekerJacky = "Jacky";
    char *jobSeekerLam = "Lam";
    char *seniorJavaDevJob = "高级Java开发";

    Application *pApplication = newApplication();
    execute(pApplication, PUBLISH, newEmployer(employerAlibaba), newJobWithType(seniorJavaDevJob, ATS), NULL, NULL, NULL);
    execute(pApplication, APPLY, newEmployer(employerAlibaba), newJobWithType(seniorJavaDevJob, ATS), newJobSeeker(jobSeekerJacky),
            NULL, now());
    execute(pApplication, APPLY, newEmployer(employerAlibaba), newJobWithType(seniorJavaDevJob, ATS), newJobSeeker(jobSeekerLam), NULL,
            now());

    LinkedList *applicants = findApplicants(pApplication, newJob(seniorJavaDevJob));

    ASSERT_STREQ("Jacky", (char *) getItem(applicants, 0));
    ASSERT_STREQ("Lam", (char *) getItem(applicants, 1));
}

TEST(ApplicationTest, employers_should_be_able_to_find_applicants_to_a_job_by_application_date) {
    char *employerAlibaba = "Alibaba";
    char *jobSeekerJacky = "Jacky";
    char *jobSeekerHo = "Ho";
    char *seniorJavaDevJob = "高级Java开发";
    Application *pApplication = newApplication();
    execute(pApplication, PUBLISH, newEmployer(employerAlibaba), newJobWithType(seniorJavaDevJob, ATS), NULL, NULL, NULL);
    execute(pApplication, APPLY, newEmployer(employerAlibaba), newJobWithType(seniorJavaDevJob, ATS), newJobSeeker(jobSeekerJacky),
            NULL, "1997-07-01");
    execute(pApplication, APPLY, newEmployer(employerAlibaba), newJobWithType(seniorJavaDevJob, ATS), newJobSeeker(jobSeekerHo), NULL,
            "1999-12-20");

    LinkedList *applicants = findApplicantsFrom(pApplication, NULL, "1999-12-20");

    ASSERT_STREQ("Ho", (char *) getItem(applicants, 0));
}

TEST(ApplicationTest,
     employers_should_be_able_to_find_applicants_to_a_job_by_period_when_period_end_is_given_while_period_start_is_not) {
    char *employerAlibaba = "Alibaba";
    char *jobSeekerJacky = "Jacky";
    char *jobSeekerHo = "Ho";
    char *seniorJavaDevJob = "高级Java开发";
    Application *pApplication = newApplication();
    execute(pApplication, PUBLISH, newEmployer(employerAlibaba), newJobWithType(seniorJavaDevJob, ATS), NULL, NULL, NULL);
    execute(pApplication, APPLY, newEmployer(employerAlibaba), newJobWithType(seniorJavaDevJob, ATS), newJobSeeker(jobSeekerJacky),
            NULL, "1997-07-01");
    execute(pApplication, APPLY, newEmployer(employerAlibaba), newJobWithType(seniorJavaDevJob, ATS), newJobSeeker(jobSeekerHo), NULL,
            "1999-12-20");

    LinkedList *applicants = findApplicantsIn(pApplication->appliedJobs, NULL, NULL, "1999-12-20");

    ASSERT_STREQ("Jacky", (char *) getItem(applicants, 0));
}

TEST(ApplicationTest, employers_should_be_able_to_find_applicants_to_a_job_by_period) {
    char *employerAlibaba = "Alibaba";
    char *jobSeekerJacky = "Jacky";
    char *jobSeekerHo = "Ho";
    char *seniorJavaDevJob = "高级Java开发";
    Application *pApplication = newApplication();
    execute(pApplication, PUBLISH, newEmployer(employerAlibaba), newJobWithType(seniorJavaDevJob, ATS), NULL, NULL, NULL);
    execute(pApplication, APPLY, newEmployer(employerAlibaba), newJobWithType(seniorJavaDevJob, ATS), newJobSeeker(jobSeekerJacky),
            NULL, "1997-07-01");
    execute(pApplication, APPLY, newEmployer(employerAlibaba), newJobWithType(seniorJavaDevJob, ATS), newJobSeeker(jobSeekerHo), NULL,
            "1999-12-20");

    LinkedList *applicants = findApplicantsIn(pApplication->appliedJobs, NULL, "1997-07-01", "1999-12-20");

    ASSERT_STREQ("Jacky", (char *) getItem(applicants, 0));
    ASSERT_STREQ("Ho", (char *) getItem(applicants, 1));
}


TEST(ApplicationTest,
     employers_should_be_able_to_find_applicants_to_a_job_by_job_name_and_period_when_period_start_is_given_while_period_end_is_not) {
    char *employerAlibaba = "Alibaba";
    char *jobSeekerJacky = "Jacky";
    char *resumeApplicantName = "Jacky";
    char *jobSeekerHo = "Ho";
    char *seniorJavaDevJob = "高级Java开发";
    char *juniorJavaDevJob = "Java开发";

    Application *pApplication = newApplication();
    execute(pApplication, PUBLISH, newEmployer(employerAlibaba), newJobWithType(juniorJavaDevJob, ATS), NULL, NULL, NULL);
    execute(pApplication, PUBLISH, newEmployer(employerAlibaba), newJobWithType(seniorJavaDevJob, JReq), NULL, NULL, NULL);

    execute(pApplication, APPLY, newEmployer(employerAlibaba), newJobWithType(juniorJavaDevJob, ATS), newJobSeeker(jobSeekerJacky),
            NULL, "1997-07-01");
    execute(pApplication, APPLY, newEmployer(employerAlibaba), newJobWithType(seniorJavaDevJob, JReq), newJobSeeker(jobSeekerJacky),
            newResume(resumeApplicantName),
            "1999-12-20");
    execute(pApplication, APPLY, newEmployer(employerAlibaba), newJobWithType(juniorJavaDevJob, ATS), newJobSeeker(jobSeekerHo), NULL,
            "1999-12-20");

    LinkedList *applicants = findApplicantsFrom(pApplication, newJob(seniorJavaDevJob), "1999-12-20");

    ASSERT_STREQ("Jacky", (char *) getItem(applicants, 0));
}

TEST(ApplicationTest,
     employers_should_be_able_to_find_applicants_to_a_job_by_job_name_and_period_when_period_end_is_given_while_period_start_is_not) {
    char *employerAlibaba = "Alibaba";
    char *jobSeekerJacky = "Jacky";
    char *jobSeekerHo = "Ho";
    char *seniorJavaDevJob = "高级Java开发";
    char *juniorJavaDevJob = "Java开发";

    Application *pApplication = newApplication();
    execute(pApplication, PUBLISH, newEmployer(employerAlibaba), newJobWithType(seniorJavaDevJob, ATS), NULL, NULL, NULL);
    execute(pApplication, PUBLISH, newEmployer(employerAlibaba), newJobWithType(juniorJavaDevJob, ATS), NULL, NULL, NULL);

    execute(pApplication, APPLY, newEmployer(employerAlibaba), newJobWithType(juniorJavaDevJob, ATS), newJobSeeker(jobSeekerJacky),
            NULL, "1997-07-01");
    execute(pApplication, APPLY, newEmployer(employerAlibaba), newJobWithType(seniorJavaDevJob, ATS), newJobSeeker(jobSeekerJacky),
            NULL, "1997-07-01");
    execute(pApplication, APPLY, newEmployer(employerAlibaba), newJobWithType(juniorJavaDevJob, ATS), newJobSeeker(jobSeekerHo), NULL,
            "1999-12-20");

    LinkedList *applicants = findApplicantsIn(pApplication->appliedJobs, newJob(seniorJavaDevJob), NULL, "1999-01-01");

    ASSERT_STREQ("Jacky", (char *) getItem(applicants, 0));
}

TEST(ApplicationTest,
     employers_should_be_able_to_find_applicants_to_a_job_by_job_name_and_period) {

    char *employerAlibaba = "Alibaba";
    char *jobSeekerWong = "Wong";
    char *jobSeekerJacky = "Jacky";
    char *jobSeekerHo = "Ho";
    char *jobSeekerLam = "Lam";
    char *seniorJavaDevJob = "高级Java开发";
    char *juniorJavaDevJob = "Java开发";

    Application *pApplication = newApplication();
    execute(pApplication, PUBLISH, newEmployer(employerAlibaba), newJobWithType(seniorJavaDevJob, ATS), NULL, NULL, NULL);
    execute(pApplication, PUBLISH, newEmployer(employerAlibaba), newJobWithType(juniorJavaDevJob, ATS), NULL, NULL, NULL);

    execute(pApplication, APPLY, newEmployer(employerAlibaba), newJobWithType(seniorJavaDevJob, ATS), newJobSeeker(jobSeekerWong), NULL,
            "1997-07-01");
    execute(pApplication, APPLY, newEmployer(employerAlibaba), newJobWithType(juniorJavaDevJob, ATS), newJobSeeker(jobSeekerJacky),
            NULL, "1996-12-31");
    execute(pApplication, APPLY, newEmployer(employerAlibaba), newJobWithType(juniorJavaDevJob, ATS), newJobSeeker(jobSeekerHo), NULL,
            "1998-01-01");
    execute(pApplication, APPLY, newEmployer(employerAlibaba), newJobWithType(juniorJavaDevJob, ATS), newJobSeeker(jobSeekerLam), NULL,
            "1999-12-20");

    LinkedList *applicants = findApplicantsIn(pApplication->appliedJobs, newJob(juniorJavaDevJob), "1997-01-01",
                                              "1999-01-01");

    ASSERT_STREQ("Ho", (char *) getItem(applicants, 0));
    ASSERT_EQ(1, len(applicants));
}

TEST(ApplicationTest,
     should_generator_csv_reports_of_all_jobseekers_on_a_given_date) {
    char *employerAlibaba = "Alibaba";
    char *jobSeekerJacky = "Jacky";
    char *jackyResume = "Jacky";
    char *jobSeekerHo = "Ho";
    char *jobSeekerLam = "Lam";
    char *lamResume = "Lam";
    char *seniorJavaDevJob = "Senior Java Developer";
    char *juniorJavaDevJob = "Java Developer";

    Application *pApplication = newApplication();
    execute(pApplication, PUBLISH, newEmployer(employerAlibaba), newJobWithType(juniorJavaDevJob, ATS), NULL, NULL, NULL);
    execute(pApplication, PUBLISH, newEmployer(employerAlibaba), newJobWithType(seniorJavaDevJob, JReq), NULL, NULL, NULL);

    execute(pApplication, APPLY, newEmployer(employerAlibaba), newJobWithType(juniorJavaDevJob, ATS), newJobSeeker(jobSeekerJacky),
            NULL, "1997-07-01");
    execute(pApplication, APPLY, newEmployer(employerAlibaba), newJobWithType(seniorJavaDevJob, JReq), newJobSeeker(jobSeekerJacky),
            newResume(jackyResume),
            "1999-12-20");
    execute(pApplication, APPLY, newEmployer(employerAlibaba), newJobWithType(juniorJavaDevJob, ATS), newJobSeeker(jobSeekerHo), NULL,
            "1999-12-20");
    execute(pApplication, APPLY, newEmployer(employerAlibaba), newJobWithType(juniorJavaDevJob, ATS), newJobSeeker(jobSeekerLam), NULL,
            "1999-12-20");
    execute(pApplication, APPLY, newEmployer(employerAlibaba), newJobWithType(seniorJavaDevJob, JReq), newJobSeeker(jobSeekerLam),
            newResume(lamResume), "1999-12-20");

    char *csv = exportTo(pApplication, CSV, "1999-12-20");
    ASSERT_STREQ(csv,
                 "Employer,Job,Job Type,Applicants,Date\n"
                 "Alibaba,Senior Java Developer,JReq,Jacky,1999-12-20\n"
                 "Alibaba,Java Developer,ATS,Ho,1999-12-20\n"
                 "Alibaba,Java Developer,ATS,Lam,1999-12-20\n"
                 "Alibaba,Senior Java Developer,JReq,Lam,1999-12-20\n"
    );
}

TEST(ApplicationTest,
     should_generator_html_reports_of_all_jobseekers_on_a_given_date) {
    char *employerAlibaba = "Alibaba";
    char *jobSeekerJacky = "Jacky";
    char *jackyResume = "Jacky";
    char *jobSeekerHo = "Ho";
    char *jobSeekerLam = "Lam";
    char *lamResume = "Lam";
    char *seniorJavaDevJob = "Senior Java Developer";
    char *juniorJavaDevJob = "Java Developer";

    Application *pApplication = newApplication();
    execute(pApplication, PUBLISH, newEmployer(employerAlibaba), newJobWithType(juniorJavaDevJob, ATS), NULL, NULL, NULL);
    execute(pApplication, PUBLISH, newEmployer(employerAlibaba), newJobWithType(seniorJavaDevJob, JReq), NULL, NULL, NULL);

    execute(pApplication, APPLY, newEmployer(employerAlibaba), newJobWithType(juniorJavaDevJob, ATS), newJobSeeker(jobSeekerJacky),
            NULL, "1997-07-01");
    execute(pApplication, APPLY, newEmployer(employerAlibaba), newJobWithType(seniorJavaDevJob, JReq), newJobSeeker(jobSeekerJacky),
            newResume(jackyResume),
            "1999-12-20");
    execute(pApplication, APPLY, newEmployer(employerAlibaba), newJobWithType(juniorJavaDevJob, ATS), newJobSeeker(jobSeekerHo), NULL,
            "1999-12-20");
    execute(pApplication, APPLY, newEmployer(employerAlibaba), newJobWithType(juniorJavaDevJob, ATS), newJobSeeker(jobSeekerLam), NULL,
            "1999-12-20");
    execute(pApplication, APPLY, newEmployer(employerAlibaba), newJobWithType(seniorJavaDevJob, JReq), newJobSeeker(jobSeekerLam),
            newResume(lamResume), "1999-12-20");

    char *html = exportTo(pApplication, HTML, "1999-12-20");
    ASSERT_STREQ(html,
                 "<!DOCTYPE html><body><table><thead><tr><th>Employer</th><th>Job</th><th>Job Type</th><th>Applicants</th><th>Date</th></tr></thead><tbody>"
                 "<tr><td>Alibaba</td><td>Senior Java Developer</td><td>JReq</td><td>Jacky</td><td>1999-12-20</td></tr>"
                 "<tr><td>Alibaba</td><td>Java Developer</td><td>ATS</td><td>Ho</td><td>1999-12-20</td></tr>"
                 "<tr><td>Alibaba</td><td>Java Developer</td><td>ATS</td><td>Lam</td><td>1999-12-20</td></tr>"
                 "<tr><td>Alibaba</td><td>Senior Java Developer</td><td>JReq</td><td>Lam</td><td>1999-12-20</td></tr>"
                 "</tbody></table></body></html>");
}

TEST(ApplicationTest, should_be_able_to_see_successful_application_of_a_job_for_an_employer) {
    char *employerAlibaba = "Alibaba";
    char *employerTencent = "Tencent";
    char *jobSeekerJacky = "Jacky";
    char *jobSeekerHo = "Ho";
    char *jobSeekerLam = "Lam";
    char *seniorJavaDevJob = "高级Java开发";
    char *juniorJavaDevJob = "Java开发";

    Application *pApplication = newApplication();
    execute(pApplication, PUBLISH, newEmployer(employerAlibaba), newJobWithType(seniorJavaDevJob, ATS), NULL, NULL, NULL);
    execute(pApplication, PUBLISH, newEmployer(employerAlibaba), newJobWithType(juniorJavaDevJob, ATS), NULL, NULL, NULL);
    execute(pApplication, PUBLISH, newEmployer(employerTencent), newJobWithType(juniorJavaDevJob, ATS), NULL, NULL, NULL);
    execute(pApplication, APPLY, newEmployer(employerAlibaba), newJobWithType(seniorJavaDevJob, ATS), newJobSeeker(jobSeekerJacky),
            NULL, now());
    execute(pApplication, APPLY, newEmployer(employerAlibaba), newJobWithType(seniorJavaDevJob, ATS), newJobSeeker(jobSeekerLam), NULL,
            now());
    execute(pApplication, APPLY, newEmployer(employerAlibaba), newJobWithType(juniorJavaDevJob, ATS), newJobSeeker(jobSeekerHo), NULL,
            now());
    execute(pApplication, APPLY, newEmployer(employerTencent), newJobWithType(juniorJavaDevJob, ATS), newJobSeeker(jobSeekerHo), NULL,
            now());

    ASSERT_EQ(2, getSuccessfulApplications(pApplication, newEmployer(employerAlibaba), newJob(seniorJavaDevJob)));
    ASSERT_EQ(1, getSuccessfulApplications(pApplication, newEmployer(employerAlibaba), newJob(juniorJavaDevJob)));
}

TEST(ApplicationTest, should_be_able_to_see_unsuccessful_applications_of_a_job_for_an_employer) {
    char *employerAlibaba = "Alibaba";
    char *jobSeekerJacky = "Jacky";
    char *jobSeekerLam = "Lam";
    char *seniorJavaDevJob = "高级Java开发";
    char *juniorJavaDevJob = "Java开发";

    Application *pApplication = newApplication();
    execute(pApplication, PUBLISH, newEmployer(employerAlibaba), newJobWithType(seniorJavaDevJob, JReq), NULL, NULL, NULL);
    execute(pApplication, PUBLISH, newEmployer(employerAlibaba), newJobWithType(juniorJavaDevJob, ATS), NULL, NULL, NULL);
    execute(pApplication, APPLY, newEmployer(employerAlibaba), newJobWithType(seniorJavaDevJob, JReq), newJobSeeker(jobSeekerJacky),
            NULL, now());
    execute(pApplication, APPLY, newEmployer(employerAlibaba), newJobWithType(juniorJavaDevJob, ATS), newJobSeeker(jobSeekerLam), NULL,
            now());
    ASSERT_EQ(1, getUnsuccessfulApplications(pApplication, newEmployer(employerAlibaba), newJob(seniorJavaDevJob)));
    ASSERT_EQ(0, getUnsuccessfulApplications(pApplication, newEmployer(employerAlibaba), newJob(juniorJavaDevJob)));
}