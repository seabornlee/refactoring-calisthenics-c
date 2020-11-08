#include "jobs.h"
#include "stdlib.h"
#include "stdio.h"

Jobs *newJobs() {
    Jobs *pJobs = (Jobs *) malloc(sizeof(Jobs));
    if (pJobs == NULL) {
        printf("内存分配失败");
        return pJobs;
    }

    pJobs->jobs = newLinkedMap();
    return pJobs;
}

void publishJob(Jobs *jobs, Employer *employer, Job *pJob) {
    LinkedList *job = newLinkedList();
    addLast(job, pJob->name);
    addLast(job, toStrJobType(pJob->jobType));

    LinkedList *alreadyPublished = getOrDefault(jobs->jobs, employer->name, newLinkedList());
    addLast(alreadyPublished, job);

    putItem(jobs->jobs, employer->name, alreadyPublished);
}

LinkedList *getPublishedJobs(Jobs *jobs, Employer *employer) {
    return getItemBy(jobs->jobs, employer->name);
}

void saveJob(Jobs *jobs, Job *pJob, JobSeeker *jobSeeker) {
    LinkedList *job = newLinkedList();
    addLast(job, pJob->name);
    addLast(job, toStrJobType(pJob->jobType));

    LinkedList *saved = getOrDefault(jobs->jobs, jobSeeker->name, newLinkedList());
    addLast(saved, job);

    putItem(jobs->jobs, jobSeeker->name, saved);
}
