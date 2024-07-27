#include "minishell.h"

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