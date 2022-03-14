#include <stdio.h>

int main(int argc[], char *argv[]) {

    if (argv[1] == NULL) {
        printf("Error! No params\n");
        return 1;   
    }

    FILE *file = fopen(argv[1], "w");
    if (file == NULL) {
        printf("Error! File not found\n");
        return 1;
    }

    while (1) {
        char symbol = getc(stdin);
        if (symbol == '\e')
            break;
        fputc(symbol, file);
    }

    fclose(file);

    return 0;

}