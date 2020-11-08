#include "employer.h"
#include "stdio.h"
#include "stdlib.h"

Employer *newEmployer(char *name) {
    Employer *pEmployer = (Employer *) malloc(sizeof(Employer));
    if (pEmployer == NULL) {
        printf("内存分配失败");
        return pEmployer;
    }

    pEmployer->name = name;
    return pEmployer;
}
