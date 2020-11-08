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