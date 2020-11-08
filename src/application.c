#include <string.h>
#include <stdio.h>
#include "application.h"
#include "stdlib.h"
#include "linked_list.h"
#include "linked_map.h"

int getCountByEmployerAndJobName(const Employer *employer, const Job *pJob, const LinkedList *list);

int apply(const Application *application, const Employer *employer, const Job *pJob, const JobSeeker *jobSeeker,
          const Resume *resume, const char *applicationTime);

Application *newApplication() {
    Application *pApplication = (Application *) malloc(sizeof(Application));
    if (pApplication == NULL) {
        return pApplication;
    }

    newLinkedMap();
    pApplication->failedApplications = newLinkedList();
    pApplication->jobs = newJobs();
    pApplication->appliedJobs = newAppliedJobs();
    return pApplication;
}

LinkedList *getJobs(Application *pApplication, char *name, enum JobStatus jobStatus) {
    if (jobStatus == APPLIED) {
        return getJobsByJobSeeker(pApplication->appliedJobs, newJobSeeker(name));
    }
    return getPublishedJobs(pApplication->jobs, newEmployer(name));
}

LinkedList *findApplicants(Application *pApplication, Job *job) {
    return findApplicantsFrom(pApplication, job, NULL);
}

LinkedList *findApplicantsFrom(Application *pApplication, Job *job, char *from) {
    return findApplicantsIn(pApplication->appliedJobs, job, from, NULL);
}

char *
exportTo(Application *pApplication, enum FileType type, char *date) {
    if (type == CSV) {
        return exportCSV(pApplication->appliedJobs, date);
    }
    return exportHTML(pApplication->appliedJobs, date);
}

int getSuccessfulApplications(Application *pApplication, Employer *employer, Job *pJob) {
    LinkedList *keys = keysOf(pApplication->appliedJobs->applied);

    int count = 0;
    for (int i = 0; i < len(keys); ++i) {
        char *applicant = getItem(keys, i);
        LinkedList *jobs = getItemBy(pApplication->appliedJobs->applied, applicant);
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
        publishJob(application->jobs, employer, pJob);
        return 0;
    }

    if (command == SAVE) {
        saveJob(application->jobs, pJob, jobSeeker);
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

    applyJob(application->appliedJobs, employer, pJob, jobSeeker, applicationTime);
    return 0;
}

