#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int handle_redirect(char *args[]);
int piping(char *args[]);
int amount_pipes(char *args[], char target[]);

int main(int argc, char *argv[]) {
    int interactive = 1;

    // for scripting
    if (argc > 1) {
        //first arg is the name of the script
        if (freopen(argv[1], "r", stdin) == NULL) {
            fprintf(stderr, "Can't read from script file %s\n", argv[1]);
            fprintf(stderr, "Exiting.\n");
            exit(1);
        }
        interactive = 0;
    }


    while (1) {
        //string to hold input buffer
        char buf[1024];

        // Prompt
        if (interactive)
            printf("$ ");

        //get input from keyboard
        if (fgets(buf, 1024, stdin) == NULL) break; // added the null check because without it it would print \n forever if scripting

        // trim newline
        char *nl = strchr(buf, '\n'); //gives a pointer to the newline
        if (nl) *nl = '\0'; //replaces that pointer's value with '\0' if found
                            
        //remove comments (also allows for shebangs to work because of how execvp works and then comment removal remove the shebang right before execution)
        char *hash = strchr(buf, '#');
        if (hash) *hash = '\0';

        // split buf into args
        char *args[20]; // watch out for overflowing buffer if im to continue this project
        int nargs = 0;

        args[nargs] = strtok(buf, " ");
        while (args[nargs] != NULL) {
            args[++nargs] = strtok(NULL, " "); // NULL here due to the way strtok works. First instance needs the string, subsequent using the same input need to be passed null. See man strtok
        }

        if (args[0] == NULL) continue;
        if (strcmp(args[0], "exit") == 0) exit(0); //when running a command that does not exist right before, you need to exit twice. Probably due to the fork still running or something, idk.
        //fork and execute
        pid_t pid = fork();

        if (pid > 0) {
            // parent
            wait(NULL);
        }
        else {
            // child

            if (handle_redirect(args) == -1) {
                fprintf(stderr, "Could not redirect\n");
                exit(1);
            }
            // if (handle_outdirect(args) == -1) {
            //     fprintf(stderr, "Could not outdirect\n");
            // }

            execvp(args[0], args);

            fprintf(stderr, "Could not exec %s\n", buf);
        }
    }
    return 0;
}


// stdin same but < and open in read and replace stdin
// Handle redirect stdout
// Return 1 if succ
// Return 0 if no redirect necesasry
// Return -1 if something broke
int handle_redirect(char *args[]) {
    for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], ">") == 0) {
            //assuming next arg is always the file to write to
            if ( freopen(args[i+1], "w", stdout) == NULL ) return -1; //reopen to replace stdout file descriptor with this new file. Also replaces abstraction layer on top out stdout
                                                                      //or use open but a lot more work, but probably better

            // Terminate args at the ">" symbol
            args[i] = NULL;
            return 1;
        }
        else if (strcmp(args[i], "<") == 0) { // stdin version done
            //assuming next arg is always the file to write to
            if ( freopen(args[i+1], "r", stdin) == NULL ) return -1; //reopen to replace stdout file descriptor with this new file. Also replaces abstraction layer on top out stdout
                                                                      //or use open but a lot more work, but probably better

            // Terminate args at the "<" symbol
            args[i] = NULL;
            return 1;
        }

    }
    return 0;
}

// handle piping
int piping(char *args[]) {
    for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], "|") == 0) {
            //if a pipe, do something
            //feel like there is a better way to do this, once for all variations
            //also gotta think of how to do it for multiple pipes
        }
    }
}

// count amount of 'target' in an array (for pipe)
int amount_pipes(char *args[], char target[]) {
    int count = 0;

    for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], "|") != NULL) count++;
    }

    return count;
}
