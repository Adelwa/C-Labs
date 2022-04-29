#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define sig_type_size 2
#define nameLength 16
#define decoration 20
#define detectSize 10000
#define bufferSize 256
#define tmp 100

typedef struct virus {
    unsigned short SigSize;
    unsigned char* sig;
    char virusName[16];
} virus;


void readVirus(virus* vir, FILE* input){
    int result = 0;
    result = fread(&vir->SigSize, sig_type_size, 1, input);
    if(result == 1){
        vir->sig = (unsigned char*)malloc(vir->SigSize);
        result = fread(vir->sig, vir->SigSize, 1, input);
        if(result != 1){
            fclose(input);
            exit(1);
        }    
        char name[nameLength];
        result = fread(name, nameLength, 1, input);
        if(result != 1){
            fclose(input);
            exit(1);
        } 
        strcpy(vir->virusName, name);
    }
    else
        vir->SigSize = 0;    
}


void printVirus(virus* virus, FILE* output){
    if(virus->SigSize != 0){
        fprintf(output,"Virus name:  ");
        fprintf(output, "%s\n", virus->virusName);
        fprintf(output,"Virus size:  ");
        fprintf(output, "%d\n", virus->SigSize);
        fprintf(output, "signature:  \n");
    }
    int i = 0;
    while(i < virus->SigSize){
        if(virus->SigSize != 0){
            fprintf(output, "%.2X ", virus->sig[i]);
            if(i % decoration == 0 && i != 0){
                fprintf(output, "\n");
            }
        }
        i = i + 1;
    }
    fprintf(output, "\n");
}



typedef struct link link;
 
struct link {
    link *nextVirus;
    virus *vir;
};

void list_print(link *virus_list, FILE* output){
    if(virus_list->nextVirus == NULL){
        printVirus(virus_list->vir, output);
        fprintf(output, "\n");
    }
    else{
        while(virus_list->nextVirus != NULL){
            printVirus(virus_list->vir, output);
            fprintf(output, "\n");
            virus_list = virus_list->nextVirus;
        }
        printVirus(virus_list->vir, output);
        fprintf(output, "\n");
    }
}




link* list_append(link* virus_list, link* to_add){
    if(virus_list == NULL)
        return to_add;
    link* next = virus_list;
    while(next->nextVirus != NULL)
        next = next->nextVirus;
    next->nextVirus = to_add;
    return virus_list;
}


void list_free(link *virus_list){
    if(virus_list->nextVirus != NULL){
        list_free(virus_list->nextVirus);
    }
    free(virus_list->vir->sig);
    free(virus_list->vir);
    free(virus_list);
}

void detect_virus(char *buffer, unsigned int size, link* virus_list){
    int result = 0;
    link* viruses_link = virus_list;
    int viruses = 0;
    int printed = 0; //(false)
    while(virus_list != NULL && viruses_link != NULL){
        if(viruses_link != NULL && viruses_link->vir->SigSize != 0){
            for(int i = 0; i < size - viruses_link->vir->SigSize; i++){
                result = memcmp(viruses_link->vir->sig, buffer+i, viruses_link->vir->SigSize);
                if(result == 0){
                    viruses++;
                }
            }
            viruses_link = viruses_link->nextVirus;
        }
        else{
            if(printed == 0){
                fprintf(stdout, "\nNumber of viruses detected is: %d\n\n", viruses);
                printed = 1;
            }
            if(virus_list->vir->SigSize != 0){
                for(int i = 0; i < size - virus_list->vir->SigSize; i++){
                    result = memcmp(virus_list->vir->sig, buffer+i, virus_list->vir->SigSize);
                    if(result == 0){
                        fprintf(stdout, "The starting byte location in the suspected file: %d\n\n", i);
                        fprintf(stdout, "The virus name: %s\n\n", virus_list->vir->virusName);
                        fprintf(stdout, "The size of the virus signature: %d\n\n", virus_list->vir->SigSize);
                        fprintf(stdout, "---------------------------------------------\n");
                        break;
                    }
                }
                virus_list = virus_list->nextVirus;
            }
            else
                break;
        }
    }
}

