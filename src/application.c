#include <string.h>
#include <stdio.h>
#include "application.h"
#include "stdlib.h"
#include "linked_list.h"
#include "linked_map.h"

Application *newApplication() {
    Application *pApplication = (Application *) malloc(sizeof(Application));
    if (pApplication == NULL) {
        return pApplication;
    }

    pApplication->jobs = newLinkedMap();
    pApplication->applied = newLinkedMap();
    pApplication->failedApplications = newLinkedList();
    return pApplication;
}

LinkedList *getJobs(Application *pApplication, char *employerName, enum JobStatus jobStatus) {
    if (jobStatus == APPLIED) {
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
                LinkedList *job = getItem(jobs, j);
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
                LinkedList *job = getItem(jobs, j);
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
                LinkedList *job = getItem(jobs, j);
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
    } else if (jobName == NULL) {
        LinkedList *pList = newLinkedList();

        LinkedMap *pMap = pApplication->applied;

        LinkedList *keys = keysOf(pMap);
        for (int i = 0; i < len(keys); ++i) {
            char *applicant = getItem(keys, i);
            LinkedList *jobs = getItemBy(pMap, applicant);
            for (int j = 0; j < len(jobs); ++j) {
                LinkedList *job = getItem(jobs, j);
                struct tm appliedAt = {0};
                strptime(getItem(job, 2), "%Y-%m-%d", &appliedAt);

                struct tm tmFrom = {0};
                strptime(from, "%Y-%m-%d", &tmFrom);

                struct tm tmTo = {0};
                strptime(to, "%Y-%m-%d", &tmTo);

                if (mktime(&appliedAt) >= mktime(&tmFrom) && mktime(&appliedAt) <= mktime(&tmTo)) {
                    addLast(pList, applicant);
                }
            }
        }
        return pList;
    } else if (from == NULL && to != NULL) {
        LinkedList *pList = newLinkedList();

        LinkedMap *pMap = pApplication->applied;

        LinkedList *keys = keysOf(pMap);
        for (int i = 0; i < len(keys); ++i) {
            char *applicant = getItem(keys, i);
            LinkedList *jobs = getItemBy(pMap, applicant);
            for (int j = 0; j < len(jobs); ++j) {
                LinkedList *job = getItem(jobs, j);
                struct tm appliedAt = {0};
                strptime(getItem(job, 2), "%Y-%m-%d", &appliedAt);

                struct tm tmTo = {0};
                strptime(to, "%Y-%m-%d", &tmTo);

                char *_jobName = getItem(job, 0);
                if (strcmp(jobName, _jobName) == 0 && mktime(&appliedAt) <= mktime(&tmTo)) {
                    addLast(pList, applicant);
                }
            }
        }
        return pList;
    } else if (from != NULL && to == NULL) {
        LinkedList *pList = newLinkedList();

        LinkedMap *pMap = pApplication->applied;

        LinkedList *keys = keysOf(pMap);
        for (int i = 0; i < len(keys); ++i) {
            char *applicant = getItem(keys, i);
            LinkedList *jobs = getItemBy(pMap, applicant);
            for (int j = 0; j < len(jobs); ++j) {
                LinkedList *job = getItem(jobs, j);
                struct tm appliedAt = {0};
                strptime(getItem(job, 2), "%Y-%m-%d", &appliedAt);

                struct tm tmFrom = {0};
                strptime(from, "%Y-%m-%d", &tmFrom);

                char *_jobName = getItem(job, 0);
                if (strcmp(jobName, _jobName) == 0 && mktime(&appliedAt) >= mktime(&tmFrom)) {
                    addLast(pList, applicant);
                }
            }
        }
        return pList;
    } else {
        LinkedList *pList = newLinkedList();

        LinkedMap *pMap = pApplication->applied;

        LinkedList *keys = keysOf(pMap);
        for (int i = 0; i < len(keys); ++i) {
            char *applicant = getItem(keys, i);
            LinkedList *jobs = getItemBy(pMap, applicant);
            for (int j = 0; j < len(jobs); ++j) {
                LinkedList *job = getItem(jobs, j);
                struct tm appliedAt = {0};
                strptime(getItem(job, 2), "%Y-%m-%d", &appliedAt);

                struct tm tmFrom = {0};
                strptime(from, "%Y-%m-%d", &tmFrom);

                struct tm tmTo = {0};
                strptime(to, "%Y-%m-%d", &tmTo);

                char *_jobName = getItem(job, 0);
                if (strcmp(jobName, _jobName) == 0 && mktime(&appliedAt) >= mktime(&tmFrom) &&
                    mktime(&appliedAt) <= mktime(&tmTo)) {
                    addLast(pList, applicant);
                }
            }
        }
        return pList;
    }
}

