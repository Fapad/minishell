/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 18:26:29 by bszilas           #+#    #+#             */
/*   Updated: 2024/08/16 21:45:24 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	flag_heredoc(t_var *var)
{
	if (var->last_token && var->last_token->type & HEREDOC)
		var->last_token->type = var->last_token->type | NO_EXPAND;
}

void	heredoc_warning(t_var *var, int signal, char *limiter)
{
	if (signal)
		write(STDOUT_FILENO, "\n", 1);
	else
	{
		var->hd_history = ft_strjoin(var->hd_history, "\n");
		ft_putstr_fd("warning: here-document delimited by end-of-file \
(wanted `", STDERR_FILENO);
		ft_putstr_fd(limiter, STDERR_FILENO);
		ft_putendl_fd("\')", STDERR_FILENO);
	}
}

char	*heredoc_prompt(t_var *var, char *limiter)
{
	if (TESTER)
		return (NULL);
	var->prompt = ft_strjoin_nofree(HD_PROMPT, limiter);
	var->prompt = ft_strjoin(var->prompt, "\" > ");
	if (!var->prompt)
		var->prompt = var->stack_prompt;
	return (var->prompt);
}
