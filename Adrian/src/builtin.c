/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 16:42:28 by bszilas           #+#    #+#             */
/*   Updated: 2024/08/08 16:03:52 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void   command_exit(t_var *var)
{
	int	exit_status;
	
	exit_status = 0;
	var->loop = false;
	if (var->list->content[1] != NULL)
		exit_status = ft_atoi(var->list->content[1]);
	if (exit_status == 0 && var->list->content[1] == NULL)
		var->status = exit_status;
	else if (exit_status == 0 && var->list->content[2] == NULL)
		error_msg(var, ": numeric argument required", 2);
	else if (exit_status == 0 && (ft_isalnum(var->list->content[2][0])))
		error_msg(var, ": numeric argument required", 2);
	else if (exit_status >= 0 && exit_status <= 255 && var->list->content[2] == NULL)
		var->status = exit_status;
	else if ((exit_status > 255 || exit_status < 0) && var->list->content[2] == NULL)
		var->status = exit_status % 256;
	else if (exit_status != 0 && (ft_isalnum(var->list->content[2][0])))
		error_msg(var, ": too many arguments", 1);	
	
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
