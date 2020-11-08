#include <gtest/gtest.h>

extern "C" {
#include "application.h"
#include <linked_list.h>
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
    execute(pApplication, "publish", employerName, jobName, "JReq", NULL, NULL, NULL);
    LinkedList *jobs = getJobs(pApplication, employerName, "published");
    LinkedList *job = static_cast<LinkedList *>(getItem(jobs, 0));

    ASSERT_STREQ("高级前端开发", (char *) getItem(job, 0));
    ASSERT_STREQ("JReq", (char *) getItem(job, 1));
}

TEST(ApplicationTest, employers_should_only_be_able_to_see_jobs_published_by_them) {
    char *employerAlibaba = "Alibaba";
    char *employerTencent = "Tencent";
    char *seniorJavaDevJob = "高级Java开发";
    char *juniorJavaDevJob = "Java开发";

    Application *pApplication = newApplication();
    execute(pApplication, "publish", employerAlibaba, seniorJavaDevJob, "JReq", NULL, NULL, NULL);
    execute(pApplication, "publish", employerTencent, juniorJavaDevJob, "JReq", NULL, NULL, NULL);
    LinkedList *jobs = getJobs(pApplication, employerAlibaba, "published");
    ASSERT_EQ(1, len(jobs));
    LinkedList *job = (LinkedList *) getItem(jobs, 0);
    ASSERT_STREQ("高级Java开发", (char *) getItem(job, 0));
}

TEST(ApplicationTest, employers_should_be_able_to_publish_ATS_jobs) {
    char *employerAlibaba = "Alibaba";
    char *seniorJavaDevJob = "高级Java开发";
    Application *pApplication = newApplication();
    execute(pApplication, "publish", employerAlibaba, seniorJavaDevJob, "ATS", NULL, NULL, NULL);
    LinkedList *jobs = getJobs(pApplication, employerAlibaba, "published");
    LinkedList *job = (LinkedList *) getItem(jobs, 0);
    ASSERT_STREQ("高级Java开发", (char *) getItem(job, 0));
    ASSERT_STREQ("ATS", (char *) getItem(job, 1));
}

TEST(ApplicationTest, employers_should_not_be_able_to_publish_jobs_that_are_neither_ATS_nor_JReq) {
    char *employerAlibaba = "Alibaba";
    char *seniorJavaDevJob = "高级Java开发";
    Application *pApplication = newApplication();
    int result = execute(pApplication, "publish", employerAlibaba, seniorJavaDevJob, "RJeq", NULL, NULL, NULL);
    ASSERT_EQ(result, 400);
}

TEST(ApplicationTest, jobseekers_should_be_able_to_save_jobs_published_by_employers_for_later_review) {
    char *employerAlibaba = "Alibaba";
    char *jobSeekerName = "Jacky";
    char *jobName = "高级Java开发";
    Application *pApplication = newApplication();
    execute(pApplication, "publish", employerAlibaba, jobName, "JReq", NULL, NULL, NULL);
    execute(pApplication, "save", jobSeekerName, jobName, "JReq", NULL, NULL, NULL);

    LinkedList *jobs = getJobs(pApplication, jobSeekerName, "published");
    LinkedList *job = (LinkedList *) getItem(jobs, 0);

    ASSERT_STREQ("高级Java开发", (char *) getItem(job, 0));
    ASSERT_STREQ("JReq", (char *) getItem(job, 1));
}

