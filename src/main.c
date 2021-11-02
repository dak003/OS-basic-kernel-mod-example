/*
    UMBC CMSC 421
    Spring 2021
    Project 1

    Due Date: 2/28/21 11:59:00 pm

    Author Name: David Kravets
    Author email: kravets1@umbc.edu
    Description: a simple linux shell designed to perform basic linux commands
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include "utils.h"
#define delim " "


/*
    In this project, you are going to implement a number of functions to
    create a simple linux shell interface to perform basic linux commands.
    Please note that the below set of functions may be modified as you see fit.
    We are just giving the following as a suggestion. If you do use our
    suggested design, you *will* need to modify the function signatures (adding
    arguments and return types).
*/


void user_prompt_loop(void);
char *get_user_command(void);
char **parse_command(char *);
int execute_command(char **);
int progExit(char **userInput);
int proc(char **input);



int main(int argc, char *argv[]) {
	if(argc == 1){
		printf("Shell Loaded!");
		user_prompt_loop();
		return 1;
	}else{
		free(argv);
		fprintf(stderr, "Too many arguments given to program");
		putchar('\n');
		exit(1);
	}
}

/*
    user_prompt_loop():
    Get the user input using a loop until the user exits, prompting the user for
    a command. Gets command and sends it to a parser, then compares the first
    element to the two built-in commands ("proc", and "exit"). If it's none of
    the built-in commands, send it to the execute_command() function. If the
    user decides to exit, then exit 0 or exit with the user given value.
*/
void user_prompt_loop() {
	char *input;
	int repeat = 1;
	char **split;

	while(repeat != 0){
		printf("\n$ ");
		input = get_user_command();
		split = parse_command(input);
		repeat = execute_command(split);
		free(input);
		free(split);
	}
}
    /*
      loop:
        1. prompt the user to type command by printing "$ " (don't forget the
           space)
        2. get the user input using get_user_command() function
        3. parse the user input using parse_command() function
        Example:
            user input: "ls -la"
            parsed output: ["ls", "-la", NULL]
        4. compare the first element of the parsed output to "proc"and "exit"
        5. if the first element is "proc" then you have to use the /proc file
           system as described in part 2 of the project
            i) concat the full command:
                Ex: user input $ proc /process_id_no/status
                    concated output: /proc/process_id_no/status
            ii) read from the file line by line. you may user fopen() and
                getline() functions
            iii) display the following information according to the user input
                 from /proc
                a) Get the cpu information if the input is /proc/cpuinfo
                - Cpu Mhz
                - Cache size
                - Cpu cores
                - Address sizes
                b) Get the number of currently running processes from
                   /proc/loadavg
                c) Get how many seconds your box has been up, and how many
                   seconds it has been idle from /proc/uptime
                d) Get the following information from /proc/process_id_no/status
                - the vm size of the virtual memory allocated the vbox
                - the most memory used vmpeak
                - the process state
                - the parent pid
                - the number of threads
                - number of voluntary context switches
                - number of involuntary context switches
                e) display the list of environment variables from
                   /proc/process_id_no/environ
                f) display the performance information if the user input is
                   /proc/process_id_no/sched
        6. if the first element is "exit" the use the exit() function to
           terminate the program
        7. otherwise pass the parsed command to execute_command() function
        8. free the allocated memory using the free() function
    */

    /*
        Functions you may find useful here (not an exhaustive list):
        get_user_command(), parse_command(), execute_command(), strcmp(),
        strcat(), strlen(), strncmp(), fopen(), fclose(), getline(), isdigit(),
        atoi(), fgetc()
    */


/*
    get_user_command():
    Take input of arbitrary size from the user and return to the
    user_prompt_loop()
*/
char *get_user_command(void) {
	unsigned int maxLength = 256;
	unsigned int size;
	size = maxLength;
	char *stringPointer = malloc(maxLength);

	if(stringPointer != NULL){
		int character = EOF;
		unsigned int i = 0;
		while ((character = getchar()) != '\n' && character != EOF){
			stringPointer[i++] = (char)character;
			if(i == size){
				size = i + maxLength;
				stringPointer = realloc(stringPointer, size);
			}
		}

		stringPointer[i] = '\0';
		return stringPointer;

	}
	return NULL;
}
    /*
        Functions you may find useful (not an exhaustive list):
        malloc(), realloc(), getline(), fgetc()
    */

