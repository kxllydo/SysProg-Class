#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */
 int build_cmd_list(char *cmd_line, command_list_t *clist) {
    memset(clist, 0, sizeof(command_list_t));
    if(strlen(cmd_line) == 0) {
        return WARN_NO_CMDS;
    }

    char cmd_buffer[SH_CMD_MAX];
    strcpy(cmd_buffer, cmd_line);

    char *save_ptr;
    char *cmd_segment = strtok_r(cmd_buffer, PIPE_STRING, &save_ptr);
    
    while (cmd_segment) {
        if (clist->num >= CMD_MAX) {
            return ERR_TOO_MANY_COMMANDS;
        }

        char trimmed_cmd[SH_CMD_MAX];
        strncpy(trimmed_cmd, cmd_segment, SH_CMD_MAX - 1);
        trimmed_cmd[SH_CMD_MAX - 1] = '\0';
        
        char *cmd_start = trimmed_cmd;
        while (isspace((unsigned char)*cmd_start)) cmd_start++;

        char *cmd_end = cmd_start + strlen(cmd_start) - 1;
        while (cmd_end > cmd_start && isspace((unsigned char)*cmd_end)) {
            *cmd_end = '\0';
            cmd_end--;
        }

        if (*cmd_start) {
            char *save_ptr2;
            char *cmd_name = strtok_r(cmd_start, " ", &save_ptr2);
            if (cmd_name) {
                if (strlen(cmd_name) >= EXE_MAX) {
                    return ERR_CMD_OR_ARGS_TOO_BIG;
                }
                strncpy(clist->commands[clist->num].exe, cmd_name, EXE_MAX - 1);
                
                char *cmd_args = strtok_r(NULL, "", &save_ptr2);
                if (cmd_args) {
                    while (isspace((unsigned char)*cmd_args)) cmd_args++;
                    if (strlen(cmd_args) >= ARG_MAX) {
                        return ERR_CMD_OR_ARGS_TOO_BIG;
                    }
                    strncpy(clist->commands[clist->num].args, cmd_args, ARG_MAX - 1);
                }
                clist->num++;
            }
        }
        cmd_segment = strtok_r(NULL, PIPE_STRING, &save_ptr);
    }

    if(clist->num == 0) {
        return WARN_NO_CMDS;
    }

    return OK;
}

