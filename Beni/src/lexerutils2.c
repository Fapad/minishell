/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexerutils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 10:35:44 by ajovanov          #+#    #+#             */
/*   Updated: 2024/08/08 17:43:36 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	identify_input_redirection(char **start, char **end)
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

int	identify_output_redirection(char **start, char **end)
{
	if (**start == '>')
	{
		if (*(*start + 1) == '>')
		{
			*end += 2;
			return (O_APPEND);
		}
		*end += 1;
		return (O_TRUNC);
	}
	return (0);
}

int	identify_pipe(char **start, char **end)
{
	if (**start == '|')
	{
		*end += 1;
		return (PIPE);
	}
	return (0);
}

char	*token_end(char *start)
{
	while (*start && !ft_strchr("< >|\t", *start))
		start++;
	return (start);
}

void	reset_end(char *start, char **end, char *ptr, char *tkn_end)
{
	if (ptr != tkn_end)
		*end = start;
	else
		*end = tkn_end;
}

int	identify_nonexistent_var(t_var *var, char **start, char **end)
{
	char	*tkn_end;
	char	*ptr;
	char	c;

	tkn_end = token_end(*start);
	ptr = *start;
	while (possible_var(var, ptr[0], ptr[1]) && ptr[1] != '?')
	{
		*end = ptr + 1;
		ptr += 2;
		while (*ptr && (ft_isalnum(*ptr) || *ptr == '_'))
			ptr++;
		c = *ptr;
		*ptr = 0;
		if (ft_getenv(var->env, *end))
		{
			*ptr = c;
			*end = *start;
			return (false);
		}
		*ptr = c;
	}
	reset_end(*start, end, ptr, tkn_end);
	return (ptr == tkn_end);
}

int	identify_general_token(t_var *var, char **start, char **end)
{
	int	type;
	
	if (!**start)
		return (END);
	if (identify_nonexistent_var(var, start, end))
		return (NO_VAR);
	type = CMD;
	while (**end && !ft_strchr("< >|\t", **end))
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

int	identify_single_quotes(char **start, char **end)
{
	char *tmp = *end;
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

int	identify_double_quotes(char **start, char **end)
{
 	char *tmp = *end;

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

int	identify_dollar_sign(char **start, char **end)
{
	
	if (**end == '$')
	{
		if (**start == '"')
			(*start)++;
		while (**end && !ft_strchr("\t\" ", **end))
			(*end)++;
		return (true);
	}
	return (false);
}

int	identify_token_type(t_var *var, char **start, char **end)
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
	return (identify_general_token(var, start, end));
}
