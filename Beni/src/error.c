/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 09:21:33 by bszilas           #+#    #+#             */
/*   Updated: 2024/07/24 19:39:40 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	free_linked_lists(t_var *var)
{
	t_node	*node;

	free_tokens(var->tokens);
	var->tokens = NULL;
	while (var->list)
	{
		node = var->list;
		var->list = node->next;
		free(node->content);
		free(node);
	}
	var->list = NULL;
}

void	unexpected_token(char *str)
{
	if (!*str)
		ft_printf("syntax error near unexpected token `newline'\n");
	else
		ft_printf("syntax error near unexpected token `%s'\n", str);
}

bool	valid_syntax(t_token *token)
{
	while (token)
	{
		if (token->type != CMD && token->right && token->right->type != CMD)
		{
			unexpected_token(token->right->str);
				return (false);
		}
		token = token->right;
	}
	return (true);
}
