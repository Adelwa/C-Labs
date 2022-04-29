#include<stdio.h>
#include<string.h>
int adder(int debbuger, int number, char* inputFileName){
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
    FILE* out = stdout;
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
            if(c >= 65 && c <= 90){
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
    return 0;
}

int substraction(int debbuger, int number, char* inputFileName){
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
    FILE* out = stdout;
    FILE* err = stderr;
    int counter = 0;
    int previousValue = 0;
    while((c=fgetc(in)) != EOF){
        previousValue = c;
        if(number <= 0){
            if(c >= 65 && c <= 90){
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
    return 0;
}

int task1a(char* inputFileName){
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
    FILE* out = stdout;
    /*
        ASCII TABLE:
            'A' - 65
            'Z' - 90
            '.' - 46
    */
    while((c=fgetc(in)) != EOF){
        if(c >= 65 && c <= 90)
            c = 46;
        fputc(c, out);
    }
    printf("\n");
    if(strcmp(inputFileName, "") != 0)
        fclose(in);
    return 0;
}




int main(int argc, char** argv){
    int debbuger = 0; //(false)
    int eFlag = 0;    //(false)
    int number = 0;
    int positiveSignal = 0; //(false)
    char* inputFileName = "";
    /*
        ASCII TABLE:
            'A' - 65
            'Z' - 90
            '.' - 46
            '\n' - 10
            'D' - 68
            'e' - 101
            '+' - 43
            '-' - 45
            'F' - 70
            'i' - 105
    */
    for(int i = 0; i < argc; i++){
        if(argv[i][1] == 68)
            debbuger = 1;
        if(argv[i][1] == 101){
            eFlag = 1;
            if(argv[i][0] == 43)
                positiveSignal = 1;
            else if(argv[i][0] != 45){
                printf("Invalid argument\n");
                return -1;
            }
            if(argv[i][2] >= 65 && argv[i][0] <= 70)  //[A - F]
                number = argv[i][2] - 55;
            else if(argv[i][2] >= 48 && argv[i][2] <= 57)
                number = argv[i][2] - 48;
            else
                printf("Invalid argument\n");
        }
        if(argv[i][1] == 105){
            inputFileName = argv[i] + 2;
        }
    }
    printf("Start typing...\n");
    if(eFlag == 1 && positiveSignal == 1){
        adder(debbuger, number, inputFileName);
    }
    else if(eFlag == 1 && positiveSignal == 0){
        substraction(debbuger, number, inputFileName);
    }
    else{
        task1a(inputFileName);
    }
    return 0;
}