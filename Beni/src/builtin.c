/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 16:42:28 by bszilas           #+#    #+#             */
/*   Updated: 2024/07/30 21:49:44 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void   command_exit(t_var *var)
{
	free_all(var);
	rl_clear_history();
	ft_printf("exit\n");
	exit(EXIT_SUCCESS);
}

 void	command_cd(t_var *var)
{
	char	*path;

	path = var->current->content[1];
	if (!path)
		path = ft_getenv(var->env, "HOME");
	chdir(path);
}

void	command_pwd(t_var *var)
{
	char	*cwd;
	char	*path;

	cwd = malloc(PATH_MAX);
	path = getcwd(cwd, PATH_MAX);
	if (!path)
		return (perror("pwd"), free(cwd), free_all(var), exit(EXIT_FAILURE));
	ft_printf("%s\n", path);
	free(path);
}

void	command_echo(t_node *list)
{
	int	i;

	i = 1;
	if (ft_strncmp(list->content[1], "-n", 3) == 0)
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
