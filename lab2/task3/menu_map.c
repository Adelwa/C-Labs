#include <stdlib.h>
#include <stdio.h>
#include <string.h>
 
struct fun_desc {
  char *name;
  char (*fun)(char);
};

char censor(char c) {
    if(c >= 'A' && c <= 'Z')
        c = c + 32;
    return c;
}

char encrypt(char c){
    if(c >= 0x41 && c <= 0x7a)
        c = c + 2;
    return c;
}

char decrypt(char c){
    if(c >= 0x41 && c <= 0x7a)
        c = c - 2;
    return c;
}

char dprt(char c){
    printf("%d\n", c);
    return c;
}

char cprt(char c){
    if(c >= 0x41 && c <= 0x7a)
        printf("%c\n", c);
    else
        printf("*\n");
    return c;
}

char my_get(char c){
    return ((char)fgetc(stdin));
}

char quit(char c){
    if(c == 'q')
        exit(0);
    return c;
}

char* map(char *array, int array_length, char (*f) (char)){
  char* mapped_array = (char*)(malloc(array_length*sizeof(char)));
  /* TODO: Complete during task 2.a */
  for(int i = 0; i < array_length; i++){
    mapped_array[i] = (*f)(array[i]);
  }
  return mapped_array;
}

int main(int argc, char **argv){
  /* TODO: Test your code */
    struct fun_desc menu[] = { { "Censor", censor }, { "Encrypt", encrypt }, { "Decrypt", decrypt }, { "Print dec", dprt}, { "Print string", cprt  }, { "Get string", my_get }, { "Quit", quit }, { NULL, NULL } };
    int length = sizeof(menu)/sizeof(struct fun_desc);
    char* carray =(char*)malloc(5*sizeof(char));
    char buffer[256];
    int number = 0;
    int cLength = 5;
    while(1){
        printf("Please choose a function:\n");
        for(int i = 0; menu[i].name != NULL; i++)
            printf("%d) %s\n", i, menu[i].name);
        printf("Option: ");
        fgets(buffer, sizeof(buffer), stdin);
        sscanf(buffer, "%d", &number);
        if(number >= (length - 1) || number < 0){
            printf("Not within bounds\n");
            free(carray);
            exit(1);
        }
        else
            printf("within bounds\n");
        // printf("%d", length);
        carray = map(carray, cLength, menu[number].fun);
        printf("Done.\n\n");
    }
}