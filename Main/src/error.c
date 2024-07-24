/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 09:21:33 by bszilas           #+#    #+#             */
/*   Updated: 2024/07/23 09:58:03 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	free_linked_lists(t_var *var)
{
	t_node	*node;
	t_token	*token;

	while (var->tokens)
	{
		token = var->tokens;
		var->tokens = token->right;
		free(token->str);
		free(token);
	}
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