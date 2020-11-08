#ifndef REFACTOR_CALISTHENICS_C_EMPLOYER_H
#define REFACTOR_CALISTHENICS_C_EMPLOYER_H

typedef struct employer {
    char *name;
} Employer;

Employer *newEmployer(char *name);

#endif //REFACTOR_CALISTHENICS_C_EMPLOYER_H
