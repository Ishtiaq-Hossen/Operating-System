#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// Function to generate and print the Collatz sequence for a given positive integer
void collatz_sequence(int n) {
    printf("%d ", n);
    
    // Continue the sequence until n becomes 1
    while (n != 1) {
        if (n % 2 == 0) {
            n = n / 2;  // If n is even, divide it by 2
        } else {
            n = 3 * n + 1;  // If n is odd, multiply it by 3 and add 1
        }
        printf("%d ", n);  // Print the current value of n in the sequence
    }
    
    printf("\n");  // Print a newline character to end the sequence
}

int main(int argc, char *argv[]) {
    // Check if the correct number of command-line arguments is provided
    if (argc != 2) {
        printf("Usage: %s <positive_integer>\n", argv[0]);
        return 1;
    }

    // Convert the command-line argument to an integer
    int n = atoi(argv[1]);

    // Check if the provided integer is positive
    if (n <= 0) {
        printf("Please provide a positive integer.\n");
        return 1;
    }

    // Create a child process using fork()
    pid_t pid = fork();

    // Check if fork() failed
    if (pid < 0) {
        fprintf(stderr, "Fork failed\n");
        return 1;
    } else if (pid == 0) {  // Child process
        collatz_sequence(n);  // Call the collatz_sequence function in the child process
    } else {  // Parent process
        wait(NULL);  // Wait for the child process to complete before continuing
    }

    return 0;  // Return 0 to indicate successful execution
}
