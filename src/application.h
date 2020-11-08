#ifndef REFACTOR_CALISTHENICS_C_APPLICATION_H
#define REFACTOR_CALISTHENICS_C_APPLICATION_H

#include "time.h"
#include "linked_list.h"
#include "linked_map.h"
#include "command.h"
#include "job_type.h"
#include "file_type.h"
#include "job_status.h"

typedef struct application {
    LinkedMap *jobs;
    LinkedMap *applied;
    LinkedList *failedApplications;
} Application;

Application *newApplication();

int
execute(Application *application, enum Command command, char *employerName, char *jobName, enum JobType jobType, char *jobSeekerName,
        char *resumeApplicantName,
        char *applicationTime);

LinkedList *getJobs(Application *pApplication, char *employerName, enum JobStatus jobStatus);
LinkedList *findApplicants(Application *pApplication, char *jobName, char *employerName);
LinkedList *findApplicantsFrom(Application *pApplication, char *jobName, char *employerName, char *from);
LinkedList *findApplicantsIn(Application *pApplication, char *jobName, char *employerName, char *from, char *to);
char *exportTo(Application *pApplication, enum FileType type, char *date);
int getSuccessfulApplications(Application *pApplication, char *employerName, char *jobName);
int getUnsuccessfulApplications(Application *pApplication, char *employerName, char *jobName);
#endif //REFACTOR_CALISTHENICS_C_APPLICATION_H
