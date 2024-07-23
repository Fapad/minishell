/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexerutils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajovanov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 10:35:44 by ajovanov          #+#    #+#             */
/*   Updated: 2024/07/22 10:35:46 by ajovanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	identify_input_redirection(const char **start, const char **end)
{
	if (**start == '<')
	{
		if (*(*start + 1) == '<')
		{
			*end += 2;
			return (HEREDOC);
		}
		*end += 1;
		return (IN_R);
	}
	return (0);
}

int	identify_output_redirection(const char **start, const char **end)
{
	if (**start == '>')
	{
		if (*(*start + 1) == '>')
		{
			*end += 2;
			return (OUT_APPEND);
		}
		*end += 1;
		return (OUT_R);
	}
	return (0);
}

int	identify_pipe(const char **start, const char **end)
{
	if (**start == '|')
	{
		*end += 1;
		return (PIPE);
	}
	return (0);
}

int	identify_general_token(const char **start, const char **end)
{
	int	a;

	a = 0;
	while (**end && **end != ' '
		&& **end != '|' && **end != '<' && **end != '>')
	{
		a = identify_dollar_sign(start, end);
		if (a != 0)
		{	
			return (VAR);
		}
		(*end)++;
	}
/* 	if (**end == '"')
		(*end)--; */
	return (CMD);
}

int	identify_single_quotes(const char **start, const char **end)
{
	const char *tmp = *end;
	if (**start == '\'')
	{
		*end = *start;
		(*end)++;
		while (**end)
		{
			if (**end == '\'')
			{
				(*end)++;
				return (10);
			}
			(*end)++;
		}
	}
	*end = tmp;
	return (0);
}

int	identify_dollar_sign(const char **start, const char **end)
{
	
	if (**end == '$')
	{
		if (**start == '"')
			(*start)++;
		while (**end != 32 && **end != '\0' && **end != '"')
			(*end)++;
		return (40);
	}
	return (0);
}

int	identify_double_quotes(const char **start, const char **end)
{
 	const char *tmp = *end;
	int	dollar;

	dollar = 0;
	if (**start == '\"')
	{
		*end = *start;
		(*end)++;
		while (**end)
		{
			//dollar = identify_dollar_sign(start, end);
			if (dollar != 0)
				return (VAR);
			if (**end == '\"')
			{
				(*end)++;
				return (10);
			}
			(*end)++;
		}
	}
	*end = tmp;
	return (0);
}

int	identify_token_type(const char **start, const char **end)
{
	int	type;

	type = 0;
	type = identify_single_quotes(start, end);
	if (type != 0)
		return (type);
	type = identify_double_quotes(start, end);
	if (type != 0)
		return (type);
	type = identify_pipe(start, end);
	if (type != 0)
		return (type);
	type = identify_input_redirection(start, end);
	if (type != 0)
		return (type);
	type = identify_output_redirection(start, end);
	if (type != 0)
		return (type);
	return (identify_general_token(start, end));
}
