#ifndef REFACTOR_CALISTHENICS_C_JOB_APPLICATION_H
#define REFACTOR_CALISTHENICS_C_JOB_APPLICATION_H
#include "job.h"
#include "employer.h"

typedef struct job_application {
    Job *job;
    char *applicationTime;
    Employer *employer;
} JobApplication;

JobApplication *newJobApplication(Job *job, Employer *employer, char *applicationTime);
#endif //REFACTOR_CALISTHENICS_C_JOB_APPLICATION_H