void kill_virus(char *fileName, int signatureOffset, int signatureSize){
    FILE* file = fopen(fileName, "r+");
    fseek(file, signatureOffset, SEEK_SET);
    char nop = 0x90;
    for(int i = 0; i < signatureSize; i++)
        fwrite(&nop, 1, 1, file);
    fclose(file);
}

int main(int argc, char** argv){
    FILE* input;
    char buffer[bufferSize];
    char name[tmp];
    int loaded = 0; //(false)
    int detected = 0;//(false)
    link* virusLinkedList = NULL;
    while(1){
        printf("Please pick an option:\n1)Load signatures\n2)Print signatures\n3)Detect viruses\n4)Fix file\n5)Quit\nYour choice is: ");
        int option = 0;
        fgets(buffer, sizeof(buffer), stdin);
        sscanf(buffer, "%d", &option);
        if(option == 1){
            printf("\nYour choice is: Load signatures\n");
            printf("\nPlease Enter input file name: \n");
            fgets(buffer, sizeof(buffer), stdin);
            sscanf(buffer, "%s", name);
            input = fopen(name, "r+");
            if(input == NULL){
                printf("\nFile can't be opened for reading\n");
                exit(1);
            }
            loaded = 1;
            while(!feof(input)){
                virus* vir = (virus*)malloc(sizeof(virus));
                readVirus(vir, input);
                link* to_add = (link*)malloc(sizeof(link));
                to_add->vir = vir;
                to_add->nextVirus = NULL;
                virusLinkedList = list_append(virusLinkedList, to_add);
            }
            printf("\nDone.\n\n");
        }
        else if(option == 2){
            printf("\nYour choice is: Print signatures\n");
            if(loaded != 1)
                printf("\nFirst you have to Load signature\n");
            else{
                list_print(virusLinkedList, stdout);
                printf("\nDone.\n\n");
            }
        }
        else if(option == 3){
            printf("\nYour choice is: Detect viruses\n");
            if(loaded != 1)
                printf("\nFirst you have to Load signature\n");
            else{
                printf("\nPlease enter input file name: \n");
                fgets(buffer, sizeof(buffer), stdin);
                sscanf(buffer, "%s", name);
                FILE* scan = fopen(name, "r+");
                if(scan == NULL){
                    printf("\nFile can't be opened for reading\n");
                    exit(1);
                }
                char* detect = (char*)malloc(detectSize*sizeof(char));
                fread(detect, detectSize, 1, scan);
                int size = detectSize;
                for(int i = 1; i <= detectSize; i++)
                    if(detect[detectSize - i]  == '\0')
                        size-=1;
                    else
                        break;
                detect_virus(detect, size, virusLinkedList);
                detected = 1;
                free(detect);
                fclose(scan);
                printf("\nDone.\n\n");
            }
        }
        else if(option == 4){
            if(detected == 1){
                int signatureOffset = 0;
                int signatureSize = 0;
                printf("Please enter signatureOffset\n");
                fgets(buffer, sizeof(buffer), stdin);
                sscanf(buffer, "%d", &signatureOffset);
                printf("Please enter signatureSize\n");
                fgets(buffer, sizeof(buffer), stdin);
                sscanf(buffer, "%d", &signatureSize);
                kill_virus(name, signatureOffset, signatureSize);
            }
            else
                printf("\nFirst you have to Detect File\n");
        }
        else if(option == 5){
            printf("\nYour choice is: Quit\n");
            printf("\nQuiting...\n");
            if(loaded == 1){
                list_free(virusLinkedList);
                fclose(input);
                loaded = 0;
            }
            printf("\nDone.\n\n");
            break;
        }
        else
            printf("\nWrong number, try again\n\n");
    }
    return 1;
}
