#include "resume.h"
#include "stdlib.h"
#include "stdio.h"

Resume *newResume(char *applicantName) {
    Resume *resume = (Resume *) malloc(sizeof(Resume));
    if (resume == NULL) {
        printf("内存分配失败");
        return resume;
    }

    resume->applicantName = applicantName;
    return resume;
}
