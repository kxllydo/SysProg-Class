1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**: We use fork/execvp instead of calling execvp directly because fork() creates a new process and execvp() replaces the current process with the command. This allows the shell to execute commands without halting its own operations.

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  If fork() failes, it returns -1. My implementation handles this scenario by returning the value of fork() which prevents the shell from crashing.

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**: execvp() searches for a command in directories in the PATH environment variable.

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**:  Calling wait() is significant because it makes the parent process wait for the child process to finish executing. This prevents the child process from occupying a process table entry even though they are done executing.

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**:  WEXITSTATUS() isolates the exit statues of a child process from the status code returned from wait(). It returns an exit status to the operating system which the parent can retrieve using wait() or waitpid().

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**: My build_cmd_buff() handles quoted arguments by checking for " or ' in the input. This was necessary so that the program was able to identify the quoted entity as one argument, not as two.

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**:  In this assignment, I made sure to account for quoted arguments and also leading, trailing, and consecutive spaces. I think the most challening part about refactoring my old code was getting my program to the point where I could compile it and test it extensively. Since everything is so cohesive, I had to make sure I was done one process before moving onto another.

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**:  Signals are a form of asynchronus communication in a Linux system and are used to notify a process that an event has occured. Signals are less structured compared to IPC.

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**:  SIGKILL(9) - immediately terminates a process, SIGTERM(15) - asks a process to terminate gracefully, SIGINT(2) - signal sent when the user tries to CTRL+C in the terminal.

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  SIGSTOP is a signal that causes a process to stop (pause) its execution. SIGSTOP cannot be caught or ignored by a process. This is because it is a low level signal meant to control process execution without interference.
