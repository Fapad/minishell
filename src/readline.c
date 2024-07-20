#include "../inc/minishell.h"

int	main(void)
{
	char *line;
	while (1)
	{
		line = readline("\033[1;32mminishell>\033[0m ");
		if (!line)
			break;
		add_history(line);
		free(line);
	}
	return (0);
}