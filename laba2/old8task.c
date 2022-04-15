#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <limits.h>
#include <sys/statvfs.h>
#include <math.h>

FILE *f;
unsigned long dirSum = 0;
unsigned long total = 0;

int getDirSize(const char *dir_name, FILE *f) {
    
    DIR *d = opendir(dir_name);
    if (!d) return 1;

    struct dirent *entry;
    while (1) {

        entry = readdir(d);
        if (!entry) break;
    
        char path[4096];
        strcpy(path, dir_name);
        if (dir_name[strlen(dir_name)-1] != '/')
            strcat(path, "/");
        strcat(path, entry->d_name);

        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".") == 0)
                continue;
            getDirSize(path, f);
        } else if (entry->d_type == DT_REG) {
            struct stat st;
            if (stat(path, &st) == 0) {
                if (st.st_size > total)
                    continue;
                dirSum += st.st_size;
                fprintf(f, "%ldB\t%ldB\t\t%d\t\t%s\n",st.st_size, dirSum, entry->d_type, path);
                //printf("%ldB\t%ld\t\t%s\n",st.st_size, dirSum, path);
            }   
        }

    }

    if (closedir(d)) {
        fputs("Error! Directory cannot be closed\n", stderr);
        return 1;
    }

    return 0;
}

int main (int argc, char *argv[]) {

    if (argc != 2) {
        fputs("Wrong number of params!\n", stderr);
        return 1;
    }

    struct statvfs fiData;
    struct statvfs *fpData;
    char fnPath[128];
    strcpy(fnPath, argv[1]);
    if((statvfs(fnPath,&fiData)) < 0 ) {
            printf("Failed to stat %s:\n", fnPath);
    } else {
        total = fiData.f_blocks * fiData.f_bsize;
        printf("Total\t\t%ldB\n", total);
    }

    f = fopen("test.txt", "w");
    if (getDirSize(argv[1], f) == 1) {
        fprintf(stderr, "Error while getting directory size!\n");
        return 1;
    }
    fclose(f);

    printf("%s\t\t%ldB\n", argv[1], dirSum);

    printf("Precentage\t%.0f\% \n", (dirSum * 1.0 / total) * 100);

    return 0;
}