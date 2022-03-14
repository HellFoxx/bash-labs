#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <limits.h>


long getDirSize(const char * dir_name) {
    
    DIR *d = opendir(dir_name);
    if (!d) {
        puts("Error! No such file");
        return -1;
    }
    long dirSum = 0;

    while (1) {

        struct dirent *entry;
        const char *d_name;
        entry = readdir(d);
        if (!entry) break;
        d_name = entry->d_name;
        char path[4096];
        strcpy(path, dir_name);
        strcat(path, "/");
        strcat(path, d_name);

        if (entry->d_type == DT_DIR) {
            if (strcmp(d_name, "..") != 0 && strcmp(d_name, ".") != 0) {
                dirSum += getDirSize(path);
            }
	    }
        else {
            struct stat st;
            if (!stat(path, &st)) {
                //printf("%s - %ldB\n", path, st.st_size);
                dirSum += st.st_size;
            }
        }
    }

    if (closedir(d)) {
        puts("Error! Directory cannot be closed\n");
        return -1;
    }

    return dirSum;
}

int main (int argc, char *argv[])
{
    long dirSum = getDirSize(argv[1]);
    printf("\n\n Dir sum = %ldB \n\n", dirSum);
    long sysSum = getDirSize("/");
    printf("\n\n Sys sum = %ldB \n\n", sysSum);
    printf("Precent - %f% \n", (dirSum * 1.0 / sysSum) * 100);
    return 0;
}