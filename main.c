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

int grepLogic(char *str);
int openDirectory(char *path);
int debuggerS(char *debugInfo,char *dbg, int shouldPrint);
int debuggerD(int debugInfo, int shouldPrint);
int argumentParser(char **args);
void printKeyValuePair(Map map);
void printKeyValuePairs(Map map[]);
void writeCreateFile(char *text);
void printArgs(int args[]);
int overrideChecker(char *path);
int openFile(char *path);

//DEFINITION LIST:

#define COLOR_RED     "\033[0;31m"
#define COLOR_GREEN   "\033[0;32m"
#define COLOR_YELLOW  "\033[0;33m"
#define COLOR_CYAN    "\033[0;36m"
#define COLOR_RESET   "\033[0m"
#define TYPE "e"
#define INIT "i"
#define NAME "n"
#define PATH "p"
#define OF "of"
#define OD "od"
#define ARGUMENT_COUNT 6
#define MAX_PATH_LENGTH 1024
//


// DECLARE AND INITIALIZE VARIABLES:
int debugFlag = 0; // 1 is on, 0 is off
int pathFlag = 0; // If the path has been set by the user specifically, turn the bit to 1
int isOpenFile = 0; // If this flag is set, open up the file that's just been created. command: -of
int isOpenDirectory = 0; // Flag for directory opening on -do command 
int state = 0; //0 -> Name, 1-> extention, 2-> initialization
int realizedArgumentCount = 0;
int argumentCounter = 0;
Map argumentMap [ARGUMENT_COUNT];
char argumentTokens[ARGUMENT_COUNT][256];
int argsPos [ARGUMENT_COUNT];
char fullPath[1024]; // Space allocated for Full path with the file.
//

//FILE INFO:
char extention[8];
char name[248];
char fullName[256];
char specifiedPath[1024];
//




