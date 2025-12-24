#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_CMD_LEN 1024
#define MAX_ARGS 64
#define HISTORY_COUNT 10 // Store last 10 commands


char history[HISTORY_COUNT][MAX_CMD_LEN];
int history_index = 0;
int total_commands = 0;


void add_to_history(char *line) {
    if (line == NULL || strlen(line) == 0) return;
    
    // Copy the command into the current slot
    strncpy(history[history_index], line, MAX_CMD_LEN);
    
    // Move index to next slot (wrap around using modulo)
    history_index = (history_index + 1) % HISTORY_COUNT;
    total_commands++;
}

// Function to print history
void print_history() {
    int start = (total_commands > HISTORY_COUNT) ? history_index : 0;
    int count = (total_commands > HISTORY_COUNT) ? HISTORY_COUNT : total_commands;

    for (int i = 0; i < count; i++) {
        int idx = (start + i) % HISTORY_COUNT;
        printf("%d  %s\n", i + 1, history[idx]);
    }
}

void read_line(char *buffer) {
    if (fgets(buffer, MAX_CMD_LEN, stdin) == NULL) {
        printf("\n");
        exit(0);
    }
    buffer[strcspn(buffer, "\n")] = 0;
}

void parse_line(char *line, char **args) {
    int i = 0;
    char *token = strtok(line, " \t");
    while (token != NULL && i < MAX_ARGS - 1) {
        args[i] = token;
        i++;
        token = strtok(NULL, " \t");
    }
    args[i] = NULL;
}

void execute_command(char **args) {
    char *outfile = NULL;
    for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], ">") == 0) {
            outfile = args[i + 1];
            args[i] = NULL;
            break;
        }
    }

    pid_t pid = fork();
    if (pid == 0) {
        if (outfile) {
            int fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) { perror("open"); exit(1); }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        if (execvp(args[0], args) == -1) perror("minishell");
        exit(1);
    } else if (pid > 0) {
        wait(NULL);
    }
}

int main() {
    char line[MAX_CMD_LEN];
    char line_copy[MAX_CMD_LEN];
    char *args[MAX_ARGS];

    while (1) {
        printf("\033[1;32mminishell-wsl>\033[0m ");
        read_line(line);

        if (strlen(line) == 0) continue;

        // 1. Add to history before we destroy the string with strtok
        add_to_history(line);
        strcpy(line_copy, line); // Copy for parsing

        parse_line(line_copy, args);

        if (args[0] == NULL) continue;
        if (strcmp(args[0], "exit") == 0) break;

        // 2. Built-in: history
        if (strcmp(args[0], "history") == 0) {
            print_history();
            continue;
        }

        if (strcmp(args[0], "cd") == 0) {
            if (args[1] == NULL || chdir(args[1]) != 0) perror("minishell");
            continue;
        }

        execute_command(args);
    }
    return 0;
}