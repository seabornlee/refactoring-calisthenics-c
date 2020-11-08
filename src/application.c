#include <string.h>
#include <stdio.h>
#include "application.h"
#include "stdlib.h"
#include "linked_list.h"
#include "linked_map.h"

int matchJobName(const Job *pJob, const LinkedList *job);

int matchJobNameAndDate(const Job *pJob, const char *from, const char *to, const LinkedList *jobs);

char *exportCSV(const Application *pApplication, const char *date);

char *exportHTML(const Application *pApplication, const char *date);

void filterApplicantsAndJobs(const Application *pApplication, const char *date, const LinkedList *applicants,
                             const LinkedList *jobFiltered);

int getCountByEmployerAndJobName(const Employer *employer, const Job *pJob, const LinkedList *list);

void publish(const Application *application, const Employer *employer, const Job *pJob);

void save(const Application *application, const Job *pJob, const JobSeeker *jobSeeker);

int apply(const Application *application, const Employer *employer, const Job *pJob, const JobSeeker *jobSeeker,
          const Resume *resume, const char *applicationTime);

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
    LinkedList *applicants = newLinkedList();

    LinkedList *keys = keysOf(pApplication->applied);
    for (int i = 0; i < len(keys); ++i) {
        char *applicant = getItem(keys, i);
        LinkedList *jobs = getItemBy(pApplication->applied, applicant);

        if (matchJobNameAndDate(pJob, from, to, jobs)) {
            addLast(applicants, applicant);
        }
    }
    return applicants;
}

int matchJobNameAndDate(const Job *pJob, const char *from, const char *to, const LinkedList *jobs) {
    for (int j = 0; j < len(jobs); ++j) {
        LinkedList *job = getItem(jobs, j);
        struct tm appliedAt = {0};
        strptime(getItem(job, 2), "%Y-%m-%d", &appliedAt);

        struct tm tmFrom = {0};
        if (from != NULL) {
            strptime(from, "%Y-%m-%d", &tmFrom);
        }

        struct tm tmTo = {0};
        if (to != NULL) {
            strptime(to, "%Y-%m-%d", &tmTo);
        }

        if ((pJob == NULL || matchJobName(pJob, job))
            && (from == NULL || mktime(&appliedAt) >= mktime(&tmFrom))
            && (to == NULL || mktime(&appliedAt) <= mktime(&tmTo))) {
            return 1;
        }
    }
    return 0;
}

int matchJobName(const Job *pJob, const LinkedList *job) {
    char *_jobName = getItem(job, 0);
    return strcmp(pJob->name, _jobName) == 0;
}

char *exportTo(Application *pApplication, enum FileType type, char *date) {
    if (type == CSV) {
        return exportCSV(pApplication, date);
    }
    return exportHTML(pApplication, date);
}

char *exportHTML(const Application *pApplication, const char *date) {
    LinkedList *applicants = newLinkedList();
    LinkedList *jobFiltered = newLinkedList();

    filterApplicantsAndJobs(pApplication, date, applicants, jobFiltered);

    char res[1024] = {0};
    for (int i = 0; i < len(applicants); ++i) {
        void *job = getItem(jobFiltered, i);
        char str[200];
        sprintf(str, "<tr><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td></tr>", getItem(job, 3),
                getItem(job, 0), getItem(job, 1), getItem(applicants, i), getItem(job, 2));
        strcat(res, str);
    }

    char result[1024] = {0};
    strcat(result,
           "<!DOCTYPE html><body><table><thead><tr><th>Employer</th><th>Job</th><th>Job Type</th><th>Applicants</th><th>Date</th></tr></thead><tbody>");
    strcat(result, res);
    strcat(result, "</tbody></table></body></html>");
    return strdup(result);
}

void filterApplicantsAndJobs(const Application *pApplication, const char *date, const LinkedList *applicants,
                             const LinkedList *jobFiltered) {
    LinkedMap *pMap = pApplication->applied;
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
                addLast(applicants, applicant);
                addLast(jobFiltered, job);
            }
        }
    }
}

char *exportCSV(const Application *pApplication, const char *date) {
    LinkedList *applicants = newLinkedList();
    LinkedList *jobFiltered = newLinkedList();

    filterApplicantsAndJobs(pApplication, date, applicants, jobFiltered);

    char res[1024] = {0};
    for (int i = 0; i < len(applicants); ++i) {
        void *job = getItem(jobFiltered, i);
        char str[200];
        sprintf(str, "%s,%s,%s,%s,%s\n", getItem(job, 3), getItem(job, 0), getItem(job, 1), getItem(applicants, i),
                getItem(job, 2));
        strcat(res, str);
    }

    char result[1024] = {0};
    strcat(result, "Employer,Job,Job Type,Applicants,Date\n");
    strcat(result, res);
    return strdup(result);
}

int getSuccessfulApplications(Application *pApplication, Employer *employer, Job *pJob) {
    LinkedList *keys = keysOf(pApplication->applied);

    int count = 0;
    for (int i = 0; i < len(keys); ++i) {
        char *applicant = getItem(keys, i);
        LinkedList *jobs = getItemBy(pApplication->applied, applicant);
        count += getCountByEmployerAndJobName(employer, pJob, jobs);
    }
    return count;
}

int getUnsuccessfulApplications(Application *pApplication, Employer *employer, Job *pJob) {
    return getCountByEmployerAndJobName(employer, pJob, pApplication->failedApplications);
}

int getCountByEmployerAndJobName(const Employer *employer, const Job *pJob, const LinkedList *list) {
    int count = 0;
    for (int i = 0; i < len(list); ++i) {
        LinkedList *job = getItem(list, i);

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
        publish(application, employer, pJob);
        return 0;
    }

    if (command == SAVE) {
        save(application, pJob, jobSeeker);
        return 0;
    }

    if (command == APPLY) {
        return apply(application, employer, pJob, jobSeeker, resume, applicationTime);
    }

    return 0;

}

int apply(const Application *application, const Employer *employer, const Job *pJob, const JobSeeker *jobSeeker,
          const Resume *resume, const char *applicationTime) {
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
    return 0;
}

void save(const Application *application, const Job *pJob, const JobSeeker *jobSeeker) {
    LinkedList *job = newLinkedList();
    addLast(job, pJob->name);
    addLast(job, toStrJobType(pJob->jobType));

    LinkedList *saved = getOrDefault(application->jobs, jobSeeker->name, newLinkedList());
    addLast(saved, job);

    putItem(application->jobs, jobSeeker->name, saved);
}

void publish(const Application *application, const Employer *employer, const Job *pJob) {
    LinkedList *job = newLinkedList();
    addLast(job, pJob->name);
    addLast(job, toStrJobType(pJob->jobType));

    LinkedList *alreadyPublished = getOrDefault(application->jobs, employer->name, newLinkedList());
    addLast(alreadyPublished, job);

    putItem(application->jobs, employer->name, alreadyPublished);
}
