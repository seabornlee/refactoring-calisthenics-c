#include "job_application.h"
#include "stdio.h"
#include "stdlib.h"

JobApplication *newJobApplication(Job *job, Employer *employer, char *applicationTime) {
    JobApplication *pApplication = (JobApplication *) malloc(sizeof(JobApplication));
    if (pApplication == NULL) {
        printf("分配内存失败");
        return pApplication;
    }

    pApplication->job = job;
    pApplication->employer = employer;
    pApplication->applicationTime = applicationTime;
    return pApplication;
}
