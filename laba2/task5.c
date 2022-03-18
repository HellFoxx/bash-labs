#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {

    if (argc != 3) {
        fputs("Error! Wrong number of params\n", stderr);
        return 1;   
    }

    FILE *fileFrom = fopen(argv[1], "r");
    if (fileFrom == NULL) {
        fputs("Error! File1 not found\n", stderr);
        return 1;
    }

    FILE *fileTo = fopen(argv[2], "w");
    if (fileTo == NULL) {
        fputs("Error! File2 not found\n", stderr);
        return 1;
    }

    while (!feof(fileFrom))
        fputc(fgetc(fileFrom), fileTo);

    struct stat st;
    stat(argv[1], &st);
    chmod(argv[2], st.st_mode);

    fclose(fileFrom);
    fclose(fileTo);

    return 0;

}