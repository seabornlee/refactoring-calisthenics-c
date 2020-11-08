#include <gtest/gtest.h>

extern "C" {
#include "application.h"
#include <linked_list.h>
}

TEST(ApplicationTest, employers_should_be_able_to_publish_a_job) {
    char *employerName = "";
    char *jobName = "高级前端开发";
    Application *pApplication = newApplication();
    execute(pApplication, "publish", employerName, jobName, "JReq", NULL, NULL, 0L);
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
    execute(pApplication, "publish", employerAlibaba, seniorJavaDevJob, "JReq", NULL, NULL, 0L);
    execute(pApplication, "publish", employerTencent, juniorJavaDevJob, "JReq", NULL, NULL, 0L);
    LinkedList *jobs = getJobs(pApplication, employerAlibaba, "published");
    ASSERT_EQ(1, len(jobs));
    LinkedList *job = (LinkedList *) getItem(jobs, 0);
    ASSERT_STREQ("高级Java开发", (char *) getItem(job, 0));
}

TEST(ApplicationTest, employers_should_be_able_to_publish_ATS_jobs) {
    char *employerAlibaba = "Alibaba";
    char *seniorJavaDevJob = "高级Java开发";
    Application *pApplication = newApplication();
    execute(pApplication, "publish", employerAlibaba, seniorJavaDevJob, "ATS", NULL, NULL, 0L);
    LinkedList *jobs = getJobs(pApplication, employerAlibaba, "published");
    LinkedList *job = (LinkedList *) getItem(jobs, 0);
    ASSERT_STREQ("高级Java开发", (char *) getItem(job, 0));
    ASSERT_STREQ("ATS", (char *) getItem(job, 1));
}