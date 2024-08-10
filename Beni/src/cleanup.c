/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 10:18:55 by bszilas           #+#    #+#             */
/*   Updated: 2024/08/10 16:36:53 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	remove_heredocs(t_var *var)
{
	t_node	*doc;

	doc = get_next_node(var->list, HEREDOC, END);
	while (doc)
	{
		if (doc->content[FILENAME])
			unlink(doc->content[FILENAME]);
		doc = get_next_node(doc->next, HEREDOC, END);
	}
}

void	exec_cleanup(t_var *var)
{
	check_received_signal(var);
	remove_heredocs(var);
	free_lists_and_path(var);
	free(var->line);
	var->line = NULL;
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

void	child_execve_error_handler(t_var *var)
{
	signal(SIGQUIT | SIGINT, SIG_IGN);
	perror(var->exec_cmd);
	free(var->exec_cmd);
	set_status(var);
	free_all(var);
	exit(var->status);
}