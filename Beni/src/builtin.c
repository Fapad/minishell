#include "minishell.h"

void   command_exit(t_var *var)
{
	(void)var;
    printf("exit\n");
    exit(0);
}

 void	command_cd(t_var *var)
{
		char	*path;

		path = var->tokens->right->str;
		if (*path == '\0')
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