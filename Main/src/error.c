/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 09:21:33 by bszilas           #+#    #+#             */
/*   Updated: 2024/08/03 13:53:54 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	status_1(t_var *var)
{
	var->status = EXIT_FAILURE;
}

void	status_2(t_var *var)
{
	var->status = EXIT_FAILURE << 1;
}

void	restore_environment(t_var *var)
{
	perror("Could not change environment");
	malloc_envps(var, var->stack_env);
	ft_putendl_fd("Environment has been reset", STDERR_FILENO);
}

void	set_status(t_var *var)
{
	if (errno == 13)
		var->status = 126;
	else if (errno == 2)
		var->status = 127;
	else
		status_1(var);
}

void	error_exec_txt_file(t_var *var)
{
	ft_putendl_fd("Minishell has no script support", STDERR_FILENO);
	free(var->exec_cmd);
	var->exec_cmd = NULL;
	status_2(var);
}

void	command_not_found(t_var *var)
{
	ft_putstr_fd(var->current->content[0], STDERR_FILENO);
	ft_putendl_fd(": command not found", STDERR_FILENO);
	set_status(var);
}