/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexerutils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajovanov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 10:35:25 by ajovanov          #+#    #+#             */
/*   Updated: 2024/07/22 10:35:27 by ajovanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	print_tokens(t_token *head)
{
	t_token	*current;

	current = head;
	while (current)
	{
		printf("Type: %d, Str: %s\n", current->type, current->str);
		current = current->right;
	}
}

void	free_token(t_token *token)
{
	if (token)
	{
		free(token->str);
		free(token);
	}
}

void	skip_whitespace(const char **input)
{
	while (**input == ' ')
		(*input)++;
}
