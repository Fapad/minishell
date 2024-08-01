/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 10:18:55 by bszilas           #+#    #+#             */
/*   Updated: 2024/08/01 13:46:28 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	free_lists_and_path(t_var *var)
{
	free_linked_lists(var);
	free_string_array(var->splitted_path);
	var->splitted_path = NULL;
}

void	exec_cleanup(t_var *var)
{
	free_lists_and_path(var);
	free(var->line);
	var->line = NULL;
}

void	free_all(t_var *var)
{
	free_lists_and_path(var);
	free_string_array(var->env);
	var->env = NULL;
	free(var->cwd);
	var->cwd = NULL;
}

void	close_in_and_out(t_var *var)
{
	if (var->in_fd != STDIN_FILENO && var->in_fd != -1)
		close(var->in_fd);
	if (var->out_fd != STDOUT_FILENO && var->out_fd != -1)
		close(var->out_fd);
}

void	close_pipe(int pfd[])
{
	close(pfd[WRITE_END]);
	close(pfd[READ_END]);
}

