#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <direct.h>

typedef struct Map{
    char key[256];
    char value[2048];    
}Map;

int argumentParser(char **args);
void printKeyValuePair(Map map);
void printKeyValuePairs(Map map[]);
void writeCreateFile(char *text);


//DEFINITION LIST:
#define TYPE "e"
#define INIT "i"
#define NAME "n"
#define DEL "del"
#define ARGUMENT_COUNT 4
#define MAX_PATH_LENGTH 1024
//
int state = 0; //0 -> Name, 1-> extention, 2-> initialization
int realizedArgumentCount = 0;
int argumentCounter = 0;
Map argumentMap [ARGUMENT_COUNT];
char argumentTokens[ARGUMENT_COUNT][256];
int argsPos [ARGUMENT_COUNT];
//FILE INFO:
char extention[8];
char name[248];
char fullName[256];
//




int main (int argc, char **args){
    argumentCounter = argc;//Inititalize global variable
    realizedArgumentCount = (argc-1)/2;                                    //double res = ceil(((double)argc-1)/2); //Actually realized argument amount arithmetic
                                                                          //realizedArgumentCount = (int) res;
    if(realizedArgumentCount>ARGUMENT_COUNT){
        printf("TOO MANY ARGUMENTS ENTERED !");
        return -1;
    }
    
    printf("argc: %d\n", argc);
    
    argumentParser(args); // Parse Commands into a poor man's Hashmap (argumentMap)

   

    strcpy(name, "tmp");
    strcpy(extention, ".txt");
    char initValue[2048]; 
    int shouldInit = 0;       // A simple flag to check if INIT was called
    
    int i = 0;
    for(i = 0; i < realizedArgumentCount; i++) {
        
        if (argumentMap[i].key[0] == '\0') continue; 

        if(strcmp(NAME, argumentMap[i].key) == 0) {
            if(strcmp(argumentMap[i].value, "") != 0) {
                strcpy(name, argumentMap[i].value);
            }
            printf("Parsed NAME: %s\n", name);
        }
        
        else if(strcmp(TYPE, argumentMap[i].key) == 0) {
            if(strcmp(argumentMap[i].value, "") != 0) {
                strcpy(extention, argumentMap[i].value);
            }
            printf("Parsed TYPE: %s\n", extention);
        }
        
        else if(strcmp(INIT, argumentMap[i].key) == 0) {
            strcpy(initValue, argumentMap[i].value);
            shouldInit = 1; 
            printf("Parsed INIT flag.\n");
        }
        
        else {
            printf("Warning: Unknown argument key '%s' ignored.\n", argumentMap[i].key);
        }
    }


    printf("\n--- Final Configuration ---\n");
    printf("Name: %s\n", name);
    printf("Extension: %s\n", extention);


    if (shouldInit) {
        writeCreateFile(initValue);
    } else {
        writeCreateFile(""); 
    }
        printf("fn: %s, name -> %s, ext -> %s\n",fullName, name, extention);
    return 0;
}



int argumentParser(char **args){
    int argsPos[realizedArgumentCount];
    if(args == NULL){
        printf("The arguments are NULL !");
        return -1;
    }
    int i = 1;

    int iTracker = 0; //Itiration Tracker for argument position placement
    while(i<argumentCounter){
        if(strchr(*(args+i),'-') != NULL){ //Check for token having a '-' in it or not.
            printf("Argument: %d = ",i);
            char * tokens = strtok(*(args+i), "-");
            while(tokens != NULL){
                printf("tokens: '%s' iTracker: %d\n",tokens,iTracker);
                strncpy(argumentMap[iTracker].key,tokens,255);
                tokens = strtok(NULL, "-");
            }
            argsPos[iTracker] = i+1;
            printf("argumentMap: %s\n",argumentMap[iTracker].key);
            iTracker++;
        }   
        i++;
    }
    int k = 0;
    for(k=0;k<ARGUMENT_COUNT;k++){
        strncpy(argumentMap[k].value,*(args+argsPos[k]),256);
        printf("argumentTokens(%d): '%s'\n", k,argumentTokens[k]);
    }
    printKeyValuePairs(argumentMap);

    return 0;
}

void printKeyValuePair(Map map){
    printf("Key: '%s', Value: '%s'\n", map.key, map.value);
}

void printKeyValuePairs(Map map[]){
    int i = 0;
    for(i=0; i<ARGUMENT_COUNT; i++){
        printf("Map[%d]Key: '%s', Value: '%s'\n", i, map[i].key, map[i].value);
    }

}

void writeCreateFile(char *text){
    strcpy(fullName,name);
    strcat(fullName,extention);
    char currentDirectory[MAX_PATH_LENGTH];
    _getcwd(currentDirectory, MAX_PATH_LENGTH);
    strcat(currentDirectory,"\\");
    char fullPath[1024];
    strcpy(fullPath,strcat(currentDirectory,fullName));
    FILE *fp = fopen(fullPath,"a+");
    printf("fullPath -> %s\n",fullPath);
    if(fp == NULL){
        printf("THERE WAS AN ERROR OPENING THE FILE");
    }

    fprintf(fp, text);

    fclose(fp);

}