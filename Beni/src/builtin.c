/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 16:42:28 by bszilas           #+#    #+#             */
/*   Updated: 2024/08/13 19:33:31 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	command_exit(t_var *var)
{
	var->loop = false;
	if (!var->list->content[1])
	{
		var->status = var->last_status;
		return ;
	}
	var->status = get_shlvl(var, var->list->content[1]);
	if ((var->status == 0 || var->overflow) \
	&& (ft_strncmp("0", var->list->content[1], 2) && \
	ft_strncmp("-9223372036854775808", var->list->content[1], 21)))
		return (error_msg(var, ": numeric argument required", 2));
	if (var->list->content[2])
	{
		var->loop = true;
		return (error_msg(var, ": too many arguments", 1));
	}
	else if ((var->status > 255 || var->status < 0))
		var->status = var->status % 256;
}

void	command_pwd(t_var *var)
{
	char	*cwd;

	cwd = getcwd(NULL, PATH_MAX);
	if (!cwd)
		ft_putendl_fd(var->cwd, STDOUT_FILENO);
	else
		ft_printf("%s\n", cwd);
	free(cwd);
}

void	command_echo_util(t_node *list, int i)
{
	while (list->content[i] != NULL)
	{
		ft_printf("%s", list->content[i]);
		if (list->content[i + 1] != NULL)
			ft_printf(" ");
		i++;
	}
	ft_printf("\n");
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
			if (ft_strncmp(list->content[i], "-n", 3) != 0
				&& list->content[i][2] != 'n')
				ft_printf("%s", list->content[i]);
			if (list->content[i + 1] != NULL
				&& ft_strncmp(list->content[i], "-n", 3) != 0
				&& list->content[i][2] != 'n')
				ft_printf(" ");
			i++;
		}
		return ;
	}
	command_echo_util(list, i);
}

void	cd_home(t_var *var, char *path)
{
	path = ft_getenv(var->env, "HOME");
	if (!path)
	{
		ft_putendl_fd("cd: HOME not set", STDERR_FILENO);
		return (status_1(var));
	}
	command_cd(var, path);
}
