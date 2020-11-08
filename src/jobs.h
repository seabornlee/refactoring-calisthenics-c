#ifndef REFACTOR_CALISTHENICS_C_JOBS_H
#define REFACTOR_CALISTHENICS_C_JOBS_H

#include "linked_map.h"
#include "employer.h"
#include "job.h"
#include "job_seeker.h"

typedef struct jobs {
    LinkedMap *jobs;
} Jobs;

Jobs *newJobs();
void publishJob(Jobs *jobs, Employer *employer, Job *pJob);
LinkedList *getPublishedJobs(Jobs *jobs, Employer *employer);
void saveJob(Jobs *jobs, Job *job, JobSeeker *jobSeeker);
#endif //REFACTOR_CALISTHENICS_C_JOBS_H
