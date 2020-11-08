#include "job_type.h"

char *toStrJobType(enum JobType jobType) {
    return jobType == ATS ? "ATS" : "JReq";
}
