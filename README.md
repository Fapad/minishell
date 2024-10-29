This is a school project building a shell in a team of two.

## run with docker:

```bash
git clone git@github.com:Fapad/minishelllllllll.git minishell
cd minishell
docker build -t minishell .
docker run --it --rm minishell
```

## run using make:
```bash
git clone git@github.com:Fapad/minishelllllllll.git minishell
cd minishell
sudo apt-get update -y
sudo apt-get install -y build-essential readline-common libreadline-dev libreadline7
make
./minishell
```

### run with valgrind:
```bash
make valgrind
```

## requirements for this school project:
Any implemented feature and its error handling should behave like in bash.

• Display a prompt when waiting for a new command.

• Have a working history.

• Search and launch the right executable (based on the PATH variable or using a
relative or an absolute path).

• Handle ’ (single quote) which should prevent the shell from interpreting the meta-
characters in the quoted sequence.

• Handle " (double quote) which should prevent the shell from interpreting the meta-
characters in the quoted sequence except for $ (dollar sign).

• Handle environment variables ($ followed by a sequence of characters) which
should expand to their values.

• Handle $? which should expand to the exit status of the most recently executed
foreground pipeline.

• Not interpret unclosed quotes or special characters which are not required by the
subject such as \ (backslash) or ; (semicolon).

### redirections:

◦ < should redirect input.

◦ > should redirect output.

◦ << should be given a delimiter, then read the input until a line containing the
delimiter is seen. However, it doesn’t have to update the history!

◦ >> should redirect output in append mode.

• Implement pipes (| character). The output of each command in the pipeline is
connected to the input of the next command via a pipe.

### builtins:

◦ echo with option -n

◦ cd with only a relative or absolute path

◦ pwd with no options

◦ export with or without arguments

◦ unset with one or more arguments

◦ env with no options or arguments

◦ exit with one argument and no options

### signal handling

• Handle ctrl-C, ctrl-D and ctrl-\ which should behave like in bash.

• In interactive mode:

◦ ctrl-C displays a new prompt on a new line.

◦ ctrl-D exits the shell.

◦ ctrl-\ does nothing.

• Avoid using more than one global variable to indicate a received signal. Consider
the implications: this approach ensures that your signal handler will not access your
main data structures.


#### P.S.
As this is a full Hungarian teamwork, the prompt is colored red-white-green as the Hungarian tricolore. 

Its not Italian, that would be green-white-red :)
