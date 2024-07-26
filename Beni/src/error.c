/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 09:21:33 by bszilas           #+#    #+#             */
/*   Updated: 2024/07/26 12:42:41 by bszilas          ###   ########.fr       */
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
		ft_putstr_fd("syntax error near unexpected token `newline'\n", 
		STDERR_FILENO);
	else
	{
		ft_putstr_fd("syntax error near unexpected token `", STDERR_FILENO);
		ft_putstr_fd(str, STDERR_FILENO);
		ft_putstr_fd("\'\n", STDERR_FILENO);	
	}
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
