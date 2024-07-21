/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajovanov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 09:55:54 by ajovanov          #+#    #+#             */
/*   Updated: 2024/04/08 14:55:03 by ajovanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

t_token	*create_token(int type, const char *str)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	token->type = type;
	token->left = NULL;
	token->right = NULL;
	token->str = ft_strdup(str);
	token->str_len = ft_strlen(str);
	return (token);
}

void	add_token(t_token **head, t_token **current,
						const char **start)
{
	t_token		*new_token;
	const char	*end;
	int			type;

	end = *start;
	type = identify_token_type(start, &end);
	new_token = create_token(type, strndup(*start, end - *start));
	if (!*head)
		*head = new_token;
	else
		(*current)->right = new_token;
	*current = new_token;
	*start = end;
}

t_token	*tokenize(const char *input)
{
	const char	*start;
	t_token		*head;
	t_token		*current;

	head = NULL;
	current = NULL;
	start = input;
	while (*start != '\0')
	{
		skip_whitespace(&start);
		if (*start == '\0')
			break ;
		add_token(&head, &current, &start);
	}
	return (head);
}