int main (int argc, char **args){
    argumentCounter = argc;//Inititalize global variable
    realizedArgumentCount = (argc-1)/2;                                    //double res = ceil(((double)argc-1)/2); //Actually realized argument amount arithmetic
                                                                          //realizedArgumentCount = (int) res;
    if(realizedArgumentCount>ARGUMENT_COUNT){
        printf("%sTOO MANY ARGUMENTS ENTERED !%s\n",COLOR_RED,COLOR_RESET);
        return -1;
    }
    
    debuggerD(argc,debugFlag);
    
    if(argumentParser(args) == -1){ // Parse Commands into a poor man's Hashmap (argumentMap)
        printf("%sOPERATION TERMINATING...%s",COLOR_RED,COLOR_RESET);
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

        else if(strcmp(OD, argumentMap[i].key) == 0){
            debuggerS("I worked for OP","",debugFlag);
            isOpenDirectory = 1;   
        }
        
        else if(strcmp(INIT, argumentMap[i].key) == 0) {
            strcpy(initValue, argumentMap[i].value);
            shouldInit = 1; 
        //    printf("Parsed INIT flag.\n");
        }
        
        else if(strcmp(PATH, argumentMap[i].key) == 0){
            pathFlag = 1;

            DWORD fileAttributes = GetFileAttributesA(argumentMap[i].value);

            if (fileAttributes != INVALID_FILE_ATTRIBUTES) {
                strcpy(specifiedPath,argumentMap[i].value);
            } else {
                printf("%sINVALID PATH: '%s'%s\n",COLOR_RED,argumentMap[i].value,COLOR_RESET);
                return -1;
            }

               
        }
	else if(strcmp(OF, argumentMap[i].key) == 0){
            debuggerS("I worked for OP","",debugFlag);
            isOpenFile = 1;   
        }
        else {
            printf("%sWarning: Unknown argument key %s'%s' %signored.%s\n", COLOR_YELLOW,
		   COLOR_RESET,
		   argumentMap[i].key,
		   COLOR_YELLOW,
		   COLOR_RESET);
        }
    }


     if (shouldInit) {
        writeCreateFile(initValue);
    } else {
        writeCreateFile(""); 
    }
     
    printf("\n%s--- Final Configuration ---%s\n",COLOR_CYAN,COLOR_RESET);
    printf("Name: %s'%s'%s\n",COLOR_CYAN, name, COLOR_RESET);
    printf("Extension: %s'%s'%s\n",COLOR_CYAN, extention, COLOR_RESET);
    printf("Path = %s'%s'%s\n", COLOR_CYAN, fullPath, COLOR_RESET); //Print the whole file path to the terminal for the user to see.


   
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
                    printf("%sTOO MANY ARGUMENTS ENTERED, ONLY %d ARGUMENTS ALLOWED\n%s",COLOR_RED,ARGUMENT_COUNT,COLOR_RESET);
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
    strcpy(fullName,name); // Add name to the fullName
    strcat(fullName,extention); // Add extension to the fullName
    char currentDirectory[MAX_PATH_LENGTH]; // Allocate memory to get Path in Stack
    _getcwd(currentDirectory, MAX_PATH_LENGTH); // Get the Current Directory Path
    char currentDir[MAX_PATH_LENGTH]; // Allocate enough memory to store the directory path

    
     if(pathFlag == 1){ //Change path if any is pre-Specified
        strcpy(currentDirectory,specifiedPath);
    }

    strcat(currentDirectory,"\\"); // Add the '\' at the end of the Current Directory Path

    strcpy(currentDir,currentDirectory); // Copy the full path to the DIRECTORY for later use

    strcpy(fullPath,strcat(currentDirectory,fullName)); // First attach fullName to currentDirectory and then Copy the new string to fullPath

    int override = overrideChecker(fullPath); // If the return value is 1, we proceed to override / create file. If the return is -1, don't override it and terminate.

    if(override == -1){
      printf("\nOverride command not accepted. Process terminating...\n");
      exit(1);
    }
    
    FILE *fp = fopen(fullPath,"a+"); // Open the file in Append+ mode.
    
    if(fp == NULL){
        printf("\n%sTHERE WAS AN ERROR OPENING / CREATING THE FILE%s\n",COLOR_RED,COLOR_RESET);
    }

   

    fprintf(fp, text); // Put the text into the file

    fclose(fp); // Close off the file

    if(isOpenDirectory == 1)
      openDirectory(currentDir); //Change the state variable, add -of, -od difference.
    if(isOpenFile == 1)
      openFile(fullPath);

    printf("%sFile created/appended successfully!%s\n",COLOR_GREEN,COLOR_RESET);
    

}

int openDirectory(char *path){
    char targetDirectory[256];

    strcpy(targetDirectory, path);
    

    HINSTANCE result = ShellExecuteA(NULL, "open", targetDirectory, NULL, NULL, SW_SHOWNORMAL); // Open File

    // ShellExecute returns a value greater than 32 if it succeeds
    if ((INT_PTR)result <= 32) {
        printf("%sFailed to open directory.%s\n",COLOR_RED,COLOR_RESET);
        return -1;
    }

    //    printf("%sDirectory opened successfully!%s\n",COLOR_GREEN,COLOR_RESET);
    return 0;


}


int grepLogic(char *str){

  
  return 0;
}

int openFile(char *path){
  char fh [MAX_PATH_LENGTH + 15];
  strcpy(fh,"start \"\" \"");
  strcat(fh,path);
  system(fh);
    return 0;
}

int overrideChecker(char *path){
  char input [2];
  strcpy(&input[1],"\0"); // ADD NULL TERMINATOR STRING
  FILE *myFile = fopen(path, "r");

  if(myFile != NULL){
    printf("%sWARNING: THIS FILE SEEMS TO ALREADY EXIST%s\n",COLOR_YELLOW,COLOR_RESET);
    printf("Are you sure you would like to append to this file ? (y/n):");
    scanf("%s",input);
    if(strcmp(input,"y")== 0){
      fclose(myFile);
      return 1;
    }
    else if(strcmp(input,"n") == 0){
      fclose(myFile);
      return -1;
    }
  }
  else{
    return 1;
  }
  
  fclose(myFile);
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
