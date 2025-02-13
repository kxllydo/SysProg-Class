1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**:  fgets() was a good choice for this application because it can detect the end of input, prevents buffer overflow, and handles spaces and empty input.

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**:  malloc() allows flexibility in memory management and makes it easy to modify values.


3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**:  If we don't trim white space or leading spaces, we could have issues when parsing the command. Some issues that we might experience is incorrect argument handling. For instance, if we had "cd directory ", it would store "directory " with 
    the white space which would cause unexpected issues. Furthermore, piped commands would experience some errors with extra spaces.

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**:  >, <, and >> are three examples of redirection that we should implement in our shell. The first one is redirecting stdout to a file. 
    This one would be challenging to implement because we would have to handle file creation and error handling. < takes input from a file into a function. Some hurdles we may face with implementing this
    is parsing through the file and error handling. Lastly, implementing >> might be difficult because we have to handle overwriting vs appending.
r
- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**:  Redirection redirects STDIN, STDOUT, or STDERR to or from a file. Meanwhile, piping connects the STDOUT of one command to the STDIN of another.

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**: Keeping STDERR and STDOUT is important because it makes it easier to distinguish between a result vs. a failure, making 
    your code easier to debug. Also, it ensures that piping only occurs when valid data (STDOUT) is passed between commands.

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**:  Our custom shell should display errors seperately to ensure that error messages do not interfere with expected results.
    We should provide a way to merge them so they have simplified logging and more user control. 
    We would implement this by doing command 2>&1 where 2 represents STDERR and 1 is STDOUT
