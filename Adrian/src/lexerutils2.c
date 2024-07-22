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
	(void)start;
	while (**end && **end != ' '
		&& **end != '|' && **end != '<' && **end != '>')
	{
		(*end)++;
	}
	return (CMD);
}

int	identify_token_type(const char **start, const char **end)
{
	int	type;

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
