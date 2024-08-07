/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 19:40:36 by bszilas           #+#    #+#             */
/*   Updated: 2024/08/07 17:11:58 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_token	*find_next_cmd_token(t_token *next)
{
	next = next->right;
	while (next && !(next->type & (PIPE | END)))
	{
		if (next->type == CMD && next->left->type == CMD)
			return (next);
		next = next->right;
	}
	return (NULL);
}

int	token_arg_count(t_token *current)
{
	int	count;

	count = 0;
	while (current)
	{
		count++;
		current = find_next_cmd_token(current);
	}
	return (count);
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

void	print_exec_list(t_node *list)
{
	int j = 1;
	while (list->next)
	{
		ft_printf("list %d. type: %d\n", j, list->type);
		for (size_t i = 0; list->content[i]; i++)
			ft_printf("\tcontent %d. : %s\n", i, list->content[i]);
		ft_printf("%p\n", list);
		ft_printf("%d\n", (size_t)list % INT_MAX);
		list = list->next;
		j++;
	}
	ft_printf("list %d. type: %d\n", j, list->type);
	for (size_t i = 0; list->content[i]; i++)
		ft_printf("\tcontent %d. : %s\n", i, list->content[i]);
}
