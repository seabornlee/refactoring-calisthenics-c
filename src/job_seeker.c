#include "job_seeker.h"
#include "stdio.h"
#include "stdlib.h"

JobSeeker *newJobSeeker(char *name) {
    JobSeeker *jobSeeker = (JobSeeker *) malloc(sizeof(JobSeeker));
    if (jobSeeker == NULL) {
        printf("内存分配失败");
        return jobSeeker;
    }

    jobSeeker->name = name;
    return jobSeeker;
}
