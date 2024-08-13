/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 18:26:29 by bszilas           #+#    #+#             */
/*   Updated: 2024/08/13 18:27:42 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	flag_heredoc(t_var *var)
{
	if (var->last_token && var->last_token->type & HEREDOC)
		var->last_token->type = var->last_token->type | NO_EXPAND;
}

void	heredoc_prompt(char *limiter, size_t limiter_size)
{
	ft_printf(HD_PROMPT);
	write(STDOUT_FILENO, limiter, limiter_size - 2);
	ft_printf("\" > ");
}
