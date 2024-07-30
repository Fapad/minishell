/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpret.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 17:26:45 by bszilas           #+#    #+#             */
/*   Updated: 2024/07/28 18:42:46 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	cat_char_to_str(char *str, char c, size_t len)
{
	char	to_cat[2];

	to_cat[0] = c;
	to_cat[1] = 0;
	ft_strlcat(str, to_cat, len + 1);
}

void	cat_single_qoutes(char *str, char **start, char *end, size_t len)
{
	*end = 0;
	ft_strlcat(str, (*start) + 1, len + 1);
	*end = '\'';
	*start = end;
}

void	cat_env_var(t_var *var, char *str, char **start, char *end)
{
	size_t	i;
	char	tmp;

	i = 0;
	if (start[0][1] == '$')
		ft_strlcat(str, "$", var->len + 1);
	else if (env_var_len(*start, end, &i, var->env))
	{
		(*start)++;
		tmp = start[0][i];
		start[0][i] = 0;
		ft_strlcat(str, ft_getenv(var->env, *start), var->len + 1);
		start[0][i] = tmp;
		*start += i - 1;
	}
	else
		*start += i;
}

void	cat_double_qoutes(t_var *var, char *str, char **start, char *end)
{
	*end = 0;
	(*start)++;
	while (*start < end)
	{
		if (**start == '$' && *(*start + 1))
			cat_env_var(var, str, start, end);
		else
			cat_char_to_str(str, **start, var->len);
		(*start)++;
	}
	*end = '\"';
}

char	*cat_intrd_str(t_var *var, char *start, char *end)
{
	char	*ptr;
	char	*str;

	str = ft_calloc(var->len + 1, sizeof (char));
	if (!str)
		return (NULL);
	while (start < end)
	{
		if (*start == '\'' && identify_single_quotes(&start, &ptr))
			cat_single_qoutes(str, &start, ptr, var->len);
		else if (*start == '\"' && identify_double_quotes(&start, &ptr))
			cat_double_qoutes(var, str, &start, ptr);
		else if (*start == '$' && *start + 1)
			cat_env_var(var, str, &start, end);
		else
			cat_char_to_str(str, *start, var->len);
		start++;
	}
	return (str);
}
