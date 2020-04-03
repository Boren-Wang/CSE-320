#include <stdio.h>
#include "sfmm.h"
#include "myheader.h"

int main(int argc, char const *argv[]) {
    sf_mem_init();

    double* ptr = sf_malloc(sizeof(double));

    *ptr = 320320320e-320;

    printf("%f\n", *ptr);

    sf_free(ptr);

    sf_mem_fini();

    return EXIT_SUCCESS;
}

