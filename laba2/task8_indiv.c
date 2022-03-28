#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <limits.h>


long getDirSize(const char *dir_name) {
    
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

        struct stat st;
        if (!stat(path, &st)) {
            // print files
            // printf("%s - %ldB\n", path, st.st_size);
            dirSum += st.st_size;
        }

        if (entry->d_type == DT_DIR)
            if (strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, ".") != 0)
                dirSum += getDirSize(path);
            
    }

    if (closedir(d)) {
        fputs("Error! Directory cannot be closed\n", stderr);
        return 0;
    }

    return dirSum;
}

int main (int argc, char *argv[])
{

    if (argc != 2) {
        fputs("Wrong number of params!", stderr);
        return 1;
    }

    long dirSum = getDirSize(argv[1]);
    printf("\n Dir sum = %ldB \n", dirSum);
    long sysSum = getDirSize("/");
    printf("\n Sys sum = %ldB \n", sysSum);
    printf("\n Precent - %lf\% \n", (dirSum * 1.0 / sysSum) * 100);
    return 0;
}