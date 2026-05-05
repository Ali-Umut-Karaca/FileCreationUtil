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
void writeCreateFile(char *t);
void printArgs(int args[]);
int overrideChecker(char *path);
int openFile(char *path);
void printHelpScreen();
//DEFINITION LIST:

#define COLOR_RED     "\033[0;31m"
#define COLOR_GREEN   "\033[0;32m"
#define COLOR_YELLOW  "\033[0;33m"
#define COLOR_CYAN    "\033[0;36m"
#define COLOR_RESET   "\033[0m"
#define TYPE "e" //Extension
#define INIT "i" //Inýtialization
#define NAME "n" //Name
#define PATH "p" //CustomPath
#define OF "of" //Open File
#define OD "od" //Open Directory
#define HELP "help" //Help
#define DOPEN "dof" //Don't Open File -> 04.05.2026
#define ARGUMENT_COUNT 8
#define MAX_PATH_LENGTH 1024
#define MAX_ARGUMENT_LENGTH 2048
//


// DECLARE AND INITIALIZE VARIABLES:
int debugFlag = 0; // 1 is on, 0 is off
int pathFlag = 0; // If the path has been set by the user specifically, turn the bit to 1
int isOpenFile = 0; // If this flag is set, open up the file that's just been created. command: -of
int isOpenDirectory = 0; // Flag for directory opening on -do command 
int isDOF = 0; // Don't open file flag
int state = 0; //0 -> Name, 1-> extention, 2-> initialization
int realizedArgumentCount = 0;
int argumentCounter = 0;
Map argumentMap [ARGUMENT_COUNT];
char argumentTokens[ARGUMENT_COUNT][256];
int argsPos [ARGUMENT_COUNT];
char fullPath[MAX_PATH_LENGTH]; // Space allocated for Full path with the file.
//

typedef struct FileInfo{
  char extention[255];
  char name[255];
  char fullName[255];
  char specifiedPath[MAX_PATH_LENGTH];
}FileInfo;

