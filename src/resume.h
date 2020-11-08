#ifndef REFACTOR_CALISTHENICS_C_RESUME_H
#define REFACTOR_CALISTHENICS_C_RESUME_H

typedef struct resume {
    char *applicantName;
} Resume;

Resume *newResume(char *applicantName);
#endif //REFACTOR_CALISTHENICS_C_RESUME_H
