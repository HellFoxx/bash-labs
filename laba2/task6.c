#include <dirent.h>
#include <stdio.h>

int main(int argc, char *argv[]) {

    DIR *d;
    struct dirent *dir;
    d = opendir(argv[1]);
    if (!d) {
        fputs("Error! No such directory\n", stderr);
        return 1;
    }

    printf("\n ----- %s ----- \n", argv[1]);
    while ((dir = readdir(d)) != NULL)
        puts(dir->d_name);
    closedir(d);

    puts("\n ----- root ----- \n");
    d = opendir("/");
    while ((dir = readdir(d)) != NULL)
        puts(dir->d_name);
    closedir(d);

    return 0;

}