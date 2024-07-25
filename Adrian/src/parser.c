/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 12:05:27 by bszilas           #+#    #+#             */
/*   Updated: 2024/07/24 19:43:12 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_node	*new_command_node(t_token **current, t_node *this)
{
	int	arg_count;
	int	i;

	arg_count = token_arg_count(*current);
	this->content = malloc((arg_count + 1) * sizeof (char *));
	if (!this->content)
	{
		free(this);
		return (NULL);
	}
	i = -1;
	while (++i < arg_count - 1)
	{
		this->content[i] = (*current)->str;
		*current = (*current)->right;
	}
	this->content[i] = (*current)->str;
	this->content[i + 1] = NULL;
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
	this->content = malloc(3 * sizeof (char *));
	if (!this->content)
	{
		free(this);
		return (NULL);
	}
	this->content[0] = (*current)->str;
	*current = (*current)->right;
	this->content[1] = (*current)->str;
	if ((*current)->type != CMD)
	{
		unexpected_token(this->content[1]);
		return (free(this->content), free(this), NULL);
	}
	this->content[2] = NULL;
	set_redirect_type(this);
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

	if (!valid_syntax(var->tokens))
		return (free_linked_lists(var), 0);
	current = var->tokens;
	while (current)
	{
		new = new_list_node(&current);
		if (!new)
			return (free_linked_lists(var), 0);
		add_to_list(var, new);
		current = current->right;
	}
	return (1);
}
