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
    LinkedList *alreadyPublished = getOrDefault(jobs->jobs, employer->name, newLinkedList());
    addLast(alreadyPublished, pJob);

    putItem(jobs->jobs, employer->name, alreadyPublished);
}

LinkedList *getPublishedJobs(Jobs *jobs, Employer *employer) {
    return getItemBy(jobs->jobs, employer->name);
}

void saveJob(Jobs *jobs, Job *pJob, JobSeeker *jobSeeker) {
    LinkedList *saved = getOrDefault(jobs->jobs, jobSeeker->name, newLinkedList());
    addLast(saved, pJob);

    putItem(jobs->jobs, jobSeeker->name, saved);
}
