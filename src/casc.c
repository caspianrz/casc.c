#include <stdio.h>
#include <stdlib.h>

#include <casc.c/keywords.h>

int casc_c_main(int argc, char *argv[])
{
    fprintf(stderr, "casc.c: is not implemented yet.\n");
    exit(EXIT_FAILURE);

    return EXIT_FAILURE;
}

int main(int argc, char *argv[])
{
    if (in_word_set("chra", 4))
    {
        printf("Found `chra` in keywords.\n");
    }
    else
    {
        printf("Did not found `chra` in keywords.\n");
    }
    return casc_c_main(argc, argv);
}