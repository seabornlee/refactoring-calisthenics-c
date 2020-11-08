#include <gtest/gtest.h>

extern "C" {
#include "application.h"
#include <linked_list.h>
}

TEST(ApplicationTest, employers_should_be_able_to_publish_a_job) {
    char *employerName = "";
    char *jobName = "高级前端开发";
    Application *pApplication = newApplication();
    execute(pApplication, "publish", employerName, jobName, "JReq", NULL, NULL, {0});
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
    execute(pApplication, "publish", employerAlibaba, seniorJavaDevJob, "JReq", NULL, NULL, {0});
    execute(pApplication, "publish", employerTencent, juniorJavaDevJob, "JReq", NULL, NULL, {0});
    LinkedList *jobs = getJobs(pApplication, employerAlibaba, "published");
    ASSERT_EQ(1, len(jobs));
    LinkedList *job = (LinkedList *) getItem(jobs, 0);
    ASSERT_STREQ("高级Java开发", (char *) getItem(job, 0));
}

TEST(ApplicationTest, employers_should_be_able_to_publish_ATS_jobs) {
    char *employerAlibaba = "Alibaba";
    char *seniorJavaDevJob = "高级Java开发";
    Application *pApplication = newApplication();
    execute(pApplication, "publish", employerAlibaba, seniorJavaDevJob, "ATS", NULL, NULL, {0});
    LinkedList *jobs = getJobs(pApplication, employerAlibaba, "published");
    LinkedList *job = (LinkedList *) getItem(jobs, 0);
    ASSERT_STREQ("高级Java开发", (char *) getItem(job, 0));
    ASSERT_STREQ("ATS", (char *) getItem(job, 1));
}

TEST(ApplicationTest, employers_should_not_be_able_to_publish_jobs_that_are_neither_ATS_nor_JReq) {
    char *employerAlibaba = "Alibaba";
    char *seniorJavaDevJob = "高级Java开发";
    Application *pApplication = newApplication();
    int result = execute(pApplication, "publish", employerAlibaba, seniorJavaDevJob, "RJeq", NULL, NULL, {0});
    ASSERT_EQ(result, 400);
}

TEST(ApplicationTest, jobseekers_should_be_able_to_save_jobs_published_by_employers_for_later_review) {
    char *employerAlibaba = "Alibaba";
    char *jobSeekerName = "Jacky";
    char *jobName = "高级Java开发";
    Application *pApplication = newApplication();
    execute(pApplication, "publish", employerAlibaba, jobName, "JReq", NULL, NULL, {0});
    execute(pApplication, "save", jobSeekerName, jobName, "JReq", NULL, NULL, {0});

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
    execute(pApplication, "publish", employerAlibaba, seniorJavaDevJob, "ATS", NULL, NULL, {0});
    execute(pApplication, "publish", employerAlibaba, juniorJavaDevJob, "ATS", NULL, NULL, {0});

    struct tm tm = {0};
    strptime("2020-01-01", "%Y-%m-%d", &tm);
    execute(pApplication, "apply", employerAlibaba, juniorJavaDevJob, "ATS", jobSeekerName, NULL, tm);
    execute(pApplication, "apply", employerAlibaba, seniorJavaDevJob, "ATS", jobSeekerName, NULL, tm);
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
    execute(pApplication, "publish", employerAlibaba, seniorJavaDevJob, "JReq", NULL, NULL, {0});
    int result = execute(pApplication, "apply", employerAlibaba, seniorJavaDevJob, "JReq", jobSeekerName, NULL, {0});
    ASSERT_EQ(401, result);
}