/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_openections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 17:39:22 by bszilas           #+#    #+#             */
/*   Updated: 2024/07/27 12:49:09 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	open_files_or_exit(t_var *var)
{
	t_node	*node;

	node = get_next_node(var->current, \
	IN_R | OUT_R | OUT_APPEND | HEREDOC, PIPE | END);
	while (node)
	{
		if (node->type & (HEREDOC | IN_R))
			var->in_fd = open(node->content[FILENAME], O_RDONLY);
		else if (node->type & (OUT_R | OUT_APPEND))
			var->out_fd = open(node->content[FILENAME], \
			O_WRONLY | O_CREAT | node->type, 0644);
		if (var->in_fd == -1 || var->out_fd == -1)
		{
			perror(node->content[FILENAME]);
			free_all(var);
			exit(EXIT_FAILURE);
		}
		node = get_next_node(node->next, \
		IN_R | OUT_R | OUT_APPEND | HEREDOC, PIPE | END);
		if (node)
			close_in_and_out(var);
	}
}

int	open_files_in_parent(t_var *var)
{
	t_node	*node;
	int		fd;
	
	node = get_next_node(var->list, IN_R | OUT_R | OUT_APPEND, END);
	if (!node)
		return (true);
	fd = INT_MAX;
	while (node)
	{
		if (node->type == IN_R)
			fd = open(node->content[FILENAME], O_RDONLY);
		else if (node->type & (OUT_R | OUT_APPEND))
			fd = open(node->content[1], O_WRONLY | O_CREAT | node->type, 0644);
		if (fd == -1)
			return (perror(node->content[FILENAME]), false);
		close(fd);
		node = get_next_node(node->next, IN_R | OUT_R | OUT_APPEND, END);
	}
	return (true);
}

void	file_redirect(t_var *var)
{
	if (dup2(var->in_fd, STDIN_FILENO) == -1)
	{
		perror("infile: dup2");
		free_all(var);
		exit(EXIT_FAILURE);
	}
	if (dup2(var->out_fd, STDOUT_FILENO) == -1)
	{
		perror("outfile: dup2");
		free_all(var);
		exit(EXIT_FAILURE);
	}
	close_in_and_out(var);
}