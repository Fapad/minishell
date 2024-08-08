/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 12:05:27 by bszilas           #+#    #+#             */
/*   Updated: 2024/08/07 17:14:44 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_node	*new_command_node(t_token **current, t_node *this)
{
	t_token	*tmp;
	int		arg_count;
	int		i;

	arg_count = token_arg_count(*current);
	this->content = malloc((arg_count + 1) * sizeof (char *));
	if (!this->content)
	{
		free(this);
		return (NULL);
	}
	i = 0;
	tmp = (*current);
	this->content[i++] = tmp->str;
	while (i < arg_count)
	{
		this->content[i++] = find_next_cmd_token(tmp)->str;
		if (tmp->type != CMD)
			(*current) = tmp;
		tmp = tmp->right;
	}
	this->content[i] = NULL;
	this->type = CMD;
	return (this);
}

t_node	*new_pipe_node(t_token *current, t_node *this)
{
	this->content = malloc(2 * sizeof (char *));
	if (!this->content)
	{
		free(this);
		return (NULL);
	}
	this->content[0] = current->str;
	this->content[1] = NULL;
	this->type = PIPE;
	return (this);
}

t_node	*new_redirect_node(t_token **current, t_node *this)
{
	int	i;

	i = 0;
	this->type = (*current)->type;
	if (this->type == HEREDOC)
		this->content = malloc(4 * sizeof (char *));
	else
		this->content = malloc(3 * sizeof (char *));
	if (!this->content)
		return (free(this), NULL);
	this->content[i++] = (*current)->str;
	if (this->type == HEREDOC)
		this->content[i++] = NULL;
	*current = (*current)->right;
	if ((*current)->type != CMD)
	{
		unexpected_token((*current)->str);
		return (free(this->content), free(this), NULL);
	}
	this->content[i++] = (*current)->str;
	this->content[i] = NULL;
	return (this);
}

t_node	*new_list_node(t_token **current)
{
	t_node	*new;

	new = malloc(sizeof (t_node));
	if (!new)
		return (NULL);
	if ((*current)->type == END)
		return (last_node(*current, new));
	if ((*current)->type == CMD)
		return (new_command_node(current, new));
	if ((*current)->type == PIPE)
		return (new_pipe_node(*current, new));
	else
		return (new_redirect_node(current, new));
}

bool	parse_tokens(t_var *var)
{
	t_token	*current;
	t_node	*new;

	var->in_fd = STDIN_FILENO;
	var->out_fd = STDOUT_FILENO;
	var->cmds = 0;
	if (!var->tokens)
		return (false);
	if (!valid_syntax(var->tokens))
	{
		var->status = 2;
		return (free_tokens(var), false);
	}
	current = var->tokens;
	while (current)
	{
		new = new_list_node(&current);
		if (!new)
			return (free_linked_lists(var), false);
		add_to_list(var, new);
		current = current->right;
	}
	//print_exec_list(var->list);	//DEBUG
	return (true);
}
