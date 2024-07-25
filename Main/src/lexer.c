/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 10:35:08 by ajovanov          #+#    #+#             */
/*   Updated: 2024/07/24 17:31:36 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_token	*create_token(int type, char *str)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (free(str), NULL);
	token->type = type;
	token->right = NULL;
	token->str = str;
	return (token);
}

char	*tokenize_str(char *start, char *end, int *type)
{
	size_t	len;
	char	*str;

	str = NULL;
	len = end - start;
	if (*type != INTERPRET)
		str = ft_strndup(start, len);
	else
	{
		len = interpreted_str_len(start, end);
		str = cat_intrd_str(str, start, end, len);
		*type = CMD;
	}
	return (str);
}

int	add_token(t_token **head, t_token **current, char **start)
{
	t_token	*new_token;
	char	*end;
	int		type;
	char	*str;

	end = *start;
	type = identify_token_type(start, &end);
	str = tokenize_str(*start, end, &type);
	if (!str)
		return (free_tokens(*head), false);
	new_token = create_token(type, str);
	if (!new_token)
		return (free_tokens(*head), false);
	if (!*head)
		*head = new_token;
	else
		(*current)->right = new_token;
	*current = new_token;
	*start = end;
	return (true);
}

t_token	*tokenize(char *input)
{
	char	*start;
	t_token	*head;
	t_token	*current;

	head = NULL;
	current = NULL;
	start = input;
	while (*start != '\0')
	{
		skip_whitespace(&start);
		if (*start == '\0')
			break ;
		if (!add_token(&head, &current, &start))
			return (NULL);
	}
	if (!add_token(&head, &current, &start))
		return (NULL);
	return (head);
}
