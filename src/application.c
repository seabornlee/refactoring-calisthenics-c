#include <string.h>
#include <stdio.h>
#include "application.h"
#include "stdlib.h"
#include "linked_list.h"
#include "linked_map.h"

int
execute(Application *application, char *command, char *employerName, char *jobName, char *jobType, char *jobSeekerName,
        char *resumeApplicantName,
        struct tm applicationTime) {
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
    } else if (strcmp(command, "save") == 0) {
        LinkedList *job = newLinkedList();
        addLast(job, jobName);
        addLast(job, jobType);

        LinkedList *saved = getOrDefault(application->jobs, employerName, newLinkedList());
        addLast(saved, job);

        putItem(application->jobs, employerName, saved);
    } else if (strcmp(command, "apply") == 0) {

        if (strcmp(jobType, "JReq") == 0 && resumeApplicantName == NULL) {
//            LinkedList *failedApplication = newLinkedList();
//            addLast(failedApplication, jobName);
//            addLast(failedApplication, jobType);
//            addLast(failedApplication, "");
//            addLast(failedApplication, employerName);
////                    add(applicationTime.format(DateTimeFormatter.ofPattern("yyyy-MM-dd")));
//            failedApplications.add(failedApplication);
            return 401;
        }
//
//        if (jobType.equals("JReq") && !resumeApplicantName.equals(jobSeekerName)) {
//            throw new InvalidResumeException();
//        }

        LinkedList *job = newLinkedList();
        addLast(job, jobName);
        addLast(job, jobType);

        char buf[256] = {0};
        strftime(buf, 256, "%Y-%m-%d", &applicationTime);

        addLast(job, strdup(buf));
        addLast(job, employerName);

        LinkedList *saved = getOrDefault(application->applied, jobSeekerName, newLinkedList());
        addLast(saved, job);
        putItem(application->applied, jobSeekerName, saved);
    }

    return 0;
}

Application *newApplication() {
    Application *pApplication = (Application *) malloc(sizeof(Application));
    if (pApplication == NULL) {
        return pApplication;
    }

    pApplication->jobs = newLinkedMap();
    pApplication->applied = newLinkedMap();
    return pApplication;
}

LinkedList *getJobs(Application *pApplication, char *employerName, const char *type) {
    if (strcmp(type, "applied") == 0) {
        return getItemBy(pApplication->applied, employerName);
    }
    return getItemBy(pApplication->jobs, employerName);
}
