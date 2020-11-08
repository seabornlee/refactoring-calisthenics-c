#include <string.h>
#include "application.h"
#include "stdlib.h"
#include "linked_list.h"
#include "linked_map.h"

int
execute(Application *application, char *command, char *employerName, char *jobName, char *jobType, char *jobSeekerName,
        char *resumeApplicantName,
        long applicationTime) {
    if (strcmp(command, "publish") == 0) {
        if (strcmp(jobType, "JReq") != 0 && strcmp(jobType, "ATS") != 0) {
            return 400;
        }

        LinkedList *job = newLinkedList();
        addLast(job, jobName);
        addLast(job, jobType);

        LinkedList *alreadyPublished = getOrDefault(application->jobs, employerName, newLinkedList());
        addLast(alreadyPublished, job);

        putItem(application->jobs, employerName, alreadyPublished);
    }

    return 0;
}

Application *newApplication() {
    Application *pApplication = (Application *) malloc(sizeof(Application));
    if (pApplication == NULL) {
        return pApplication;
    }

    pApplication->jobs = newLinkedMap();

    return pApplication;
}

LinkedList *getJobs(Application *pApplication, char *employerName, const char *type) {
    return getItemBy(pApplication->jobs, employerName);
}
