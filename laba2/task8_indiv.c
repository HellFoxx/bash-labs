#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <limits.h>
#include <sys/statvfs.h>

static unsigned long dirSum = 0;

int sum(const char *fpath, const struct stat *sb, int typeflag) {
    dirSum += sb->st_size;
    return 0;
}

int getDirSize(const char *dir_name) {
    
    DIR *d = opendir(dir_name);
    if (!d) {
        fputs("No such directory", stderr);
        return 1;
    }

    struct dirent *entry = readdir(d);
    while (entry) {
    
        char path[4096];
        strcpy(path, dir_name);
        if (dir_name[strlen(dir_name)-1] != '/')
            strcat(path, "/");
        strcat(path, entry->d_name);

        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".") == 0) {
                entry = readdir(d);
                continue;
            }
            if (ftw(path, &sum, 1)) {
                fprintf(stderr, "FTW Error with: %s\n", path);
                //return 1;
            }
        } else {
            struct stat st;
            if (stat(path, &st) == 0) {
                // print files
                // printf("%ldB\t\t%s\n", st.st_size, path);
                dirSum += st.st_size;
            }
            else
                fprintf(stderr, "STAT Error!");   
        }
        
        printf("%s\n", path);
        entry = readdir(d);

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

    if (getDirSize(argv[1]) == 1) {
        fprintf(stderr, "Error while getting directory size\n");
        return 1;
    }
    printf(" Dir sum: %s - %dB\n", argv[1], dirSum);

    struct statvfs fiData;
    struct statvfs *fpData;
    char fnPath[128];

    strcpy(fnPath, argv[1]);
    long total = 0;
    if((statvfs(fnPath,&fiData)) < 0 ) {
        printf("Failed to stat %s:\n", fnPath);
    } else {
        total = fiData.f_blocks * fiData.f_bsize;
        printf(" Total - %dB\n", total);
    }

    printf("\n Precent - %lf\% \n", (dirSum * 1.0 / total) * 100);

    return 0;
}