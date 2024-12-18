#include <stdio.h>
#include <stdlib.h>

// Main function: argc = argument count, argv = argument vector (array of strings)
int main(int argc, char *argv[])
{
    // Check if the correct number of arguments is provided
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <server> <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Extract server and file arguments
    char *server = argv[1];
    char *file = argv[2];

    // Print the arguments (confirm they're received correctly)
    printf("Server Address: %s\n", server);
    printf("File Name: %s\n", file);

    // This is where you would add further TFTP functionality
    return 0;
}
