/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 19:40:36 by bszilas           #+#    #+#             */
/*   Updated: 2024/07/24 19:43:19 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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
		list = list->next;
		j++;
	}
	ft_printf("list %d. type: %d\n", j, list->type);
	for (size_t i = 0; list->content[i]; i++)
		ft_printf("\tcontent %d. : %s\n", i, list->content[i]);
/* 	ft_printf("BACKWARDS:\n");
	while (list->prev)
	{
		j--;
		list = list->prev;
		ft_printf("list %d. type: %d\n", j, list->type);
		for (size_t i = 0; list->content[i]; i++)
			ft_printf("\tcontent %d. : %s\n", i, list->content[i]);
	} */
}
