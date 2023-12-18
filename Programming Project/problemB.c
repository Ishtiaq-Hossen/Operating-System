
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_CHILDREN 3

int main() {
    int pipes[NUM_CHILDREN][2]; // Array of pipes for each child process
    pid_t child_pids[NUM_CHILDREN]; // Array to store child process IDs
    int i;

    // Create pipes for communication between parent and child processes
    for (i = 0; i < NUM_CHILDREN; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("Pipe creation failed");
            exit(EXIT_FAILURE);
        }
    }

    // Create child processes
    for (i = 0; i < NUM_CHILDREN; i++) {
        pid_t pid = fork();

        if (pid == -1) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        } else if (pid == 0) { // Child process
            close(pipes[i][0]); // Close reading end in child
            printf("Child %d (PID %d) ready to read:\n", i + 1, getpid());

            char c;
            char prev = '\0';
            while (read(STDIN_FILENO, &c, 1) > 0) {
                if (c == '\n' && prev == '\n') {
                    break; // Terminate if two consecutive newlines are received
                }
                write(pipes[i][1], &c, 1); // Send character to parent through pipe
                prev = c;
            }

            close(pipes[i][1]); // Close writing end in child
            exit(EXIT_SUCCESS);
        } else { // Parent process
            child_pids[i] = pid;
            close(pipes[i][1]); // Close writing end in parent
        }
    }

    // Parent process reads characters from pipes and outputs them
    for (i = 0; i < NUM_CHILDREN; i++) {
        char buffer;
        printf("Characters from Child %d (PID %d):\n", i + 1, child_pids[i]);
        while (read(pipes[i][0], &buffer, 1) > 0) {
            printf("%c", buffer);
        }
        printf("\n");
        close(pipes[i][0]); // Close reading end in parent
    }

    // Wait for all child processes to terminate
    for (i = 0; i < NUM_CHILDREN; i++) {
        waitpid(child_pids[i], NULL, 0);
    }

    return 0;
}

