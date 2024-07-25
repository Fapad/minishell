/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpret_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 17:26:06 by bszilas           #+#    #+#             */
/*   Updated: 2024/07/24 17:26:29 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

size_t	single_quote_len(char *s, char *end, size_t *i)
{
	size_t	len;

	len = 0;
	while (s + len < end)
	{
		len++;
		if (s[len] == '\'')
		{
			*i += len;
			return (len - 1);
		}
	}
	return (1);
}

size_t	env_var_len(char *s, char *end, size_t *i)
{
	size_t	len;
	char	tmp;

	end = ++s;
	if (*end == '$')
		return (1);
	while (*end && !ft_strchr("\"\' $", *end))
		end++;
	tmp = *end;
	*end = '\0';
	len = ft_strlen(getenv(s));
	*end = tmp;
	*i += end - s;
	return (len);
}

size_t	double_qoute_len(char *s, char *end, size_t *i)
{
	size_t	envv_len;
	size_t	len;
	size_t	j;

	len = 0;
	j = 1;
	while (s + j < end)
	{
		envv_len = 1;
		if (s[j] == '$' && s[j + 1] && s[j + 1] != '\"')
			envv_len = env_var_len(s + j, end, &j);
		if (s[j] == '\"')
		{
			*i += j;
			return (len);
		}
		len += envv_len;
		j++;
	}
	return (1);
}

size_t	interpreted_str_len(char *start, char *end)
{
	size_t	i;
	size_t	len;
	
	i = 0;
	len = 0;
	while (start + i < end)
	{
		if (start[i] == '\'')
			len += single_quote_len(start + i, end, &i);
		else if (start[i] == '\"')
			len += double_qoute_len(start + i, end, &i);
		else if (start[i] == '$' && start[i + 1])
			len += env_var_len(start + i, end, &i);
		else
			len++;
		i++;
	}
	return (len);
}
