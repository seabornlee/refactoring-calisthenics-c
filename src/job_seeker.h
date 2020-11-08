#ifndef REFACTOR_CALISTHENICS_C_JOB_SEEKER_H
#define REFACTOR_CALISTHENICS_C_JOB_SEEKER_H

typedef struct job_seeker {
    char *name;
} JobSeeker;

JobSeeker *newJobSeeker(char *name);
#endif //REFACTOR_CALISTHENICS_C_JOB_SEEKER_H
