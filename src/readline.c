#include "../inc/minishell.h"

int	main(int argc, char **argv)
{
	char *line = argv[0];
	(void)argc;
	while (line)
	{
		line = readline("minishell");
		if (!line)
			break;
		ft_printf("%s\n", line);
		free(line);
	}
	return (0);
}
