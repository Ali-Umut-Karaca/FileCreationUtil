#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <direct.h>
#include <Windows.h>

typedef struct Map{
    char key[256];
    char value[2048];    
}Map;

int openDirectory(char *path);
int debuggerS(char *debugInfo,char *dbg, int shouldPrint);
int debuggerD(int debugInfo, int shouldPrint);
int argumentParser(char **args);
void printKeyValuePair(Map map);
void printKeyValuePairs(Map map[]);
void writeCreateFile(char *text);
void printArgs(int args[]);


//DEFINITION LIST:
#define TYPE "e"
#define INIT "i"
#define NAME "n"
#define DEL "del"
#define OP "o"
#define ARGUMENT_COUNT 5
#define MAX_PATH_LENGTH 1024
//

int debugFlag = 0; // 1 is on, 0 is off
int isOpenDirectory = 0; // Flag for directory opening on 
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
    
    debuggerD(argc,debugFlag);
    
    if(argumentParser(args) == -1){ // Parse Commands into a poor man's Hashmap (argumentMap)
        printf("OPERATION TERMINATING...");
        return -1;
    }
   debuggerS("I worked after parse !","",debugFlag);

    strcpy(name, "tmp");
    strcpy(extention, ".txt");
    char initValue[2048]; 
    int shouldInit = 0;       // A simple flag to check if INIT was called
    
    int i = 0;
    for(i = 0; i < realizedArgumentCount+1; i++) {
            debuggerD(i,debugFlag);
        if (argumentMap[i].key[0] == '\0') continue; 

        if(strcmp(NAME, argumentMap[i].key) == 0) {
            if(strcmp(argumentMap[i].value, "") != 0) {
                strcpy(name, argumentMap[i].value);
            }
        //    printf("Parsed NAME: %s\n", name);
        }
        
        else if(strcmp(TYPE, argumentMap[i].key) == 0) {
            if(strcmp(argumentMap[i].value, "") != 0) {
                strcpy(extention, argumentMap[i].value);
            }
        //    printf("Parsed TYPE: %s\n", extention);
        }

        else if(strcmp(OP, argumentMap[i].key) == 0){
            debuggerS("I worked for OP","",debugFlag);
            isOpenDirectory = 1;   
        }
        
        else if(strcmp(INIT, argumentMap[i].key) == 0) {
            strcpy(initValue, argumentMap[i].value);
            shouldInit = 1; 
        //    printf("Parsed INIT flag.\n");
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
       // printf("fn: %s, name -> %s, ext -> %s\n",fullName, name, extention);
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
        if(args[i][0] == '-'){ //Check for token having a '-' in it or not.
        //    printf("Argument: %d = ",i);
            char * tokens = strtok(*(args+i), "-");
            while(tokens != NULL){
                if(iTracker<ARGUMENT_COUNT){
                debuggerS(tokens,"",debugFlag);
                strncpy(argumentMap[iTracker].key,tokens,255);
                tokens = strtok(NULL, "-");
                }
                else{
                    printf("TOO MANY ARGUMENTS ENTERED, ONLY %d ARGUMENTS ALLOWED\n",ARGUMENT_COUNT);
                    return -1;
                }
            }
            argsPos[iTracker] = i+1;
            //printf("argumentMap: %s, iTracker-> %d, i-> %d\n",argumentMap[iTracker].key,iTracker,i+1);
            iTracker++;
        }   
        i++;
        
    }
    
    //printKeyValuePairs(argumentMap);
    int k = 0;
    //printArgs(argsPos);
    for(k=0;k<=realizedArgumentCount;k++){

        if(argsPos[k]>=argumentCounter){
            continue;
        }

        if((*(args+argsPos[k]))[0] != '-' && *(args+argsPos[k]) != NULL)
            strncpy(argumentMap[k].value,*(args+argsPos[k]),256);
        debuggerS(argumentMap[k].key,"",debugFlag);
        //printf("argumentTokens(%d): '%s'\n", k,argumentTokens[k]);
    }
    //printKeyValuePairs(argumentMap);

    return 0;
}

void printKeyValuePair(Map map){
    printf("Key: '%s', Value: '%s'\n", map.key, map.value);
}

void printArgs(int args[]){
    int i = 0;
    for(i=0; i<=realizedArgumentCount;i++)
        printf("argsPos[%d]: '%d'\n",i, args[i]);
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
    char currentDir[256];

    strcpy(currentDir,currentDirectory);

    char fullPath[1024];
    strcpy(fullPath,strcat(currentDirectory,fullName));
    FILE *fp = fopen(fullPath,"a+");
    printf("Path = '%s'\n",fullPath);
    if(fp == NULL){
        printf("THERE WAS AN ERROR OPENING THE FILE");
    }

    fprintf(fp, text);

    fclose(fp);

    if(isOpenDirectory == 1)
        openDirectory(currentDir);




}

int openDirectory(char *path){
    char targetDirectory[256];

    strcpy(targetDirectory, path);
    

    HINSTANCE result = ShellExecuteA(NULL, "open", targetDirectory, NULL, NULL, SW_SHOWNORMAL); // Open File

    // ShellExecute returns a value greater than 32 if it succeeds
    if ((INT_PTR)result <= 32) {
        printf("Failed to open directory.\n");
        return -1;
    }

    printf("Directory opened successfully!\n");
    return 0;


}

int debuggerS(char *debugInfo, char*dbg, int shouldPrint){
    if(shouldPrint == 1){
        if(strcmp(dbg,"") != 0)
            printf("%s -> %s\n",debugInfo,dbg);
        else
            printf("%s\n",debugInfo);
        return 0;
    }
    else
        return 0;
}
int debuggerD(int debugInfo, int shouldPrint){
    if(shouldPrint == 1){
        printf("%d\n",debugInfo);
        return 0;
    }
    else
        return 0;
}