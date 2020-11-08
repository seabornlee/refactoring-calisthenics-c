#ifndef REFACTOR_CALISTHENICS_C_APPLIED_JOBS_H
#define REFACTOR_CALISTHENICS_C_APPLIED_JOBS_H

#include "linked_map.h"
#include "employer.h"
#include "job.h"
#include "job_seeker.h"

typedef struct applied_jobs {
    LinkedMap *applied;
} AppliedJobs;

AppliedJobs *newAppliedJobs();

void applyJob(AppliedJobs *appliedJobs, Employer *employer, Job *pJob, JobSeeker *jobSeeker,
              const char *applicationTime);

void filterApplicantsAndJobs(AppliedJobs *appliedJobs, const char *date, const LinkedList *applicants,
                             const LinkedList *jobFiltered);

char *exportCSV(AppliedJobs *appliedJobs, char *date);

char *exportHTML(AppliedJobs *appliedJobs, char *date);

LinkedList *
findApplicantsIn(AppliedJobs *appliedJobs, Job *pJob, char *from, char *to);

LinkedList *getJobsByJobSeeker(AppliedJobs *appliedJobs, JobSeeker *jobSeeker);

#endif //REFACTOR_CALISTHENICS_C_APPLIED_JOBS_H
