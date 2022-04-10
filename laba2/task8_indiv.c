#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <limits.h>
#include <sys/statvfs.h>

FILE *f;

long getDirSize(const char *dir_name, FILE *f) {
    
    DIR *d = opendir(dir_name);
    if (!d) {
        //fputs("No such directory", stderr);
        return 0;
    }
    long dirSum = 0;
    struct dirent *entry;

    while (1) {

        entry = readdir(d);
        if (!entry)
            break;
    
        char path[4096];
        strcpy(path, dir_name);
        strcat(path, "/");
        strcat(path, entry->d_name);

        if (entry->d_type == DT_DIR)
            if (strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, ".") != 0)
                dirSum += getDirSize(path, f);
            else
                continue;

        struct stat st;
        if (stat(path, &st) == 0) {
            // print files
            // printf("%s - %ldB\n", path, st.st_size);
            fprintf(f, "%s - %ldB\n", path, st.st_size);
            dirSum += st.st_size;
        }

        

    }

    if (closedir(d)) {
        fputs("Error! Directory cannot be closed\n", stderr);
        return 0;
    }

    return dirSum;
}

int main (int argc, char *argv[]) {

    if (argc != 2) {
        fputs("Wrong number of params!", stderr);
        return 1;
    }

    struct statvfs fiData;
    struct statvfs *fpData;
    char fnPath[128];

    f = fopen("test1.txt", "w");
    long dirSum = getDirSize(argv[1], f);
    fclose(f);
    printf("\n Dir sum = %ldB \n", dirSum);

    // f = fopen("test2.txt", "w");
    // long sysSum = getDirSize("/", f);
    // fclose(f);

    // printf("\n Sys sum = %ldB \n", sysSum);
    // printf("\n Precent - %lf\% \n", (dirSum * 1.0 / sysSum) * 100);

    strcpy(fnPath, argv[1]);
    long total = 0;
    if((statvfs(fnPath,&fiData)) < 0 ) {
            printf("Failed to stat %s:\n", fnPath);
    } else {
        total = fiData.f_blocks * fiData.f_bsize;
        printf("total - %dB\n", total);
    }

    printf("\n Precent - %lf\% \n", (dirSum * 1.0 / total) * 100);

    return 0;
}