//FILE INFO:
/*char extention[8];
char name[248];
char fullName[256];
char specifiedPath[MAX_PATH_LENGTH];*/ //DEPRECATED
FileInfo fileInfo;
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

    strcpy(fileInfo.name, "tmp");
    strcpy(fileInfo.extention, ".txt");
    char initValue[MAX_ARGUMENT_LENGTH]; 
    int shouldInit = 0;       // A simple flag to check if INIT was called
    
    int i = 0;
    for(i = 0; i < realizedArgumentCount+1; i++) {
            debuggerD(i,debugFlag);
        if (argumentMap[i].key[0] == '\0') continue; 

        if(strcmp(NAME, argumentMap[i].key) == 0) {
            if(strcmp(argumentMap[i].value, "") != 0) {
	      strncpy(fileInfo.name, argumentMap[i].value,255); // Windows allows a max of 202 chars for file names.
            }
        //    printf("Parsed NAME: %s\n", name);
        }
        
        else if(strcmp(TYPE, argumentMap[i].key) == 0) {
            if(strcmp(argumentMap[i].value, "") != 0) {
	      strncpy(fileInfo.extention, argumentMap[i].value,6); // Max length of an extension is 8 bytes.
            }
        //    printf("Parsed TYPE: %s\n", extention);
        }

        else if(strcmp(OD, argumentMap[i].key) == 0){
            debuggerS("I worked for OD","",debugFlag);
            isOpenDirectory = 1;   
        }
        
        else if(strcmp(INIT, argumentMap[i].key) == 0) {
	  strncpy(initValue, argumentMap[i].value, MAX_ARGUMENT_LENGTH); //strncpy -> safe way to copy entered argument
            shouldInit = 1; 
        //    printf("Parsed INIT flag.\n");
        }
        
        else if(strcmp(PATH, argumentMap[i].key) == 0){
            pathFlag = 1;

            DWORD fileAttributes = GetFileAttributesA(argumentMap[i].value);

            if (fileAttributes != INVALID_FILE_ATTRIBUTES) {
	      //printf("strlen: %lld",strlen(argumentMap[i].value));
	      strncpy(fileInfo.specifiedPath,argumentMap[i].value, 255); //255 MAX
            } else {
                printf("%sINVALID PATH: '%s'%s\n",COLOR_RED,argumentMap[i].value,COLOR_RESET);
                return -1;
            }

               
        }
	else if(strcmp(OF, argumentMap[i].key) == 0){
            debuggerS("I worked for OF","",debugFlag);
            isOpenFile = 1;   
        }
	else if(strcmp(HELP, argumentMap[i].key) == 0){
            debuggerS("I worked for HELP","",debugFlag);
	    printHelpScreen();
        }
	else if(strcmp(DOPEN, argumentMap[i].key) == 0){
	  isDOF = 1;
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
     

     if(isDOF == 0){
       printf("\n%s--- Final Configuration ---%s\n",COLOR_CYAN,COLOR_RESET);
       printf("Name: %s'%s'%s\n",COLOR_CYAN, fileInfo.name, COLOR_RESET);
       printf("Extension: %s'%s'%s\n",COLOR_CYAN, fileInfo.extention, COLOR_RESET);
       printf("Path = %s'%s'%s\n", COLOR_CYAN, fullPath, COLOR_RESET); //Print the whole file path to the terminal for the user to see.
     }

   
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

        if((*(args+argsPos[k]))[0] != '-' && *(args+argsPos[k]) != NULL){
	  strncpy(argumentMap[k].value,*(args+argsPos[k]),MAX_ARGUMENT_LENGTH);
	  debuggerS(argumentMap[k].key,"",debugFlag);
	}
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
  strncpy(fileInfo.fullName,fileInfo.name,strlen(fileInfo.name)); // Add name to the fullName
    strcat(fileInfo.fullName,fileInfo.extention); // Add extension to the fullName
    char currentDirectory[MAX_PATH_LENGTH]; // Allocate memory to get Path in Stack
    _getcwd(currentDirectory, MAX_PATH_LENGTH); // Get the Current Directory Path
    char currentDir[MAX_PATH_LENGTH]; // Allocate enough memory to store the directory path

    
     if(pathFlag == 1){ //Change path if any is pre-Specified
       strncpy(currentDirectory,fileInfo.specifiedPath,255); //255 MAX
    }

    strcat(currentDirectory,"\\"); // Add the '\' at the end of the Current Directory Path
    
    strcpy(currentDir,currentDirectory); // Copy the full path to the DIRECTORY for later use. strcpy needed for null terminator.
    //currentDir[strlen(currentDirectory)] = '\0';
    

    if(isDOF == 1 && isOpenDirectory == 1){
      debuggerS("curDir", currentDir,1);
      printf("Opening Directory...\n");
      openDirectory(currentDir);
      exit(0);
    }

    strcpy(fullPath,strcat(currentDirectory,fileInfo.fullName)); // First attach fullName to currentDirectory and then Copy the new string to fullPath

    int override = overrideChecker(fullPath); // If the return value is 1, we proceed to override / create . If the return is -1, don't override it and terminate.

    if(override == -1){
      printf("\nOverride command not accepted. Process terminating...\n");
      exit(1);
    }
    int fileNameCount = strlen(fullPath);
    // printf("\n fileNameCount In Func: %d\n", fileNameCount);

    int pathLength = strlen(currentDir);

    FILE *fp = fopen(fullPath,"a+"); // Open the file in Append+ mode.

    if(fileNameCount >255){
      printf("%sFILE COULDN'T BE OPENED / CREATED. CODE: FNTL001%s\n",COLOR_YELLOW,COLOR_RESET);
      printf("\n%sPLEASE LIMIT YOUR FILE NAME WITH EXTENSION TO: %s%d (Your file name length: %lld)%s\n",COLOR_YELLOW,COLOR_GREEN,(255-pathLength),strlen(fileInfo.fullName),COLOR_RESET);
      printf("\npath ->%s, len -> %d\n",currentDir, pathLength);
      printf("\nfullName->%s\n",fileInfo.fullName);
      exit(-1);
    }
    
    if(fp == NULL){
        printf("\n%sTHERE WAS AN ERROR OPENING / CREATING THE FILE%s\n",COLOR_RED,COLOR_RESET);
	exit(-1);
    }
    else{
      fprintf(fp, text); // Put the text into the file

      fclose(fp); // Close off the file
    }

    if(isOpenDirectory == 1)
      openDirectory(currentDir); //Change the state variable, add -of, -od difference.
    if(isOpenFile == 1)
      openFile(fullPath);

    printf("%sFile created/appended successfully!%s\n",COLOR_GREEN,COLOR_RESET);
    

}

int openDirectory(char *path){
    char targetDirectory[255];

    strcpy(targetDirectory, path);
    
    debuggerS(targetDirectory, path, debugFlag);
    HINSTANCE result = ShellExecuteA(NULL, "open", targetDirectory, NULL, NULL, SW_SHOWNORMAL); // Open File

    // ShellExecute returns a value greater than 32 if it succeeds
    if ((INT_PTR)result <= 32) {
      printf("%sFailed to open directory. CODE: %d%s\n",COLOR_RED,(int)result,COLOR_RESET);
        return -1;
    }

    //    printf("%sDirectory opened successfully!%s\n",COLOR_GREEN,COLOR_RESET);
    return 0;


}


int grepLogic(char *str){

  
  return 0;
}

int openFile(char *path){
  char fh [MAX_PATH_LENGTH + 15]; // 15 for command initialization
  strcpy(fh,"start \"\" \"");
  strcat(fh,path);
  system(fh);
    return 0;
}

int overrideChecker(char *path){
  char input [3];
  int validationFlag = 0;
  //strcpy(&input[1],"\0"); // ADD NULL TERMINATOR STRING
  FILE *myFile = fopen(path, "r");

  if(myFile != NULL){
    printf("%sWARNING: THIS FILE SEEMS TO ALREADY EXIST%s\n",COLOR_YELLOW,COLOR_RESET);
    printf("Are you sure you would like to append to this file ? (y/n):");

    while(validationFlag != 1){
      if(fgets(input, sizeof(input), stdin) != NULL){
	if (strchr(input, '\n') == NULL) { // Checks for buffer overflows
	  printf("Please only enter 'y' or 'n':\n"); 
		int c;
                while ((c = getchar()) != '\n' && c != EOF); // In case of buffer overflows, empty out the stdin buffer.
                
                continue; // Continue to next itiration.
	}
	if (input[0] == '\n') { // If user leaves the input buffer empty.
	  printf("Please enter 'y' or 'n' to proceed:");
                continue;
            }
		
    
	 if(input[0] == 'y'){
	   fclose(myFile);
	   return 1;
	 }
	 else if(input[0]== 'n'){
	   fclose(myFile);
	   return -1;
	 }
	 else if(input[0]== 'N'){
	   fclose(myFile);
	   return -1;
	 }
	 else if(input[0]== 'Y'){
	   fclose(myFile);
	   return 1;
	 }
	 else
	   printf("Please enter 'n' or 'y' to proceed:");

      } //fgets paranthesis.
    } // validation Flag paranthesis.
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

void printHelpScreen(){
  char *sp = "-\n";
  printf("\n%s~~~~~~~~~~~~~~~COMMANDS AVAILABLE FOR USE~~~~~~~~~~~~~~~%s\n",COLOR_CYAN,COLOR_RESET);
  printf("|-help: to pull up this documentation\n|%s%s|%s%s|%s%s|%s%s|%s%s|%s%s|%s%s|%s%s", sp,
	 "|-n: to name the file as desired. If this field's left empty, the default name is going to be 'tmp'.\n", sp,
	 "|-e: to give a file the desired extension. If this field's left empty, the default extension is going to be '.txt'.\n", sp,
	 "|-i: to give a file the desired initialization. It will create the file and write in it the provided text. (Limit: 1024 characters)\n", sp,
	 "|-of: to open up the crated file after creating it.\n", sp,
	 "|-od: to open up the created file's directory after it's been created.\n", sp,
	 "|-dof: to not create a file, I use it to navigate the folders from the cmd and open directories in the GUI when I need to.\n", sp,
	 "|-p: to provide a custom ABSOLUTE path for the program to create the file in.\n", sp,
	 "  |- MONTECARLO @_@ \\@w@/ Q_Q - https://github.com/Ali-Umut-Karaca/FileCreationUtil.git -\n"
	 );
  printf("%s~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~%s",COLOR_CYAN,COLOR_RESET);
  exit(1);
}
