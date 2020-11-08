#ifndef REFACTOR_CALISTHENICS_C_JOB_TYPE_H
#define REFACTOR_CALISTHENICS_C_JOB_TYPE_H

enum JobType {
    ATS, JReq
};

char *toStrJobType(enum JobType jobType);
#endif //REFACTOR_CALISTHENICS_C_JOB_TYPE_H
