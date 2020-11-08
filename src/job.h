#ifndef REFACTOR_CALISTHENICS_C_JOB_H
#define REFACTOR_CALISTHENICS_C_JOB_H

#include "job_type.h"

typedef struct job {
    char *name;
    enum JobType jobType;
} Job;

Job *newJob(char *name);
Job *newJobWithType(char *name, enum JobType jobType);
#endif //REFACTOR_CALISTHENICS_C_JOB_H
