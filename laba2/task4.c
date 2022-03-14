#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Error! Not enough params\n");
        return 1;   
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Error! File not found\n");
        return 1;
    }

    int N = atoi(argv[2]);
    if (N < 0) {
        printf("Error! Wrong second param");
        return 1;
    }

    if (N == 0) {
        while (!feof(file))
            putc(fgetc(file), stdout); 
    } else {
        int k = 0;
        while (N != k) {
            char symbol = fgetc(file);
            if (symbol == '\n') k++;
            putc(symbol, stdout);
        }
        
    }

    fclose(file);

    return 0;

}