/*
    parse_command():
    Take command input read from the user and parse appropriately.
    Example:
        user input: "ls -la"
        parsed output: ["ls", "-la", NULL]
    Example:
        user input: "echo     hello                     world  "
        parsed output: ["echo", "hello", "world", NULL]
*/
char** parse_command(char *userInput) {
	unsigned int tokenBufferSize = 128;
	int bufSize = 128;
	char **tokens = malloc(bufSize * sizeof(char*));

	char *firstArg = malloc(bufSize);
	int i = 0;
	while(userInput[i] != ' ' && userInput[i] != '\0'){
		firstArg[i] = userInput[i];
		i++;
	}
	char *procCMD[] = {"proc"};
	char *exitCMD[] = {"exit"};
	if((strcmp(firstArg, procCMD[0]) != 0)  && strcmp(firstArg, exitCMD[0]) != 0){

		char *token;
		int bufsize = tokenBufferSize;
		int currentPos = 0;
		tokens = malloc(bufSize * sizeof(char*));


		if (!tokens) {
			fprintf(stderr, "Token Error\n");
			return 0;
		}

		token = strtok(userInput, delim);
		while (token != NULL) {
			tokens[currentPos] = token;
			currentPos++;
			if (currentPos >= bufsize) {
				bufsize = bufsize + tokenBufferSize;
				tokens = realloc(tokens, bufsize * sizeof(char*));
				if (!tokens) {
					fprintf(stderr, "Token Error\n");
					return 0;
				}
			}

			token = strtok(NULL, delim);
		}
		tokens[currentPos] = NULL;
		return tokens;
	}else{

		char *nextChar;
		int currentToken = 0;
		int nextUnquoted = 0;
		int lastUnquoted = 0;
		int totalCharsForArg;
		nextUnquoted = first_unquoted_space(userInput);
		//loop to get chars in command until break
		while(nextUnquoted != -1 ){
			totalCharsForArg = nextUnquoted - lastUnquoted;

			char *nextArgument = malloc(bufSize);
			nextArgument = realloc(nextArgument, totalCharsForArg);
			//get chars until break
			for(int j = 0; j < totalCharsForArg; j++){
					nextChar = userInput+j+lastUnquoted;
					if(*nextChar != '\0')
						nextArgument[j] = userInput[j+lastUnquoted+currentToken];
				}

			tokens[currentToken] = nextArgument;

			lastUnquoted = nextUnquoted;
			nextUnquoted = (first_unquoted_space((userInput+lastUnquoted+1+currentToken)));
			currentToken++;
			if(nextUnquoted == -1){
				break;
			}else{
				nextUnquoted = nextUnquoted+lastUnquoted;
				if(currentToken >= bufSize){
					bufSize = bufSize + tokenBufferSize;
					tokens = realloc(tokens, bufSize * sizeof(char*));
					if (tokens == NULL) {
						return 0;
					}
				}
			}

		}
		char *nextArgument = malloc(bufSize);
		lastUnquoted++;
		i = 0;
		int bool = 1;
		while(bool == 1){
			nextChar = userInput+i+lastUnquoted;
			if(*nextChar != '\000')
				nextArgument[i] = userInput[i+lastUnquoted+currentToken-1];
			else
				bool = 0;
			if(i > bufSize){
				nextArgument = realloc(nextArgument, i+bufSize);
			}
			i++;
		}
		nextUnquoted = -1;
		tokens[currentToken] = nextArgument;
		currentToken++;
		tokens[currentToken] = NULL;
		return tokens;
	}
}
    /*
        Functions you may find useful (not an exhaustive list):
        malloc(), realloc(), free(), strlen(), first_unquoted_space(),
        unescape()
    */

/*
    execute_command():
    Execute the parsed command if the commands are neither proc nor exit;
    fork a process and execute the parsed command inside the child process
*/
int execute_command(char **input) {
	//check for the built in functions and execute
	if(input[0][0] == 'p' && input[0][1] == 'r' && input[0][2] == 'o'){
		return (proc(input));
	}else if(input[0][0] == 'e' && input[0][1] == 'x' && input[0][2] == 'i'){
		return (progExit(input));
		//if not built in, parse and execute external cmd
	}else{
		int counter = 1;
		while (input[counter] != NULL){
			input[counter] = unescape(input[counter], stderr);
			counter++;
		}
		pid_t pid;
		int statusOfProcess;
		pid = fork();
		if (pid == 0) {
			if (execvp(input[0], input) == -1) {
				fprintf(stderr, "An error occured in forking");
				return 0;
			}
			fprintf(stderr, "An error occured in forking");
			return 0;
		} else if (pid < 0) {
			fprintf(stderr, "An error occured in forking");
			return 0;
		}else {
			do {
				waitpid(pid, &statusOfProcess, WUNTRACED);
			} while (!WIFEXITED(statusOfProcess) && !WIFSIGNALED(statusOfProcess));
		}
	}

	return 1;

}
    /*
        Functions you may find useful (not an exhaustive list):
        fork(), execvp(), waitpid(), wait()
    */

int progExit(char **input){
	if(input[1] == NULL){
		exit(0); //exit with exit code 0
	}
	int inputInt = atoi(input[1]);
	if(inputInt){

		exit(inputInt); //exit with exit code passed in
	}
	return 1;

}

int proc(char **input){
	FILE *fp = fopen(*input, "r");
	size_t n = 0;
	char *line = NULL;
	while (getline(&line, &n, fp) > 0) {
			printf("%s", line);
	}
	free(line);
	fclose(fp);
	return 1;
}

