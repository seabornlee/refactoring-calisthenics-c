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

LinkedList *getJobs(Application *pApplication, char *name, enum JobStatus jobStatus) {
    if (jobStatus == APPLIED) {
        return getItemBy(pApplication->applied, name);
    }
    return getItemBy(pApplication->jobs, name);
}

LinkedList *findApplicants(Application *pApplication, Job *job) {
    return findApplicantsFrom(pApplication, job, NULL);
}

LinkedList *findApplicantsFrom(Application *pApplication, Job *job, char *from) {
    return findApplicantsIn(pApplication, job, from, NULL);
}

LinkedList *
findApplicantsIn(Application *pApplication, Job *pJob, char *from, char *to) {
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
                if (strcmp(_jobName, pJob->name) == 0) {
                    addLast(pList, applicant);
                }
            }
        }
        return pList;
    } else if (pJob == NULL && to == NULL) {
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

    } else if (pJob == NULL && from == NULL) {
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
    } else if (pJob == NULL) {
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
                if (strcmp(pJob->name, _jobName) == 0 && mktime(&appliedAt) <= mktime(&tmTo)) {
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
                if (strcmp(pJob->name, _jobName) == 0 && mktime(&appliedAt) >= mktime(&tmFrom)) {
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
                if (strcmp(pJob->name, _jobName) == 0 && mktime(&appliedAt) >= mktime(&tmFrom) &&
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
                    sprintf(str, "%s,%s,%s,%s,%s\n", getItem(job, 3), getItem(job, 0), getItem(job, 1), applicant,
                            getItem(job, 2));
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
                    sprintf(str, "<tr><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td></tr>", getItem(job, 3),
                            getItem(job, 0), getItem(job, 1), applicant, getItem(job, 2));
                    strcat(res, str);
                }
            }
        }

        char result[1024] = {0};
        strcat(result,
               "<!DOCTYPE html><body><table><thead><tr><th>Employer</th><th>Job</th><th>Job Type</th><th>Applicants</th><th>Date</th></tr></thead><tbody>");
        strcat(result, res);
        strcat(result, "</tbody></table></body></html>");
        return strdup(result);
    }
}

int getSuccessfulApplications(Application *pApplication, Employer *employer, Job *pJob) {
    int count = 0;
    LinkedMap *pMap = pApplication->applied;

    LinkedList *keys = keysOf(pMap);
    for (int i = 0; i < len(keys); ++i) {
        char *applicant = getItem(keys, i);
        LinkedList *jobs = getItemBy(pMap, applicant);
        for (int j = 0; j < len(jobs); ++j) {
            LinkedList *job = getItem(jobs, j);

            char *_jobName = getItem(job, 0);
            if (strcmp(pJob->name, _jobName) == 0 && strcmp(employer->name, getItem(job, 3)) == 0) {
                count++;
            }
        }
    }
    return count;
}

int getUnsuccessfulApplications(Application *pApplication, Employer *employer, Job *pJob) {
    int count = 0;
    for (int i = 0; i < len(pApplication->failedApplications); ++i) {
        LinkedList *job = getItem(pApplication->failedApplications, i);

        char *_jobName = getItem(job, 0);
        if (strcmp(pJob->name, _jobName) == 0 && strcmp(employer->name, getItem(job, 3)) == 0) {
            count++;
        }
    }
    return count;
}

int
execute(Application *application, enum Command command, Employer *employer, Job *pJob,
        JobSeeker *jobSeeker, Resume *resume, char *applicationTime) {
    if (command == PUBLISH) {
        LinkedList *job = newLinkedList();
        addLast(job, pJob->name);
        addLast(job, toStrJobType(pJob->jobType));

        LinkedList *alreadyPublished = getOrDefault(application->jobs, employer->name, newLinkedList());
        addLast(alreadyPublished, job);

        putItem(application->jobs, employer->name, alreadyPublished);
    } else if (command == SAVE) {
        LinkedList *job = newLinkedList();
        addLast(job, pJob->name);
        addLast(job, toStrJobType(pJob->jobType));

        LinkedList *saved = getOrDefault(application->jobs, jobSeeker->name, newLinkedList());
        addLast(saved, job);

        putItem(application->jobs, jobSeeker->name, saved);
    } else if (command == APPLY) {

        if (pJob->jobType == JReq && resume == NULL) {
            LinkedList *failedApplication = newLinkedList();
            addLast(failedApplication, pJob->name);
            addLast(failedApplication, toStrJobType(pJob->jobType));
            addLast(failedApplication, applicationTime);
            addLast(failedApplication, employer->name);
            addLast(application->failedApplications, failedApplication);
            printf("需要提供简历才能申请 JReq 类型的工作\r\n");
            return 401;
        }

        if (pJob->jobType == JReq && strcmp(jobSeeker->name, resume->applicantName) != 0) {
            printf("请用自己的简历申请工作\r\n");
            return 402;
        }

        LinkedList *job = newLinkedList();
        addLast(job, pJob->name);
        addLast(job, toStrJobType(pJob->jobType));

        addLast(job, applicationTime);
        addLast(job, employer->name);

        LinkedList *saved = getOrDefault(application->applied, jobSeeker->name, newLinkedList());
        addLast(saved, job);
        putItem(application->applied, jobSeeker->name, saved);
    }

    return 0;

}
