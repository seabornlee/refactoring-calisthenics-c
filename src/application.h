#ifndef REFACTOR_CALISTHENICS_C_APPLICATION_H
#define REFACTOR_CALISTHENICS_C_APPLICATION_H

#include "time.h"
#include "linked_list.h"
#include "linked_map.h"
#include "command.h"
#include "job_type.h"
#include "file_type.h"
#include "job_status.h"
#include "employer.h"
#include "job_seeker.h"
#include "job.h"
#include "resume.h"

typedef struct application {
    LinkedMap *jobs;
    LinkedMap *applied;
    LinkedList *failedApplications;
} Application;

Application *newApplication();

int
execute(Application *application, enum Command command, Employer *employer, Job *job, JobSeeker *jobSeeker,
        Resume *resume,
        char *applicationTime);

LinkedList *getJobs(Application *pApplication, char *name, enum JobStatus jobStatus);
LinkedList *findApplicants(Application *pApplication, Job *job);
LinkedList *findApplicantsFrom(Application *pApplication, Job *job, char *from);
LinkedList *findApplicantsIn(Application *pApplication, Job *job, char *from, char *to);
char *exportTo(Application *pApplication, enum FileType type, char *date);
int getSuccessfulApplications(Application *pApplication, Employer *employer, Job *job);
int getUnsuccessfulApplications(Application *pApplication, Employer *employer, Job *job);
#endif //REFACTOR_CALISTHENICS_C_APPLICATION_H
