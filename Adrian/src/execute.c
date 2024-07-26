/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 19:46:32 by bszilas           #+#    #+#             */
/*   Updated: 2024/07/25 21:03:47 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	execute(t_var *var)
{
	t_node	*node;
	pid_t	pid;

	write_here_docs(var);
	if (no_pipes(var->list))
	{
		if (cd_export_exit_or_unset(var))
			return ;
		else
		pid = fork();
		if (pid == 0)
		{
			redir_or_exit();
			exec_builtin();
		}
		return ;
	}
	var->cmds = count_commands(var->list);
	node = var->list;
	while (var->cmds--)
	{
		if (var->cmds && pipe(var->pfd) == -1)
			return (free_everything(), exit(EXIT_FAILURE));
		pid = fork();
		if (pid == 0)
		{
			redir_or_exit();
			exec_builtin();
		}
		if (var->cmds)
			close_pipe();
		go_to_next_cmd(node);
	}
	wait_children(var);
}