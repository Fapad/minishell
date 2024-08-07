/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpret_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 20:43:43 by bszilas           #+#    #+#             */
/*   Updated: 2024/08/06 21:18:36 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

bool	possible_var(t_var *var, char c, char d)
{
	if (var->last_token && var->last_token->type & HEREDOC)
		return (false);
	if (!d || d == '\"')
		return (false);
	return (c == '$' && (ft_isalpha(d) || ft_strchr("_?", d)));
}

void	mark_whitespaces(char *str)
{
	while (*str)
	{
		if (*str == ' ')
			*str = TO_SPLIT;
		str++;
	}
}
