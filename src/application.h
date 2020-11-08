#ifndef REFACTOR_CALISTHENICS_C_APPLICATION_H
#define REFACTOR_CALISTHENICS_C_APPLICATION_H

#include "time.h"
#include "linked_list.h"
#include "linked_map.h"

typedef struct application {
    LinkedMap *jobs;
    LinkedMap *applied;
} Application;

Application *newApplication();

int
execute(Application *application, char *command, char *employerName, char *jobName, char *jobType, char *jobSeekerName,
        char *resumeApplicantName,
        struct tm *applicationTime);

LinkedList *getJobs(Application *pApplication, char *employerName, const char *type);
LinkedList *findApplicants(Application *pApplication, char *jobName, char *employerName);
LinkedList *findApplicantsFrom(Application *pApplication, char *jobName, char *employerName, struct tm *from);
LinkedList *findApplicantsIn(Application *pApplication, char *jobName, char *employerName, struct tm *from, struct tm *to);
#endif //REFACTOR_CALISTHENICS_C_APPLICATION_H
