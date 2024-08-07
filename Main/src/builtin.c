/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 16:42:28 by bszilas           #+#    #+#             */
/*   Updated: 2024/08/01 19:44:42 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void   command_exit(t_var *var)
{
	int	exit_status;
	
	var->loop = false;

	exit_status = ft_atoi(var->list->content[1]);
	if (exit_status == 0 && var->list->content[2] == NULL)
	{
		ft_putendl_fd(" numeric argument required", STDERR_FILENO);
		var->status = 2;
	}
	else if (exit_status == 0 && (var->list->content[2][0] >= 'a' && var->list->content[2][0] <= 'z'))
	{
		ft_putendl_fd(" numeric argument required", STDERR_FILENO);
		var->status = 2;
	}
	
	else if (exit_status >= 0 && exit_status <= 255 && var->list->content[2] == NULL)
		var->status = exit_status;
	else if ((exit_status > 255 || exit_status < 0) && var->list->content[2] == NULL)
		var->status = exit_status % 256;
	else if (exit_status != 0 && (var->list->content[2][0] >= 'a' && var->list->content[2][0] <= 'z'))
	{
		ft_putendl_fd(" too many arguments", STDERR_FILENO);
		var->status = 1;
	}
	else if (exit_status != 0 && var->list->content[2] != NULL)
		var->status = 2;
	
	
	
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
