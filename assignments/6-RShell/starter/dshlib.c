#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"



#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"

/**** 
 **** FOR REMOTE SHELL USE YOUR SOLUTION FROM SHELL PART 3 HERE
 **** THE MAIN FUNCTION CALLS THIS ONE AS ITS ENTRY POINT TO
 **** EXECUTE THE SHELL LOCALLY
 ****
 */

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

int free_cmd_list(command_list_t *cmd_list) {
    for (int i = 0; i < cmd_list->num; i++) {
        free_cmd_buff(&cmd_list->commands[i]);
    }
    return OK;
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

int alloc_cmd_buff(cmd_buff_t *cmd_buff) {
    cmd_buff->_cmd_buffer = malloc(SH_CMD_MAX);
    if (!cmd_buff->_cmd_buffer) {
        return ERR_MEMORY;
    }
    return OK;
}

// Free memory allocated for the command buffer
int free_cmd_buff(cmd_buff_t *cmd_buff) {
    if (cmd_buff->_cmd_buffer) {
        free(cmd_buff->_cmd_buffer);
        cmd_buff->_cmd_buffer = NULL;
    }
    return OK;
}

// Build the command buffer by parsing the input string
int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff) {
    if (strlen(cmd_line) >= SH_CMD_MAX) {
        return ERR_CMD_OR_ARGS_TOO_BIG;
    }

    strncpy(cmd_buff->_cmd_buffer, cmd_line, SH_CMD_MAX);
    cmd_buff->_cmd_buffer[SH_CMD_MAX - 1] = '\0'; // Ensure null termination

    // Parse the command line into arguments
    return parse_input_to_cmd_buff(cmd_buff->_cmd_buffer, cmd_buff);
}
// Free memory allocated for the command list

int build_cmd_list(char *cmd_line, command_list_t *clist) {
    char *token;
    int i = 0;

    token = strtok(cmd_line, PIPE_STRING);
    while (token != NULL && i < CMD_MAX) {
        if (alloc_cmd_buff(&clist->commands[i]) != OK) {
            return ERR_MEMORY;
        }
        if (build_cmd_buff(token, &clist->commands[i]) != OK) {
            return ERR_CMD_ARGS_BAD;
        }
        token = strtok(NULL, PIPE_STRING);
        i++;
    }

    if (token != NULL) {
        return ERR_TOO_MANY_COMMANDS;
    }

    clist->num = i;
    return OK;
}

int free_command_list(command_list_t *cmd_lst) {
    for (int i = 0; i < cmd_lst->num; i++) {
        free_cmd_buff(&cmd_lst->commands[i]);
    }
    cmd_lst->num = 0;
    return OK;
}

int execute_pipeline(command_list_t *clist) {
    int pipefd[2];
    int prev_pipe_read = -1;
    pid_t pids[CMD_MAX];
    int status;

    for (int i = 0; i < clist->num; i++) {
        if (i < clist->num - 1) {
            if (pipe(pipefd) == -1) {
                perror("pipe");
                return ERR_EXEC_CMD;
            }
        }

        pids[i] = fork();
        if (pids[i] == -1) {
            perror("fork");
            return ERR_EXEC_CMD;
        } else if (pids[i] == 0) {
            if (i > 0) {
                dup2(prev_pipe_read, STDIN_FILENO);
                close(prev_pipe_read);
            }
            if (i < clist->num - 1) {
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);
                close(pipefd[0]);
            }
            execvp(clist->commands[i].argv[0], clist->commands[i].argv);
            perror("execvp");
            exit(ERR_EXEC_CMD);
        } else {
            if (i > 0) {
                close(prev_pipe_read);
            }
            if (i < clist->num - 1) {
                close(pipefd[1]);
                prev_pipe_read = pipefd[0];
            }
        }
    }

    for (int i = 0; i < clist->num; i++) {
        waitpid(pids[i], &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) != OK) {
            return WEXITSTATUS(status);
        }
    }

    return OK;
}
int exec_local_cmd_loop() {
    char cmd_line[SH_CMD_MAX];
    command_list_t clist;
    int last_return_code = 0;

    while (1) {
        printf("%s", SH_PROMPT);
        if (fgets(cmd_line, SH_CMD_MAX, stdin) == NULL) {
            printf("\n");
            break;
        }

        cmd_line[strcspn(cmd_line, "\n")] = '\0';

        if (strlen(cmd_line) == 0) {
            continue;
        }

        if (strcmp(cmd_line, EXIT_CMD) == 0) {
            break;
        }

        if (strncmp(cmd_line, "cd", 2) == 0) {
            char *arg = cmd_line + 2;
            while (isspace(*arg)) arg++;
            if (strlen(arg) == 0) {
                continue;
            }

            if (chdir(arg) != 0) {
                perror("cd");
                last_return_code = errno;
            } else {
                last_return_code = 0;
            }
            continue;
        }

        if (strcmp(cmd_line, "dragon") == 0) {
            print_dragon();
            last_return_code = 0;
            continue;
        }

        if (strcmp(cmd_line, "rc") == 0) {
            printf("%d\n", last_return_code);
            continue;
        }

        if (build_cmd_list(cmd_line, &clist) != OK) {
            printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
            last_return_code = ERR_TOO_MANY_COMMANDS;
            continue;
        }

        last_return_code = execute_pipeline(&clist);

        free_cmd_list(&clist);
    }

    return last_return_code;
}