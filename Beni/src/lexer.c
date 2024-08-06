/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 10:35:08 by ajovanov          #+#    #+#             */
/*   Updated: 2024/08/06 10:22:54 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	add_token_to_list(t_var *var, t_token *new_token)
{
	if (!var->tokens && !var->last_token)
	{
		var->tokens = new_token;
		var->last_token = new_token;
	}
	else
	{
		var->last_token->right = new_token;
		var->last_token = new_token;
	}
}

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

void	free_bare_tokens(t_token *last)
{
	t_token	*ptr;
	t_token	*current;

	ptr = last->right;
	while (ptr)
	{
		current = ptr;
		ptr = ptr->right;
		free(current);
	}
	last->right = NULL;
}

int	add_compound_tokens(t_var *var, char *str)
{
	char	*split;
	size_t	token_count;
	size_t	i;
	t_token	*new;
	t_token	*last;

	split = ft_split(str, TO_SPLIT);
	if (!split)
		return (false);
	last = var->last_token;
	token_count = 0;
	while (split[token_count])
		token_count++;
	while (i < token_count)
	{
		new = malloc(sizeof (t_token));
		if (!new)
			return (free_string_array(split), free_bare_tokens(last), false);
		new->type = CMD;
		new->str = split[i];
		new->right = NULL;
		add_token_to_list(var, new);
		i++;
	}
	return (true);
}

int	add_token(t_var *var, char **start)
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
	if (ft_strchr(str, TO_SPLIT))
	{
		if (!add_compound_tokens(var, str))
			return (free_tokens(var->tokens), false);
	}
	else
	{
		new_token = create_token(type, str);
		if (!new_token)
			return (free_tokens(var->tokens), false);
		add_token_to_list(var, new_token);
	}
	*start = end;
	return (true);
}

t_token	*tokenize(t_var *var)
{
	char	*start;

	start = var->line;
	while (*start)
	{
		skip_whitespace(&start);
		if (!*start)
			break ;
		if (!add_token(var, &start))
			return (NULL);
	}
	if (!add_token(var, &start))
		return (NULL);
	return (var->tokens);
}
