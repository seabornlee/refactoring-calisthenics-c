#include <time.h>
#include <string.h>
#include "applied_jobs.h"
#include "stdio.h"
#include "stdlib.h"

int matchJobName(const Job *pJob, const LinkedList *job);

int matchJobNameAndDate(const Job *pJob, const char *from, const char *to, const LinkedList *jobs);


AppliedJobs *newAppliedJobs() {
    AppliedJobs *pJobs = (AppliedJobs *) malloc(sizeof(AppliedJobs));
    if (pJobs == NULL) {
        printf("内存分配失败");
        return pJobs;
    }

    pJobs->applied = newLinkedMap();
    return pJobs;
}

void
applyJob(AppliedJobs *appliedJobs, Employer *employer, Job *pJob, JobSeeker *jobSeeker, const char *applicationTime) {
    LinkedList *job = newLinkedList();
    addLast(job, pJob->name);
    addLast(job, toStrJobType(pJob->jobType));

    addLast(job, applicationTime);
    addLast(job, employer->name);

    LinkedList *saved = getOrDefault(appliedJobs->applied, jobSeeker->name, newLinkedList());
    addLast(saved, job);
    putItem(appliedJobs->applied, jobSeeker->name, saved);
}

void filterApplicantsAndJobs(AppliedJobs *appliedJobs, const char *date, const LinkedList *applicants,
                             const LinkedList *jobFiltered) {
    LinkedMap *pMap = appliedJobs->applied;
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

char *exportCSV(AppliedJobs *appliedJobs, char *date) {
    LinkedList *applicants = newLinkedList();
    LinkedList *jobFiltered = newLinkedList();

    filterApplicantsAndJobs(appliedJobs, date, applicants, jobFiltered);

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

char *exportHTML(AppliedJobs *appliedJobs, char *date) {
    LinkedList *applicants = newLinkedList();
    LinkedList *jobFiltered = newLinkedList();

    filterApplicantsAndJobs(appliedJobs, date, applicants, jobFiltered);

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

LinkedList *
findApplicantsIn(AppliedJobs *appliedJobs, Job *pJob, char *from, char *to) {
    LinkedList *applicants = newLinkedList();

    LinkedList *keys = keysOf(appliedJobs->applied);
    for (int i = 0; i < len(keys); ++i) {
        char *applicant = getItem(keys, i);
        LinkedList *jobs = getItemBy(appliedJobs->applied, applicant);

        if (matchJobNameAndDate(pJob, from, to, jobs)) {
            addLast(applicants, applicant);
        }
    }
    return applicants;
}

LinkedList *getJobsByJobSeeker(AppliedJobs *appliedJobs, JobSeeker *jobSeeker) {
    return getItemBy(appliedJobs->applied, jobSeeker->name);
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
