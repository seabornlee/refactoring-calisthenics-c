#include <time.h>
#include <string.h>
#include "applied_jobs.h"
#include "stdio.h"
#include "stdlib.h"

int matchJobName(const Job *pJob, JobApplication *jobApplication);

int matchJobNameAndDate(const Job *pJob, const char *from, const char *to, const LinkedList *pJobApplications);


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
    JobApplication *pJobApplication = newJobApplication(pJob, employer, applicationTime);
    LinkedList *saved = getOrDefault(appliedJobs->applied, jobSeeker->name, newLinkedList());
    addLast(saved, pJobApplication);
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
            JobApplication *pJobApplication = getItem(jobs, j);
            struct tm appliedAt = {0};
            strptime(pJobApplication->applicationTime, "%Y-%m-%d", &appliedAt);

            struct tm tmDate = {0};
            strptime(date, "%Y-%m-%d", &tmDate);

            if (mktime(&appliedAt) == mktime(&tmDate)) {
                addLast(applicants, applicant);
                addLast(jobFiltered, pJobApplication);
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
        JobApplication *pJobApplication = getItem(jobFiltered, i);
        char str[200];
        sprintf(str, "%s,%s,%s,%s,%s\n", pJobApplication->employer->name, pJobApplication->job->name, toStrJobType(pJobApplication->job->jobType), getItem(applicants, i),
                pJobApplication->applicationTime);
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
        JobApplication *pJobApplication = getItem(jobFiltered, i);
        char str[200];
        sprintf(str, "<tr><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td></tr>", pJobApplication->employer->name,
                pJobApplication->job->name, toStrJobType(pJobApplication->job->jobType), getItem(applicants, i), pJobApplication->applicationTime);
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
        LinkedList *pJobApplications = getItemBy(appliedJobs->applied, applicant);

        if (matchJobNameAndDate(pJob, from, to, pJobApplications)) {
            addLast(applicants, applicant);
        }
    }
    return applicants;
}

LinkedList *getJobsByJobSeeker(AppliedJobs *appliedJobs, JobSeeker *jobSeeker) {
    return getItemBy(appliedJobs->applied, jobSeeker->name);
}

int matchJobNameAndDate(const Job *pJob, const char *from, const char *to, const LinkedList *pJobApplications) {
    for (int j = 0; j < len(pJobApplications); ++j) {
        JobApplication *pJobApplication = getItem(pJobApplications, j);
        struct tm appliedAt = {0};
        strptime(pJobApplication->applicationTime, "%Y-%m-%d", &appliedAt);

        struct tm tmFrom = {0};
        if (from != NULL) {
            strptime(from, "%Y-%m-%d", &tmFrom);
        }

        struct tm tmTo = {0};
        if (to != NULL) {
            strptime(to, "%Y-%m-%d", &tmTo);
        }

        if ((pJob == NULL || matchJobName(pJob, pJobApplication))
            && (from == NULL || mktime(&appliedAt) >= mktime(&tmFrom))
            && (to == NULL || mktime(&appliedAt) <= mktime(&tmTo))) {
            return 1;
        }
    }
    return 0;
}

int matchJobName(const Job *pJob, JobApplication *jobApplication) {
    char *_jobName = jobApplication->job->name;
    return strcmp(pJob->name, _jobName) == 0;
}
