/****************
LE2: Introduction to Unnamed Pipes
****************/
#include <unistd.h> // pipe, fork, dup2, execvp, close

using namespace std;

int main () {
    // lists all the files in the root directory in the long format
    char* cmd1[] = {(char*) "ls", (char*) "-al", (char*) "/", nullptr};
    // translates all input from lowercase to uppercase
    char* cmd2[] = {(char*) "tr", (char*) "a-z", (char*) "A-Z", nullptr};

    // Save original STDIN and STDOUT commands
    dup(0); // STDIN
    dup(1); // STDOUT

    // TODO: add functionality
    // Create pipe
    int pipefd[2];
    pid_t cpid;

    pipe(pipefd); // look at what parameters it needs & what it returns

    // Create child to run first command
    cpid = fork();

    // In child, redirect output to write end of pipe
    if (cpid == 0){
        dup2(pipefd[1], 1); // changing the STDOUT to write end of pipe
        
        // Close the read end of the pipe on the child side
        close(pipefd[0]);

        // In child, execute the command
        execvp(cmd1[0], cmd1); // ls
    }

   

    // Create another child to run second command
    cpid = fork();

    // In child, redirect input to the read end of the pipe
    if (cpid == 0){        
        dup2(pipefd[0], 0); // changing STDIN to read end of pipe

        // Close the write end of the pipe on the child side.
        close(pipefd[1]);

        // Execute the second command.
        execvp(cmd2[0], cmd2); // tr

    }
    
    // Reset the input and output file descriptors of the parent.
    // Overwrite in/out w/ what was saved
    dup2(0, 3); // resetting STDIN
    dup2(1, 4); // resetting STDOUT
}