TEST(Application, jobseekers_should_be_able_to_apply_for_an_ATS_job_some_employer_published_without_a_resume) {
    char *employerAlibaba = "Alibaba";
    char *jobSeekerName = "Jacky";
    char *seniorJavaDevJob = "高级Java开发";
    char *juniorJavaDevJob = "Java开发";

    Application *pApplication = newApplication();
    execute(pApplication, "publish", employerAlibaba, seniorJavaDevJob, "ATS", NULL, NULL, NULL);
    execute(pApplication, "publish", employerAlibaba, juniorJavaDevJob, "ATS", NULL, NULL, NULL);

    execute(pApplication, "apply", employerAlibaba, juniorJavaDevJob, "ATS", jobSeekerName, NULL, "2020-01-01");
    execute(pApplication, "apply", employerAlibaba, seniorJavaDevJob, "ATS", jobSeekerName, NULL, "2020-01-01");
    LinkedList *jobs = getJobs(pApplication, jobSeekerName, "applied");

    LinkedList *job1 = (LinkedList *) getItem(jobs, 0);

    ASSERT_STREQ("Java开发", (char *) getItem(job1, 0));
    ASSERT_STREQ("ATS", (char *) getItem(job1, 1));
    ASSERT_STREQ("2020-01-01", (char *) getItem(job1, 2));
    ASSERT_STREQ("Alibaba", (char *) getItem(job1, 3));

    LinkedList *job2 = (LinkedList *) getItem(jobs, 1);
    ASSERT_STREQ("高级Java开发", (char *) getItem(job2, 0));
    ASSERT_STREQ("ATS", (char *) getItem(job2, 1));
    ASSERT_STREQ("2020-01-01", (char *) getItem(job2, 2));
    ASSERT_STREQ("Alibaba", (char *) getItem(job2, 3));
}

TEST(ApplicationTest, jobseekers_should_not_be_able_to_apply_for_an_JReq_job_some_employer_published_without_a_resume) {
    char *employerAlibaba = "Alibaba";
    char *jobSeekerName = "Jacky";
    char *seniorJavaDevJob = "高级Java开发";

    Application *pApplication = newApplication();
    execute(pApplication, "publish", employerAlibaba, seniorJavaDevJob, "JReq", NULL, NULL, NULL);
    int result = execute(pApplication, "apply", employerAlibaba, seniorJavaDevJob, "JReq", jobSeekerName, NULL, now());
    ASSERT_EQ(401, result);
}

TEST(ApplicationTest,
     jobseekers_should_not_be_able_to_apply_for_an_JReq_job_some_employer_published_with_someone_else_s_resume) {
    char *employerAlibaba = "Alibaba";
    char *jobSeekerName = "Jacky";
    char *seniorJavaDevJob = "高级Java开发";
    char *resumeApplicantName = "Jacky Chen";

    Application *pApplication = newApplication();
    execute(pApplication, "publish", employerAlibaba, seniorJavaDevJob, "JReq", NULL, NULL, NULL);
    int result = execute(pApplication, "apply", employerAlibaba, seniorJavaDevJob, "JReq", jobSeekerName,
                         resumeApplicantName, NULL);
    ASSERT_EQ(402, result);
}

TEST(ApplicationTest, employers_should_be_able_to_find_applicants_of_a_job) {
    char *employerAlibaba = "Alibaba";
    char *jobSeekerJacky = "Jacky";
    char *jobSeekerLam = "Lam";
    char *seniorJavaDevJob = "高级Java开发";

    Application *pApplication = newApplication();
    execute(pApplication, "publish", employerAlibaba, seniorJavaDevJob, "ATS", NULL, NULL, NULL);
    execute(pApplication, "apply", employerAlibaba, seniorJavaDevJob, "ATS", jobSeekerJacky, NULL, now());
    execute(pApplication, "apply", employerAlibaba, seniorJavaDevJob, "ATS", jobSeekerLam, NULL, now());

    LinkedList *applicants = findApplicants(pApplication, seniorJavaDevJob, employerAlibaba);

    ASSERT_STREQ("Jacky", (char *) getItem(applicants, 0));
    ASSERT_STREQ("Lam", (char *) getItem(applicants, 1));
}

TEST(ApplicationTest, employers_should_be_able_to_find_applicants_to_a_job_by_application_date) {
    char *employerAlibaba = "Alibaba";
    char *jobSeekerJacky = "Jacky";
    char *jobSeekerHo = "Ho";
    char *seniorJavaDevJob = "高级Java开发";
    Application *pApplication = newApplication();
    execute(pApplication, "publish", employerAlibaba, seniorJavaDevJob, "ATS", NULL, NULL, NULL);
    execute(pApplication, "apply", employerAlibaba, seniorJavaDevJob, "ATS", jobSeekerJacky, NULL, "1997-07-01");
    execute(pApplication, "apply", employerAlibaba, seniorJavaDevJob, "ATS", jobSeekerHo, NULL, "1999-12-20");

    LinkedList *applicants = findApplicantsFrom(pApplication, NULL, employerAlibaba, "1999-12-20");

    ASSERT_STREQ("Ho", (char *) getItem(applicants, 0));
}

