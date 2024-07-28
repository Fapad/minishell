#include "../inc/minishell.h"

void   command_exit(t_var *var)
{
	free_all(var);
	rl_clear_history();
	ft_printf("exit\n");
	exit(0);
}

 void	command_cd(t_var *var)
{
	char	*path;

	path = var->current->content[1];
	if (!path)
		path = getenv("HOME");
	chdir(path);
}

void	command_pwd(void)
{
	char	*cwd;
	char	*path;

	cwd = malloc(PATH_MAX);
	path = getcwd(cwd, PATH_MAX);
	printf("%s\n", path);
	if (path != NULL)
		free(path);
}

void	command_echo(t_node *list)
{
	int	i;

	i = 1;
	if (strcmp(list->content[1], "-n") == 0)
	{
		i++;
		while (list->content[i] != NULL)
		{
			ft_printf("%s", list->content[i]);
			if (list->content[i + 1] != NULL)
				ft_printf(" ");
			i++;
		}
		return ;
	}
	while (list->content[i] != NULL)
	{
		ft_printf("%s", list->content[i]);
		if (list->content[i + 1] != NULL)
			ft_printf(" ");
		i++;
	}
	ft_printf("\n");
}
