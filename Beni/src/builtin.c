/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 16:42:28 by bszilas           #+#    #+#             */
/*   Updated: 2024/08/16 09:40:13 by bszilas          ###   ########.fr       */
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

bool	check_n(t_node *list, int i)
{
	int	b;

	b = 1;
	if (!list->content[i] || !*list->content[i])
		return (false);
	if ((ft_strlen(list->content[i]) == 1)
		|| (ft_strncmp(list->content[i], "-", 2) == 0))
		return (false);
	while (list->content[i][b])
	{
		if (!(list->content[i][0] == '-'
			&& (ft_strncmp(&list->content[i][b], "n", 1) == 0
			|| ft_strncmp(&list->content[i][b], "'\0'", 1) == 0)))
			return (false);
		b++;
	}
	return (true);
}

void	command_echo(t_node *list)
{
	int	i;

	i = 1;
	if (check_n(list, i) == true)
	{
		while (check_n(list, i) == true)
			i++;
	}
	while (list->content[i] != NULL)
	{
		ft_printf("%s", list->content[i]);
		if (list->content[i + 1] != NULL)
			ft_printf(" ");
		i++;
	}
	if (check_n(list, 1) == false)
		ft_printf("\n");
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
