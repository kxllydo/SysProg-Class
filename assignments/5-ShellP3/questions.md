1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

In my implementation, after forking child processes for each command in the pipeline, the parent process calls waitpid() for each child process to wait for their completion. This ensures that the shell does not proceed to accept new input until all child processes in the pipeline have finished executing. If waitpid() was not called, it would cause zombie processes and resource leaks.

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

It is necessary to close unused pipe ends after calling dup2() to avoid resource leaks and ensure proper pipeline behavior. If you leave pipes open, it can cause resource leaks, deadlock, and unexecpted behavior, making the program not work as intended.

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

The cd command is implemented as a built-in command because it needs to modify the current working directory of the shell process. If cd were implemented as an external command, it would change the directory for the child process but not for the shell, making it ineffective. Furthermore, Users would need to manually synchronize the shell’s working directory with the external cd command which can lead to many errors.

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

To allow an arbitrary number of piped commands, I could dynamically allocate memory for the command list and pipe file descriptors. I would have to sacrafice simplicity, memory overhead, and possibly performance.