/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_redirections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 17:39:22 by bszilas           #+#    #+#             */
/*   Updated: 2024/07/26 18:23:01 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void free_all(t_var *var)
{
	free_linked_lists(var);
	free_string_array(var->env);
}

void	in_redir_or_exit(t_var *var)
{
	t_node	*node;
	
	node = get_next_node(var->current, IN_R);
	if (!node)
		return ;
	while (node)
	{
		if (!node->content[FILENAME])
			return (free_all(var), perror("infile"), exit(EXIT_FAILURE));
		var->in_fd = open(node->content[FILENAME], O_RDONLY | O_CREAT, 0644);
		if (var->in_fd == -1)
		{
			perror(node->content[FILENAME]);
			return (free_all(var), exit(EXIT_FAILURE));
		}
		node = get_next_node(node, IN_R);
		if (node)
			close(var->in_fd);
	}
}

void	out_redir_or_exit(t_var *var)
{
	t_node	*node;
	
	node = get_next_node(var->current, OUT_R | OUT_APPEND);
	if (!node)
		return ;
	while (node)
	{
		var->out_fd = open(node->content[FILENAME], O_WRONLY | node->type);
		if (var->out_fd == -1)
		{
			perror(node->content[FILENAME]);
			return (free_all(var), exit(EXIT_FAILURE));
		}
		node = get_next_node(node, OUT_R | OUT_APPEND);
		if (node)
			close(var->out_fd);
	}
}

int	out_redir_never_exit(t_var *var)
{
	t_node	*node;
	
	var->status = 0;
	var->out_fd = STDOUT_FILENO;
	node = get_next_node(var->list, OUT_R | OUT_APPEND);
	if (!node)
		return (EXIT_FAILURE);
	while (node)
	{
		var->out_fd = open(node->content[FILENAME], O_WRONLY | node->type);
		if (var->out_fd == -1)
			return (perror(node->content[FILENAME]), EXIT_FAILURE);
		node = get_next_node(node, IN_R);
		if (node)
			close(var->out_fd);
	}
	return (EXIT_SUCCESS);
}

int	in_redir_never_exit(t_var *var)
{
	t_node	*node;
	
	var->status = 0;
	var->in_fd = STDIN_FILENO;
	node = get_next_node(var->list, IN_R);
	if (!node)
		return (EXIT_FAILURE);
	while (node)
	{
		if (!node->content[FILENAME])
			return (perror("infile"), EXIT_FAILURE);
		var->in_fd = open(node->content[FILENAME], O_RDONLY);
		if (var->in_fd == -1)
			return (perror(node->content[FILENAME]), EXIT_FAILURE);
		node = get_next_node(node, IN_R);
		if (node)
			close(var->in_fd);
	}
	return (EXIT_SUCCESS);
}
