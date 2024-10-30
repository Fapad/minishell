This is a school project building a shell in a team of two.

## clone the repo
```bash
git clone git@github.com:Fapad/minishelllllllll.git minishell
cd minishell
```

## run with docker:

```bash
docker build -t minishell .
docker run --it --rm minishell
```

## or run using make:
```bash
sudo apt-get update -y
sudo apt-get install -y build-essential readline-common libreadline-dev
make
./minishell
```

## requirements for this school project:

launching the right executable from the environment PATH variable or from a relative or absolute path

’ (single quote) " (double quote) and enviroment variable expansion $ (dollar sign), also show the exit status of the last command ($?).

### builtins:

◦ echo with option -n

◦ cd with only a relative or absolute path

◦ pwd with no options

◦ export with or without arguments

◦ unset with one or more arguments

◦ env with no options or arguments

◦ exit with one argument and no options

### redirections:

infile <, outfile > or >> and << heredoc is handled like in bash

any number of pipes | can be handled

### signal handling

keyboard generated signals SIGINT and SIGQUIT are handled like in bash, as well as Ctrl+D
