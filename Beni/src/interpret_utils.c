/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpret_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 17:26:06 by bszilas           #+#    #+#             */
/*   Updated: 2024/08/16 09:13:47 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	ft_getenv_get_envvar_name(char *s, char **var, char *tmp, size_t len)
{
	if (var)
	{
		*tmp = **var;
		**var = 0;
	}
	else
	{
		*tmp = 0;
		*var = s + len;
	}
}

char	*ft_getenv(char **env, char *s)
{
	size_t	i;
	size_t	len;
	char	*var;
	char	tmp;

	i = 0;
	if (!s)
		return (NULL);
	len = ft_strlen(s);
	while (env[i])
	{
		var = ft_strchr(env[i], '=');
		ft_getenv_get_envvar_name(s, &var, &tmp, len);
		if (ft_strlen(env[i]) == len && !ft_strncmp(s, env[i], len + 1))
		{
			*var = tmp;
			return (var);
		}
		*var = tmp;
		i++;
	}
	return (NULL);
}

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

size_t	env_var_len(t_var *var, char *s, char *end, size_t *i)
{
	size_t	len;
	char	tmp;
	char	*str;

	end = ++s;
	if (*end == '?')
	{
		*i += 1;
		return (digits_count(var->status, 10));
	}
	while (*end && (ft_isalnum(*end) || *end == '_'))
		end++;
	tmp = *end;
	*end = '\0';
	str = ft_getenv(var->env, s);
	len = ft_strlen(str);
	*end = tmp;
	*i += end - s;
	return (len);
}

size_t	double_qoute_len(t_var *var, char *s, char *end, size_t *i)
{
	size_t	envv_len;
	size_t	len;
	size_t	j;

	len = 0;
	j = 1;
	while (s + j < end)
	{
		envv_len = 1;
		if (possible_var(var, s[j], s[j + 1]))
			envv_len = env_var_len(var, s + j, end, &j);
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

size_t	interpreted_str_len(t_var *var, char *start, char *end)
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
			len += double_qoute_len(var, start + i, end, &i);
		else if (possible_var(var, start[i], start[i + 1]))
			len += env_var_len(var, start + i, end, &i);
		else
			len++;
		i++;
	}
	return (len);
}
