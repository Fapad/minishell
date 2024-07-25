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
	int		cuc;

	i = 0;
	env = __environ;
	while (env[i])
		i++;
	cuc = 0;
	while (arg[cuc - 1] != ' ')
		cuc++;
	env[i] = arg + cuc;
	env[i + 1] = NULL;
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
void   command_exit(char	*line)
{
	exit(0);
}


void	command_unset(char *line)
{
	size_t	i;
	size_t	to_compare;
	size_t	start;
	char	**env = __environ;

	i = 0;
	to_compare = 0;
	while (line[i] != 32 && line[i])
		i++;
	while (line[i] == 32 && line)
		i++;
	start = i;
	while (line[i] > 32)
	{
		i++;
		to_compare++;
	}
	printf("Ez 1 proba\n");
	i = 0;
	while (env[i])
	{
		printf("Proba2\n");
		if (strncmp(env[i], line + start, to_compare) == 0)
		{
			printf("%s\n", env[i]);
			int b = i;
			//free(env[i]); // = env[i + 1];
			while (env[b])
			{
				env[b] = env[b + 1];
				b++;
			}
			printf("%s\n", env[b]);
			break ;
				
		}
		i++;
	}
	}



int	main(int argc, char **argv)
{
	int			flag;
	char		*cwd;		
		
	while (1)
	{
		char	*line = (0);readline("SMTH: ");
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
	else if (strcmp(line, "exit") == 0)
	{
			command_exit(line);
	}
	else if (strncmp(line, "unset", 5) == 0)
	{
			command_unset(line);
	}		
	}
	return (0);
}

