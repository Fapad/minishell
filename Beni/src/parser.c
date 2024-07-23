/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 12:05:27 by bszilas           #+#    #+#             */
/*   Updated: 2024/07/23 14:50:06 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_node	*last_node(t_token *current, t_node *this)
{
	this->content = malloc(2 * sizeof (char *));
	if (!this->content)
	{
		free(this);
		return (NULL);
	}
	this->content[0] = current->str;
	this->content[1] = NULL;
	this->type = END;
	return (this);
}

int	token_arg_count(t_token *current)
{
	int	count;

	count = 1;
	while (current->right)
	{
		current = current->right;
		if (current->type != CMD)
			break ;
		count++;
	}
	return (count);
}

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

void	set_redirect_type(t_node *this)
{
	if (this->content[0][0] == '<')
		this->type = IN_R;
	if (this->content[0][0] == '>')
		this->type = OUT_R;
	if (this->content[0][1] == '<')
		this->type = HEREDOC;
	if (this->content[0][1] == '>')
		this->type = OUT_APPEND;
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

void	add_to_list(t_var *var, t_node *this)
{
	t_node	*ptr;

	ptr = var->list;
	if (!ptr)
	{
		var->list = this;
		this->prev = NULL;
		this->next = NULL;
		return ;
	}
	else
	{
		while (ptr->next)
			ptr = ptr->next;
		ptr->next = this;
		this->prev = ptr;
		this->next = NULL;
	}
}

void	print_exec_list(t_node *list)
{
	int j = 1;
	while (list->next)
	{
		ft_printf("list %d. type: %d\n", j, list->type);
		for (size_t i = 0; list->content[i]; i++)
			ft_printf("\tcontent %d. : %s\n", i, list->content[i]);
		list = list->next;
		j++;
	}
	ft_printf("list %d. type: %d\n", j, list->type);
	for (size_t i = 0; list->content[i]; i++)
		ft_printf("\tcontent %d. : %s\n", i, list->content[i]);
	ft_printf("BACKWARDS:\n");
	while (list->prev)
	{
		j--;
		list = list->prev;
		ft_printf("list %d. type: %d\n", j, list->type);
		for (size_t i = 0; list->content[i]; i++)
			ft_printf("\tcontent %d. : %s\n", i, list->content[i]);
	}
}

bool	valid_syntax(t_node *node)
{
	if ((node->type != CMD && !node->prev) || 
		(node->type == PIPE && node->prev && node->prev->type == PIPE))
	{
		unexpected_token(node->content[0]);
		return (false);
	}
	return (true);
}

bool	parse_tokens(t_var *var)
{
	t_token	*current;
	t_node	*new;

	current = var->tokens;
	while (current)
	{
		new = new_list_node(&current);
		if (!new)
			return (free_linked_lists(var), 0);
		add_to_list(var, new);
		if (!valid_syntax(new))
			return (free_linked_lists(var), 0);
		current = current->right;
	}
	return (1);
}
