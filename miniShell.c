/*******************************************************************
 *           File:  miniShell.c
 *         Author:  Ashwitha Yadav T
 *   Date Started:  September 22, 2017
 *    Description:  A simple implementation of the Unix Shell in the
 *                      C Programming language.
 *******************************************************************/

#include <stdio.h>	// For: printf(), fprintf(), getchar(), perror(), stderr
#include <stdlib.h>     // For: malloc(), realloc(), free(), exit(), execvp(), EXIT_SUCCESS, EXIT_FAILURE
#include <string.h>     // For: strtok(), strcmp()
#include <unistd.h>     // For: chdir(), fork(), exec(), pid_t
#include <sys/wait.h>	// For: waitpid()

#define BUILTIN_COMMANDS 3	// Number of builtin commands defined

/*
 * Built-in command names
 */
char * builtin[] = {"cd", "exit", "help"};

/*
 * Built-in command functions
 */

/*
 * Function:  shell_cd
 * -------------------
 *  changes current working directory
 * 
 * args: arguments to the cd command, will consider only the first argument after the command name
 */
int shell_cd(char ** args){
	if (args[1] == NULL){
		fprintf(stderr, "minsh: one argument required\n");
	}
	else if (chdir(args[1]) < 0){
		perror("minsh");
	}
	return 1;
}

/*
 * Function:  shell_exit
 * ---------------------
 *  exits from the shell
 *
 * return: status 0 to indicate termination
 */
int shell_exit(char ** args){
	return 0;
}

/*
 * Function:  shell_help
 * ---------------------
 *  prints a small description
 *
 * return: status 1 to indicate successful termination
 */
int shell_help(char ** args){
	printf("\nA mini implementation of the Unix Shell by Ashwitha Yadav T.\n\n");
	return 1;
}

/*
 * Array of function pointers to built-in command functions
 */
int (* builtin_function[]) (char **) = {
	&shell_cd,
	&shell_exit,
	&shell_help
};

/*
 * Function:  split_command_line
 * -----------------------------
 *  splits a commandline into tokens using strtok()
 *
 *  command: a line of command read from terminal
 *
 *  returns: an array of pointers to individual tokens
 */
char ** split_command_line(char * command){
        int position = 0;
        int no_of_tokens = 64;
        char ** tokens = malloc(sizeof(char *) * no_of_tokens);
        char delim[2] = " ";

        // Split the command line into tokens with space as delimiter
        char * token = strtok(command, delim);
        while (token != NULL){
                tokens[position] = token;
                position++;
                //printf("\n\t%s", token);
                token = strtok(NULL, delim);
        }
        tokens[position] = NULL;
        //printf("\n");
        return tokens;
}

/*
 * Function:  read_command_line
 * ----------------------------
 *  reads a commandline from terminal
 *
 *  returns: a line of command read from terminal
 */
char * read_command_line(void){
        int position = 0;
        int buf_size = 1024;
        char * command = (char *)malloc(sizeof(char) * 1024);
        char c;

        // Read the command line character by character
        c = getchar();
        while (c != EOF && c != '\n'){
                command[position] = c;

                // Reallocate buffer as and when needed
                if (position >= buf_size){
                        buf_size += 64;
                        command = realloc(command, buf_size);
                }

                position++;
                c = getchar();
        }
        return command;
}

/*
 * Function:  start_process
 * -----------------------
 * starts and executes a process for a command
 *
 * args: arguments tokenized from the command line
 *
 * return: status 1
 */
int start_process(char ** args){
        int status;
        pid_t pid, wpid;

        pid = fork();

        if (pid == 0){  // It's the child process
                printf("\nChild Process");
                // Execute the required process

        }
        else if (pid < 0){      // Error in forking
		perror("minsh");
        }
        else{           // It's the parent process
                printf("\nParent Process!");
                do{
                        wpid = waitpid(pid, &status, WUNTRACED);
                } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }
        return 1;
}

int shell_execute(char ** args){
	
	if (args[0] == NULL){	// Empty command
		return 1;
	}

	// If the command is a built-in command, execute that function
	int i;
	for(i = 0 ; i < BUILTIN_COMMANDS ; i++){
		if ( strcmp(args[0], builtin[i]) == 0 ){
			return (* builtin_function[i])(args);
		}
	}

	// For other commands, execute a child process
	return start_process(args);

}

/*
 * Function:  shell_loop
 * ---------------------
 *  main loop of the Mini-Shell
 */
void shell_loop(void){
        char * command_line;
        char ** arguments;
        int status = 1;

        while (status == 1){
                printf("minsh> ");
                command_line = read_command_line();
		if ( strcmp(command_line, "") == 0 ){
			continue;
		}
                arguments = split_command_line(command_line);
                status = shell_execute(arguments);
        }
}

/*
 * Function:  main
 */
int main(int argc, char ** argv){
        // Load configuration files


        // Main loop of the shell
        shell_loop();

        // Shell shutdown and memory cleanup


}
