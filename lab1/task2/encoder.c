#include<stdio.h>
#include<string.h>
int adder(int debbuger, int number, char* inputFileName, char* outputFileName){
    int c;
    FILE* in;
    if(strcmp(inputFileName, "") == 0)
        in = stdin;
    else{
        in = fopen(inputFileName, "r");
        if(in == NULL){
            perror("File can't be opened for reading\n");
            return -1;
        }
    }
    FILE* out;
    if(strcmp(outputFileName, "") == 0)
        out = stdout;
    else{
        out = fopen(outputFileName, "w");
        if(out == NULL){
            perror("File can't be opened for reading\n");
            return -1;
        }
    }
    FILE* err = stderr;
    int counter = 0;
    int firstLetter = 0;
    int firstLoop = 0;
    int previousValue = 0;
    while((c=fgetc(in)) != EOF){
            if(firstLoop == 0){
                firstLoop = 1;
                firstLetter = c;
            }
            previousValue = c;
            if(c >= 'A' && c <= 'Z'){
                counter++;
            }
            if(debbuger == 1)
                fprintf(err, "%d        %d\n", previousValue, c);
            if(c == 10){
                for(int i = 0; i < number; i++){
                    previousValue = firstLetter;
                    if(debbuger == 1 && firstLetter >= 65 && firstLetter <= 90 ){
                        counter++;
                    }
                    if(debbuger == 1)
                        fprintf(err, "%d        %d\n", previousValue, firstLetter);
                    fputc(firstLetter, out);
                }
                if(debbuger == 1){
                    fprintf(err, "the number of letters: %d\n", counter);
                }  
                printf("\n");
            }
            else
                fputc(c, out);
    }
    if(strcmp(inputFileName, "") != 0)
        fclose(in);
    if(strcmp(outputFileName, "") != 0)
        fclose(out);
    return 0;
}

int substraction(int debbuger, int number, char* inputFileName, char* outputFileName){
    int c;
    FILE* in;
    if(strcmp(inputFileName, "") == 0)
        in = stdin;
    else{
        in = fopen(inputFileName, "r");
        if(in == NULL){
            perror("File can't be opened for reading\n");
            return -1;
        }
    }
    FILE* out;
    if(strcmp(outputFileName, "") == 0)
        out = stdout;
    else{
        out = fopen(outputFileName, "w");
        if(out == NULL){
            perror("File can't be opened for reading\n");
            return -1;
        }
    }
    FILE* err = stderr;
    int counter = 0;
    int previousValue = 0;
    while((c=fgetc(in)) != EOF){
        previousValue = c;
        if(number <= 0){
            if(c >= 'A' && c <= 'Z'){
                counter++;
            }
            if(debbuger == 1)
                fprintf(err, "%d        %d\n", previousValue, c);
            if(c != 10){
                fputc(c, out);
            }
            else{
                if(debbuger == 1){
                    fprintf(err, "the number of letters: %d\n", counter);
                }
                printf("\n"); 
            }
        }
        if(c == 10 && number > 0)
            printf("-END-\n");
        number--;
    }
    if(strcmp(inputFileName, "") != 0)
        fclose(in);
    if(strcmp(outputFileName, "") != 0)
        fclose(out);
    return 0;
}

int task1a(char* inputFileName, char* outputFileName){
    int c;
    FILE* in;
    if(strcmp(inputFileName, "") == 0)
        in = stdin;
    else{
        in = fopen(inputFileName, "r");
        if(in == NULL){
            perror("File can't be opened for reading\n");
            return -1;
        }
    }
    FILE* out;
    if(strcmp(outputFileName, "") == 0)
        out = stdout;
    else{
        out = fopen(outputFileName, "w");
        if(out == NULL){
            perror("File can't be opened for reading\n");
            return -1;
        }
    }
    while((c=fgetc(in)) != EOF){
        if(c >= 'A' && c <= 'Z')
            c = '.';
        fputc(c, out);
    }
    printf("\n");
    if(strcmp(inputFileName, "") != 0)
        fclose(in);
    if(strcmp(outputFileName, "") != 0)
        fclose(out);
    return 0;
}




int main(int argc, char** argv){
    int debbuger = 0; //(false)
    int eFlag = 0;    //(false)
    int number = 0;
    int positiveSignal = 0; //(false)
    char* inputFileName = "";
    char* outputFileName = "";
    for(int i = 0; i < argc; i++){
        if(argv[i][1] == 'D')
            debbuger = 1;
        if(argv[i][1] == 'e'){
            eFlag = 1;
            if(argv[i][0] == '+')
                positiveSignal = 1;
            else if(argv[i][0] != '-'){
                printf("Invalid argument\n");
                return -1;
            }
            if(argv[i][2] >= 'A' && argv[i][0] <= 'F')
                number = argv[i][2] - 55;
            else if(argv[i][2] >= '0' && argv[i][2] <= '9')
                number = argv[i][2] - 48;
            else
                printf("Invalid argument\n");
        }
        if(argv[i][1] == 'i'){
            inputFileName = argv[i] + 2;
        }
        if(argv[i][1] == 'o' && argv[i][0] == '-'){
            outputFileName = argv[i] + 2;
        }
    }
    if(eFlag == 1 && positiveSignal == 1){
        adder(debbuger, number, inputFileName, outputFileName);
    }
    else if(eFlag == 1 && positiveSignal == 0){
        substraction(debbuger, number, inputFileName,outputFileName);
    }
    else{
        task1a(inputFileName, outputFileName);
    }
    return 0;
}