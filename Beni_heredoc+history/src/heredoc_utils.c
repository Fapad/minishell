/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 18:26:29 by bszilas           #+#    #+#             */
/*   Updated: 2024/08/17 11:05:22 by bszilas          ###   ########.fr       */
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
	var->prompt = ft_strjoin_nofree(HD_PROMPT, limiter);
	var->prompt = ft_strjoin(var->prompt, "\" > ");
	if (!var->prompt)
		var->prompt = var->stack_prompt;
	return (var->prompt);
}

char	*ft_strdup_plus_1(char *str)
{
	char	*ret;

	ret = NULL;
	if (str)
		ret = ft_strdup(str + 1);
	if (ret && !*ret)
	{
		free(ret);
		ret = NULL;
	}
	return (ret);
}

void	update_input_after_nl(t_var *var, char *nl)
{
	char	*line;

	line = var->input_after_nl;
	if (nl)
		*nl = '\n';
	if (!var->hd_history || !nl)
	{
		free(var->input_after_nl);
		var->input_after_nl = NULL;
	}
	else
	{
		line = ft_strdup_plus_1(nl);
		free(var->input_after_nl);
		var->input_after_nl = line;
	}
}
