/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexerutils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 10:35:44 by ajovanov          #+#    #+#             */
/*   Updated: 2024/07/24 11:44:56 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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
	int	type;
	
	if (!**start)
		return (END);
	type = CMD;
	while (**end && !ft_strchr("< >|", **end))
	{
		if (**end == '\'' && identify_single_quotes(end, end))
			type = INTERPRET;
		else if (**end == '\"' && identify_double_quotes(end, end))
			type = INTERPRET;
		else if (**end == '$')
			type = INTERPRET;
		(*end)++;
	}
	return (type);
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
				return (true);
			(*end)++;
		}
	}
	*end = tmp;
	return (false);
}

int	identify_double_quotes(const char **start, const char **end)
{
 	const char *tmp = *end;

	if (**start == '\"')
	{
		*end = *start;
		(*end)++;
		while (**end)
		{
			if (**end == '\"')
				return (true);
			(*end)++;
		}
	}
	*end = tmp;
	return (false);
}

int	identify_dollar_sign(const char **start, const char **end)
{
	
	if (**end == '$')
	{
		if (**start == '"')
			(*start)++;
		while (**end != 32 && **end != '\0' && **end != '"')
			(*end)++;
		return (true);
	}
	return (false);
}

int	identify_token_type(const char **start, const char **end)
{
	int	type;

	type = 0;
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
