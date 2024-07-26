#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>

void	command_cd(char *line)
{
		char	*path;

		path = line + 3;
		while (*path == ' ')
			path++;
		if (*path == '\0')
			path = getenv("HOME");
		chdir(path);

}

int	main(void)
{
		char	*cwd;
		char *firsttest = malloc(30);
		cwd = malloc(30);
		firsttest = getcwd(cwd, 30);
		printf("first: %s\n", firsttest);

		while (1)
		{
			char	*line = readline("VALAMI->");
			if (strncmp(line, "cd", 2) == 0)
			{
				command_cd(line);
			}
			char *cwd2 = malloc(40);
			char *secoundtest = malloc(40);
			secoundtest = getcwd(cwd2, 40);
			printf("secound: %s\n", secoundtest);
		}
		return (0);
}