TEST(ApplicationTest,
     employers_should_be_able_to_find_applicants_to_a_job_by_period_when_period_end_is_given_while_period_start_is_not) {
    char *employerAlibaba = "Alibaba";
    char *jobSeekerJacky = "Jacky";
    char *jobSeekerHo = "Ho";
    char *seniorJavaDevJob = "高级Java开发";
    Application *pApplication = newApplication();
    execute(pApplication, "publish", employerAlibaba, seniorJavaDevJob, "ATS", NULL, NULL, NULL);
    execute(pApplication, "apply", employerAlibaba, seniorJavaDevJob, "ATS", jobSeekerJacky, NULL, "1997-07-01");
    execute(pApplication, "apply", employerAlibaba, seniorJavaDevJob, "ATS", jobSeekerHo, NULL, "1999-12-20");

    LinkedList *applicants = findApplicantsIn(pApplication, NULL, employerAlibaba, NULL, "1999-12-20");

    ASSERT_STREQ("Jacky", (char *) getItem(applicants, 0));
}

TEST(ApplicationTest, employers_should_be_able_to_find_applicants_to_a_job_by_period) {
    char *employerAlibaba = "Alibaba";
    char *jobSeekerJacky = "Jacky";
    char *jobSeekerHo = "Ho";
    char *seniorJavaDevJob = "高级Java开发";
    Application *pApplication = newApplication();
    execute(pApplication, "publish", employerAlibaba, seniorJavaDevJob, "ATS", NULL, NULL, NULL);
    execute(pApplication, "apply", employerAlibaba, seniorJavaDevJob, "ATS", jobSeekerJacky, NULL, "1997-07-01");
    execute(pApplication, "apply", employerAlibaba, seniorJavaDevJob, "ATS", jobSeekerHo, NULL, "1999-12-20");

    LinkedList *applicants = findApplicantsIn(pApplication, NULL, employerAlibaba, "1997-07-01", "1999-12-20");

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
    execute(pApplication, "publish", employerAlibaba, juniorJavaDevJob, "ATS", NULL, NULL, NULL);
    execute(pApplication, "publish", employerAlibaba, seniorJavaDevJob, "JReq", NULL, NULL, NULL);

    execute(pApplication, "apply", employerAlibaba, juniorJavaDevJob, "ATS", jobSeekerJacky, NULL, "1997-07-01");
    execute(pApplication, "apply", employerAlibaba, seniorJavaDevJob, "JReq", jobSeekerJacky, resumeApplicantName,
            "1999-12-20");
    execute(pApplication, "apply", employerAlibaba, juniorJavaDevJob, "ATS", jobSeekerHo, NULL, "1999-12-20");

    LinkedList *applicants = findApplicantsFrom(pApplication, seniorJavaDevJob, employerAlibaba, "1999-12-20");

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
    execute(pApplication, "publish", employerAlibaba, seniorJavaDevJob, "ATS", NULL, NULL, NULL);
    execute(pApplication, "publish", employerAlibaba, juniorJavaDevJob, "ATS", NULL, NULL, NULL);

    execute(pApplication, "apply", employerAlibaba, juniorJavaDevJob, "ATS", jobSeekerJacky, NULL, "1997-07-01");
    execute(pApplication, "apply", employerAlibaba, seniorJavaDevJob, "ATS", jobSeekerJacky, NULL, "1997-07-01");
    execute(pApplication, "apply", employerAlibaba, juniorJavaDevJob, "ATS", jobSeekerHo, NULL, "1999-12-20");

    LinkedList *applicants = findApplicantsIn(pApplication, seniorJavaDevJob, employerAlibaba, NULL, "1999-01-01");

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
    execute(pApplication, "publish", employerAlibaba, seniorJavaDevJob, "ATS", NULL, NULL, NULL);
    execute(pApplication, "publish", employerAlibaba, juniorJavaDevJob, "ATS", NULL, NULL, NULL);

    execute(pApplication, "apply", employerAlibaba, seniorJavaDevJob, "ATS", jobSeekerWong, NULL, "1997-07-01");
    execute(pApplication, "apply", employerAlibaba, juniorJavaDevJob, "ATS", jobSeekerJacky, NULL, "1996-12-31");
    execute(pApplication, "apply", employerAlibaba, juniorJavaDevJob, "ATS", jobSeekerHo, NULL, "1998-01-01");
    execute(pApplication, "apply", employerAlibaba, juniorJavaDevJob, "ATS", jobSeekerLam, NULL, "1999-12-20");

    LinkedList *applicants = findApplicantsIn(pApplication, juniorJavaDevJob, employerAlibaba, "1997-01-01",
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
    execute(pApplication, "publish", employerAlibaba, juniorJavaDevJob, "ATS", NULL, NULL, NULL);
    execute(pApplication, "publish", employerAlibaba, seniorJavaDevJob, "JReq", NULL, NULL, NULL);

    execute(pApplication, "apply", employerAlibaba, juniorJavaDevJob, "ATS", jobSeekerJacky, NULL, "1997-07-01");
    execute(pApplication, "apply", employerAlibaba, seniorJavaDevJob, "JReq", jobSeekerJacky, jackyResume,
            "1999-12-20");
    execute(pApplication, "apply", employerAlibaba, juniorJavaDevJob, "ATS", jobSeekerHo, NULL, "1999-12-20");
    execute(pApplication, "apply", employerAlibaba, juniorJavaDevJob, "ATS", jobSeekerLam, NULL, "1999-12-20");
    execute(pApplication, "apply", employerAlibaba, seniorJavaDevJob, "JReq", jobSeekerLam, lamResume, "1999-12-20");

    char *csv = exportTo(pApplication, "csv", "1999-12-20");
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
    execute(pApplication, "publish", employerAlibaba, juniorJavaDevJob, "ATS", NULL, NULL, NULL);
    execute(pApplication, "publish", employerAlibaba, seniorJavaDevJob, "JReq", NULL, NULL, NULL);

    execute(pApplication, "apply", employerAlibaba, juniorJavaDevJob, "ATS", jobSeekerJacky, NULL, "1997-07-01");
    execute(pApplication, "apply", employerAlibaba, seniorJavaDevJob, "JReq", jobSeekerJacky, jackyResume,
            "1999-12-20");
    execute(pApplication, "apply", employerAlibaba, juniorJavaDevJob, "ATS", jobSeekerHo, NULL, "1999-12-20");
    execute(pApplication, "apply", employerAlibaba, juniorJavaDevJob, "ATS", jobSeekerLam, NULL, "1999-12-20");
    execute(pApplication, "apply", employerAlibaba, seniorJavaDevJob, "JReq", jobSeekerLam, lamResume, "1999-12-20");

    char *html = exportTo(pApplication, "html", "1999-12-20");
    ASSERT_STREQ(html,
                 "<!DOCTYPE html><body><table><thead><tr><th>Employer</th><th>Job</th><th>Job Type</th><th>Applicants</th><th>Date</th></tr></thead><tbody>"
                 "<tr><td>Alibaba</td><td>Senior Java Developer</td><td>JReq</td><td>Jacky</td><td>1999-12-20</td></tr>"
                 "<tr><td>Alibaba</td><td>Java Developer</td><td>ATS</td><td>Ho</td><td>1999-12-20</td></tr>"
                 "<tr><td>Alibaba</td><td>Java Developer</td><td>ATS</td><td>Lam</td><td>1999-12-20</td></tr>"
                 "<tr><td>Alibaba</td><td>Senior Java Developer</td><td>JReq</td><td>Lam</td><td>1999-12-20</td></tr>"
                 "</tbody></table></body></html>");
}
