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

void	in_open_or_exit(t_var *var)
{
	t_node	*node;
	
	node = get_next_node(var->current, IN_R, PIPE | END);
	if (!node)
		return ;
	while (node)
	{
		if (!node->content[FILENAME])
			return (free_all(var), perror("infile"), exit(EXIT_FAILURE));
		var->in_fd = open(node->content[FILENAME], O_RDONLY);
		if (var->in_fd == -1)
		{
			perror(node->content[FILENAME]);
			return (free_all(var), close(var->pfd[READ_END]), exit(EXIT_FAILURE));
		}
		node = get_next_node(node->next, IN_R, PIPE | END);
		if (node)
			close(var->in_fd);
	}
}

void	out_open_or_exit(t_var *var)
{
	t_node	*node;
	
	node = get_next_node(var->current, OUT_R | OUT_APPEND, PIPE | END);
	if (!node)
		return ;
	while (node)
	{
		var->out_fd = open(node->content[FILENAME], O_WRONLY | O_CREAT | node->type, 0644);
		if (var->out_fd == -1)
		{
			perror(node->content[FILENAME]);
			return (free_all(var), close(var->pfd[WRITE_END]), exit(EXIT_FAILURE));
		}
		node = get_next_node(node->next, OUT_R | OUT_APPEND, PIPE | END);
		if (node)
			close(var->out_fd);
	}
}

int	out_open_return_status(t_var *var)
{
	t_node	*node;
	
	node = get_next_node(var->list, OUT_R | OUT_APPEND, PIPE | END);
	if (!node)
		return (EXIT_SUCCESS);
	while (node)
	{
		var->out_fd = open(node->content[FILENAME], O_WRONLY | O_CREAT | node->type, 0644);
		if (var->out_fd == -1)
			return (perror(node->content[FILENAME]), EXIT_FAILURE);
		node = get_next_node(node->next, OUT_R | OUT_APPEND, PIPE | END);
		if (node)
			close(var->out_fd);
	}
	return (EXIT_SUCCESS);
}

int	in_open_return_status(t_var *var)
{
	t_node	*node;
	
	node = get_next_node(var->list, IN_R, PIPE | END);
	if (!node)
		return (EXIT_SUCCESS);
	while (node)
	{
		if (!node->content[FILENAME])
			return (perror("infile"), EXIT_FAILURE);
		var->in_fd = open(node->content[FILENAME], O_RDONLY);
		if (var->in_fd == -1)
			return (perror(node->content[FILENAME]), EXIT_FAILURE);
		node = get_next_node(node->next, IN_R, PIPE | END);
		if (node)
			close(var->in_fd);
	}
	return (EXIT_SUCCESS);
}

void	file_redirect(t_var *var)
{
	char c = var->i + '0';
	if (var->in_fd == -1 || var->out_fd == -1)
		return (free_all(var), exit(EXIT_FAILURE));
	if (dup2(var->in_fd, STDIN_FILENO) == -1)
		return (perror("infile: dup2"), ft_putchar_fd(c, 2), ft_putchar_fd('\n', 2), free_all(var), exit(EXIT_FAILURE));
	if (dup2(var->out_fd, STDOUT_FILENO) == -1)
		return (perror("outfile: dup2"), ft_putchar_fd(c, 2), ft_putchar_fd('\n', 2), free_all(var), exit(EXIT_FAILURE));
	(close_in_and_out(var));
}