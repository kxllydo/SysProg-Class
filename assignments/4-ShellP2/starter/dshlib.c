#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */
 

    // TODO IMPLEMENT parsing input to cmd_buff_t *cmd_buff

    // TODO IMPLEMENT if built-in command, execute builtin logic for exit, cd (extra credit: dragon)
    // the cd command should chdir to the provided directory; if no directory is provided, do nothing

    // TODO IMPLEMENT if not built-in command, fork/exec as an external command
    // for example, if the user input is "ls -l", you would fork/exec the command "ls" with the arg "-l"
void remove_spaces(char *str) {
    int i = 0, j = 0;
    bool inside_quotes = false;

    while (str[i] != '\0') {
        if (str[i] == '\\' && (str[i+1] == '"' || str[i+1] == '\'')) {
            str[j++] = str[i++];
            str[j++] = str[i++];
        } else if (str[i] == '"' || str[i] == '\'') {
            inside_quotes = !inside_quotes;
            str[j++] = str[i++];
        } else if (isspace(str[i])) {
            if (inside_quotes) {
                str[j++] = str[i++];
            } else {
                i++;
            }
        } else {
            str[j++] = str[i++]; 
        }
    }

    str[j] = '\0';
}

int parse_input_to_cmd_buff(char *input, cmd_buff_t *cmd_buff) {
    int argc = 0;
    char *start = input;
    bool in_quotes = false;

    while (*start) {
        while (*start == ' ' || *start == '\t') {
            start++;
        }

        if (*start == '\0') {
            break;
        }
        if (*start == '"') {
            in_quotes = true;
            start++;
            cmd_buff->argv[argc++] = start;
            while (*start && (*start != '"' || (*(start - 1) == '\\'))) {
                start++;
            }

            if (*start == '"') {
                *start = '\0';
                start++;
            }
        } else {
            cmd_buff->argv[argc++] = start;
            while (*start && (*start != ' ' && *start != '\t' && (!in_quotes || *start != '"'))) {
                start++;
            }
        }

        if (*start) {
            *start = '\0';
            start++;
        }

        if (argc >= CMD_ARGV_MAX - 1) {
            printf("Too many arguments!\n");
            return -1;
        }
    }

    cmd_buff->argv[argc] = NULL;
    cmd_buff->argc = argc;
    return 0;
}
    
int exec_local_cmd_loop() {
    cmd_buff_t cmd;
    cmd._cmd_buffer = malloc(SH_CMD_MAX);
    if (!cmd._cmd_buffer) {
        perror("malloc");
        return -1;
    }

    while (1) {
        printf("%s", SH_PROMPT);
        if (fgets(cmd._cmd_buffer, SH_CMD_MAX, stdin) == NULL) {
            printf("\n");
            break;
        }

        cmd._cmd_buffer[strcspn(cmd._cmd_buffer, "\n")] = '\0';

        if (strlen(cmd._cmd_buffer) == 0) {
            continue;
        }

        if (strcmp(cmd._cmd_buffer, EXIT_CMD) == 0) {
            free(cmd._cmd_buffer);
            exit(0);
        }

        if (strncmp(cmd._cmd_buffer, "cd", 2) == 0) {
            char *arg = cmd._cmd_buffer + 2;
            while (isspace(*arg)) arg++;
            if (strlen(arg) == 0) {
                continue;
            }

            if (chdir(arg) != 0) {
                perror("cd");
            }
            continue;
        }

        if (strcmp(cmd._cmd_buffer, "dragon") == 0) {
            print_dragon();
            continue;
        }

        if (parse_input_to_cmd_buff(cmd._cmd_buffer, &cmd) == -1) {
            continue;
        }

        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            continue;
        } else if (pid == 0) {
            execvp(cmd.argv[0], cmd.argv);
            perror("execvp");
            exit(1);
        } else {
            int status;
            waitpid(pid, &status, 0);
        }
    }

    free(cmd._cmd_buffer);
    return 0;
}

