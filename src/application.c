#include <string.h>
#include <stdio.h>
#include "application.h"
#include "stdlib.h"
#include "linked_list.h"
#include "linked_map.h"

int
execute(Application *application, char *command, char *employerName, char *jobName, char *jobType, char *jobSeekerName,
        char *resumeApplicantName,
        char *applicationTime) {
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
            printf("需要提供简历才能申请 JReq 类型的工作\r\n");
            return 401;
        }
        if (strcmp(jobType, "JReq") == 0 && strcmp(jobSeekerName, resumeApplicantName) != 0) {
            printf("请用自己的简历申请工作\r\n");
            return 402;
        }

        LinkedList *job = newLinkedList();
        addLast(job, jobName);
        addLast(job, jobType);

        addLast(job, applicationTime);
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

LinkedList *findApplicants(Application *pApplication, char *jobName, char *employerName) {
    return findApplicantsFrom(pApplication, jobName, employerName, NULL);
}

LinkedList *findApplicantsFrom(Application *pApplication, char *jobName, char *employerName, char *from) {
    return findApplicantsIn(pApplication, jobName, employerName, from, NULL);
}

LinkedList *
findApplicantsIn(Application *pApplication, char *jobName, char *employerName, char *from, char *to) {
    if (from == NULL && to == NULL) {
        LinkedList *pList = newLinkedList();

        LinkedMap *pMap = pApplication->applied;

        LinkedList *keys = keysOf(pMap);
        for (int i = 0; i < len(keys); ++i) {
            char *applicant = getItem(keys, i);
            LinkedList *jobs = getItemBy(pMap, applicant);
            for (int j = 0; j < len(jobs); ++j) {
                LinkedList *job = getItem(jobs, 0);
                char *_jobName = getItem(job, 0);
                if (strcmp(_jobName, jobName) == 0) {
                    addLast(pList, applicant);
                }
            }
        }
        return pList;
    } else if (jobName == NULL && to == NULL) {
        LinkedList *pList = newLinkedList();

        LinkedMap *pMap = pApplication->applied;

        LinkedList *keys = keysOf(pMap);
        for (int i = 0; i < len(keys); ++i) {
            char *applicant = getItem(keys, i);
            LinkedList *jobs = getItemBy(pMap, applicant);
            for (int j = 0; j < len(jobs); ++j) {
                LinkedList *job = getItem(jobs, 0);
                struct tm appliedAt = {0};
                strptime(getItem(job, 2), "%Y-%m-%d", &appliedAt);

                struct tm tmFrom = {0};
                strptime(from, "%Y-%m-%d", &tmFrom);

                if (mktime(&appliedAt) >= mktime(&tmFrom)) {
                    addLast(pList, applicant);
                }
            }
        }
        return pList;

    } else if (jobName == NULL && from == NULL) {
        LinkedList *pList = newLinkedList();

        LinkedMap *pMap = pApplication->applied;

        LinkedList *keys = keysOf(pMap);
        for (int i = 0; i < len(keys); ++i) {
            char *applicant = getItem(keys, i);
            LinkedList *jobs = getItemBy(pMap, applicant);
            for (int j = 0; j < len(jobs); ++j) {
                LinkedList *job = getItem(jobs, 0);
                struct tm appliedAt = {0};
                strptime(getItem(job, 2), "%Y-%m-%d", &appliedAt);

                struct tm tmTo = {0};
                strptime(to, "%Y-%m-%d", &tmTo);

                if (mktime(&appliedAt) < mktime(&tmTo)) {
                    addLast(pList, applicant);
                }
            }
        }
        return pList;
    }
    return NULL;
}
