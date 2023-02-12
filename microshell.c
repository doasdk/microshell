#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

#define AMOUNT_ELEM 10
int forChangeDir;
int forHelp;
int forExit;
char lastCWD[256];

void ChangeDirectory(char *pathName){
    if((pathName == NULL) || (strcmp(pathName, "~")==0)){
        getcwd(lastCWD, sizeof(lastCWD));
        chdir(getenv("HOME"));
        return;
    }
    if(strcmp(pathName, "-")==0){
        strcpy(pathName, lastCWD);
    }
    getcwd(lastCWD, sizeof(lastCWD));
    chdir(pathName);
}

void Help(){
    printf("\t __________________________________________________\n");
    printf("\t|            \x1b[36m       MicroShell            \x1b[0m         |\n");
    printf("\t|              Systemy Operacyjne 2022             |\n");
    printf("\t|          Autorka: Yevheniia Kryzhanovska         |\n");
    printf("\t|                    Polecenia                     |\n");
    printf("\t|               Własna implementacja:              |\n");
    printf("\t|1. cd     ---   przemieszczenie miedzy katalogami |\n");
    printf("\t|                cd [nazwa_katalogu]               |\n");
    printf("\t|2. exit   ---   konczyc dzialanie powloki         |\n");
    printf("\t|3. help   ---   wyswietlic pomoc                  |\n");
    printf("\t|__________________________________________________|\n");
}

void PrintPrompt(){
    char dirName[255];
    getcwd(dirName, sizeof(dirName));
    char *user = getenv("USER");
    printf("\x1b[0m[\x1b[31m%s\x1b[0m: \x1b[37m%s\x1b[39m]\n\x1b[0m", user, dirName);
}

char * Register(){
    char *inputReadL = NULL;
    inputReadL = readline("$ ");
    add_history(inputReadL);
    return inputReadL;
}
int main() {

    char *command = NULL;

    char **arrayOfCommands = NULL;
    arrayOfCommands = (char**)malloc(AMOUNT_ELEM*sizeof(char*));
    
    int i;
    
    do{
        errno=0;
        PrintPrompt();

        command = Register();

        arrayOfCommands[0] = strtok(command, " \n");

        for(i = 1; i < AMOUNT_ELEM; i++){
            arrayOfCommands[i] = strtok(NULL, " \n");
        }

        forChangeDir = strcmp(arrayOfCommands[0], "cd");
        forHelp = strcmp(arrayOfCommands[0], "help");
        forExit = strcmp(arrayOfCommands[0], "exit");

        if (forChangeDir == 0) {
            ChangeDirectory(arrayOfCommands[1]);
        }
        if (forHelp == 0) {
            Help();
        }

        if(forChangeDir!=0 && forHelp!=0 && forExit!=0) {
            pid_t forkID = fork();
            if (forkID == 0) {
                if (execvp(arrayOfCommands[0], arrayOfCommands) < 0) {

                    printf("\x1b[31mBłąd polecenia (%d)\x1b[0m: %s\n", errno, strerror(errno));
                    errno = 0;
                    exit(EXIT_FAILURE);
                }
                else {
                    exit(0);
                }
            }
            else {
                wait(NULL);
            }
        }
        if (errno != 0) { printf("\x1b[31mBłąd polecenia (%d)\x1b[0m: %s\n", errno, strerror(errno)); }

    } while(strcmp(arrayOfCommands[0], "exit")!=0);
    
    free(arrayOfCommands);
    
    return 0;
}

/*      Makefile:
 *  microshell:microshell.c
	    gcc -ansi -Wall -o microshell microshell.c -lreadline

    clean:
	    rm -f *.o microshell
 *
 *
 * */