char *exportTo(Application *pApplication, enum FileType type, char *date) {
    if (type == CSV) {
        LinkedMap *pMap = pApplication->applied;

        char res[1024] = {0};
        LinkedList *keys = keysOf(pMap);

        for (int i = 0; i < len(keys); ++i) {
            char *applicant = getItem(keys, i);
            LinkedList *jobs = getItemBy(pMap, applicant);
            for (int j = 0; j < len(jobs); ++j) {
                LinkedList *job = getItem(jobs, j);
                struct tm appliedAt = {0};
                strptime(getItem(job, 2), "%Y-%m-%d", &appliedAt);

                struct tm tmDate = {0};
                strptime(date, "%Y-%m-%d", &tmDate);

                if (mktime(&appliedAt) == mktime(&tmDate)) {
                    char str[200];
                    sprintf(str, "%s,%s,%s,%s,%s\n", getItem(job, 3), getItem(job, 0), getItem(job, 1), applicant, getItem(job, 2));
                    strcat(res, str);
                }
            }
        }

        char result[1024] = {0};
        strcat(result, "Employer,Job,Job Type,Applicants,Date\n");
        strcat(result, res);
        return strdup(result);
    } else {
        LinkedMap *pMap = pApplication->applied;

        char res[1024] = {0};
        LinkedList *keys = keysOf(pMap);

        for (int i = 0; i < len(keys); ++i) {
            char *applicant = getItem(keys, i);
            LinkedList *jobs = getItemBy(pMap, applicant);
            for (int j = 0; j < len(jobs); ++j) {
                LinkedList *job = getItem(jobs, j);
                struct tm appliedAt = {0};
                strptime(getItem(job, 2), "%Y-%m-%d", &appliedAt);

                struct tm tmDate = {0};
                strptime(date, "%Y-%m-%d", &tmDate);

                if (mktime(&appliedAt) == mktime(&tmDate)) {
                    char str[200];
                    sprintf(str, "<tr><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td></tr>", getItem(job, 3), getItem(job, 0), getItem(job, 1), applicant, getItem(job, 2));
                    strcat(res, str);
                }
            }
        }

        char result[1024] = {0};
        strcat(result, "<!DOCTYPE html><body><table><thead><tr><th>Employer</th><th>Job</th><th>Job Type</th><th>Applicants</th><th>Date</th></tr></thead><tbody>");
        strcat(result, res);
        strcat(result, "</tbody></table></body></html>");
        return strdup(result);
    }
}

int getSuccessfulApplications(Application *pApplication, char *employerName, char *jobName) {
    int count = 0;
    LinkedMap *pMap = pApplication->applied;

    LinkedList *keys = keysOf(pMap);
    for (int i = 0; i < len(keys); ++i) {
        char *applicant = getItem(keys, i);
        LinkedList *jobs = getItemBy(pMap, applicant);
        for (int j = 0; j < len(jobs); ++j) {
            LinkedList *job = getItem(jobs, j);

            char *_jobName = getItem(job, 0);
            if (strcmp(jobName, _jobName) == 0 && strcmp(employerName, getItem(job, 3)) == 0) {
                count++;
            }
        }
    }
    return count;
}

int getUnsuccessfulApplications(Application *pApplication, char *employerName, char *jobName) {
    int count = 0;
    for (int i = 0; i < len(pApplication->failedApplications); ++i) {
        LinkedList *job = getItem(pApplication->failedApplications, i);

        char *_jobName = getItem(job, 0);
        if (strcmp(jobName, _jobName) == 0 && strcmp(employerName, getItem(job, 3)) == 0) {
            count++;
        }
    }
    return count;
}

int execute(Application *application, enum Command command, char *employerName, char *jobName, enum JobType jobType,
            char *jobSeekerName, char *resumeApplicantName, char *applicationTime) {
    {
        if (command == PUBLISH) {
            LinkedList *job = newLinkedList();
            addLast(job, jobName);
            addLast(job, toStrJobType(jobType));

            LinkedList *alreadyPublished = getOrDefault(application->jobs, employerName, newLinkedList());
            addLast(alreadyPublished, job);

            putItem(application->jobs, employerName, alreadyPublished);
        } else if (command == SAVE) {
            LinkedList *job = newLinkedList();
            addLast(job, jobName);
            addLast(job, toStrJobType(jobType));

            LinkedList *saved = getOrDefault(application->jobs, employerName, newLinkedList());
            addLast(saved, job);

            putItem(application->jobs, employerName, saved);
        } else if (command == APPLY) {

            if (jobType == JReq && resumeApplicantName == NULL) {
                LinkedList *failedApplication = newLinkedList();
                addLast(failedApplication, jobName);
                addLast(failedApplication, jobType);
                addLast(failedApplication, applicationTime);
                addLast(failedApplication, employerName);
                addLast(application->failedApplications, failedApplication);
                printf("需要提供简历才能申请 JReq 类型的工作\r\n");
                return 401;
            }

            if (jobType == JReq && strcmp(jobSeekerName, resumeApplicantName) != 0) {
                printf("请用自己的简历申请工作\r\n");
                return 402;
            }

            LinkedList *job = newLinkedList();
            addLast(job, jobName);
            addLast(job, toStrJobType(jobType));

            addLast(job, applicationTime);
            addLast(job, employerName);

            LinkedList *saved = getOrDefault(application->applied, jobSeekerName, newLinkedList());
            addLast(saved, job);
            putItem(application->applied, jobSeekerName, saved);
        }

        return 0;
    }
}
