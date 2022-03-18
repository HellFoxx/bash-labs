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
        //puts("No such directory");
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

        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, ".") != 0) {
                // print dirs
                // puts(path);
                dirSum += getDirSize(path);
            }
	    }
        else {
            struct stat st;
            if (!stat(path, &st)) {
                // print files
                // printf("%s - %ldB\n", path, st.st_size);
                dirSum += st.st_size;
            }
        }
    }

    if (closedir(d)) {
        puts("Error! Directory cannot be closed\n");
        return 0;
    }

    return dirSum;
}

int main (int argc, char *argv[])
{
    long dirSum = getDirSize(argv[1]);
    printf("\n Dir sum = %ldB \n", dirSum);
    long sysSum = getDirSize("/");
    printf("\n Sys sum = %ldB \n", sysSum);
    printf("\n Precent - %f\% \n", (dirSum * 1.0 / sysSum) * 100);
    return 0;
}