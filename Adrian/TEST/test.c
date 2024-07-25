/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajovanov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 08:57:49 by ajovanov          #+#    #+#             */
/*   Updated: 2024/07/25 10:55:18 by ajovanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
# define PATH_MAX 4096
#include <readline/readline.h>
#include <readline/history.h>



void	command_export(char	*arg)
{
	char	**env;
	int		i;

	i = 0;
	env = __environ;
	while (env[i])
		i++;
	env[i] = arg;
	env[i + 1] = "asd";
	env[i+ 2] = "kukac";
	env[i + 3] = NULL;
}



void	command_env(void)
{
	int	i;
	char	**env;

	i = 0;
	env = __environ;
	while (env[i])
	{
		printf("%s\n", env[i]);
		i++;
	}
}

void	command_pwd(int	*flag)
{
	size_t	size;
	char	*cwd;
	char	*path;

	cwd = malloc(PATH_MAX);
	path = getcwd(cwd, PATH_MAX);
	printf("%s\n", path);
	if (path != NULL)
		free(path);
}

int	main(int argc, char **argv)
{
	int			flag;
	char		*cwd;		
		
	while (1)
	{
		char	*line = readline("SMTH: ");
	//printf("%s\n", line);
	if (strcmp(line, "pwd") == 0)
	{
			command_pwd(&flag);
	}
	else if(strcmp(line, "env") == 0)
	{
              command_env();
	}
	else if (strncmp(line, "EXPORT", 6) == 0)
	{
	 		command_export(line);
	}		
	}
	return (0);
}

