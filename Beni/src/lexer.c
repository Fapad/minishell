/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 10:35:08 by ajovanov          #+#    #+#             */
/*   Updated: 2024/07/30 13:55:57 by bszilas          ###   ########.fr       */
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

char	*tokenize_str(t_var *var, char *start, char *end, int *type)
{
	char	*str;

	str = NULL;
	var->len = end - start;
	if (*type != INTERPRET)
		str = ft_strndup(start, var->len);
	else
	{
		var->len = interpreted_str_len(var, start, end);
		str = cat_intrd_str(var, start, end);
		*type = CMD;
	}
	return (str);
}

int	add_token(t_var *var, t_token **last, char **start)
{
	t_token	*new_token;
	char	*end;
	int		type;
	char	*str;

	end = *start;
	type = identify_token_type(start, &end);
	str = tokenize_str(var, *start, end, &type);
	if (!str)
		return (free_tokens(var->tokens), false);
	new_token = create_token(type, str);
	if (!new_token)
		return (free_tokens(var->tokens), false);
	if (!var->tokens)
		var->tokens = new_token;
	else if (*last)
		(*last)->right = new_token;
	*last = new_token;
	*start = end;
	return (true);
}

t_token	*tokenize(t_var *var)
{
	char	*start;
	t_token	*last;

	last = NULL;
	start = var->line;
	while (*start != '\0')
	{
		skip_whitespace(&start);
		if (*start == '\0')
			break ;
		if (!add_token(var, &last, &start))
			return (NULL);
	}
	if (!add_token(var, &last, &start))
		return (NULL);
	return (var->tokens);
}
