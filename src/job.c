#include "job.h"
#include "stdio.h"
#include "stdlib.h"

Job *newJob(char *name) {
    Job *job = (Job *) malloc(sizeof(Job));
    if (job == NULL) {
        printf("内存分配失败");
        return job;
    }

    job->name = name;
    return job;
}

Job *newJobWithType(char *name, enum JobType jobType) {
    Job *pJob = newJob(name);
    if (pJob == NULL) {
        return pJob;
    }

    pJob->jobType = jobType;
    return pJob;
}
