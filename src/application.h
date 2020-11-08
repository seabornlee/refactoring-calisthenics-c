#ifndef REFACTOR_CALISTHENICS_C_APPLICATION_H
#define REFACTOR_CALISTHENICS_C_APPLICATION_H

#include "time.h"
#include "linked_list.h"
#include "linked_map.h"

typedef struct application {
    LinkedMap *jobs;
} Application;

Application *newApplication();

void
execute(Application *application, char *command, char *employerName, char *jobName, char *jobType, char *jobSeekerName,
        char *resumeApplicantName,
        long applicationTime);

LinkedList *getJobs(Application *pApplication, char *employerName, const char *type);

#endif //REFACTOR_CALISTHENICS_C_APPLICATION_H
