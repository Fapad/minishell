/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 12:13:32 by bszilas           #+#    #+#             */
/*   Updated: 2024/07/31 20:51:07 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	count_node_types(t_node *node, int type)
{
	int	n;

	n = 0;
	while (node)
	{
		if (node->type & type)
			n++;
		node = node->next;
	}
	return (n);
}

t_node	*get_next_node(t_node *node, int get_type, int before_type)
{
	while (node && !(node->type & before_type))
	{
		if (node->type & get_type)
			return (node);
		node = node->next;
	}
	return (NULL);
}

void	free_lists_and_path(t_var *var)
{
	free_linked_lists(var);
	free_string_array(var->splitted_path);
	var->splitted_path = NULL;
}

void	free_all(t_var *var)
{
	free_string_array(var->env);
	var->env = NULL;
	free_lists_and_path(var);
	free(var->line);
	var->line = NULL;
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