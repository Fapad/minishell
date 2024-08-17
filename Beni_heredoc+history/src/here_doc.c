/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 12:35:45 by bszilas           #+#    #+#             */
/*   Updated: 2024/08/17 10:26:56 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*write_heredoc_line(t_var *var, int fd, char *line, int expand)
{
	line = ft_strjoin(line, "\n");
	if (expand)
		line = expand_heredoc_line(var, line);
	if (!line)
		return (free(var->hd_history), NULL);
	write(fd, line, ft_strlen(line));
	var->hd_history = ft_strjoin(var->hd_history, line);
	free(line);
	return (var->hd_history);
}

void	write_doc_from_history(t_var *var, int fd)
{
	char	*limiter;
	char	*line;
	char	*nl;

	line = var->input_after_nl;
	nl = ft_strchr(line, '\n');
	if (nl)
		*nl = 0;
	limiter = var->current->content[2];
	while (line && ft_strncmp(line, limiter, ft_strlen(limiter) + 1))
	{
		write(fd, line, ft_strlen(line));
		if (nl)
			*nl = '\n';
		line = ft_strchr(line, '\n');
		if (line)
		{
			line++;
			nl = ft_strchr(line, '\n');
			if (nl)
				*nl = 0;
		}
	}
	var->hd_history = ft_strjoin(var->hd_history, var->input_after_nl);
	update_input_after_nl(var, nl);
}

char	*write_doc(t_var *var, char *limiter, int fd, int expand)
{
	extern sig_atomic_t	g_signal;
	char				*line;

	var->prompt = heredoc_prompt(var, limiter);
	sigint_handler_interactive_mode(var);
	line = readline(var->prompt);
	while (line && ft_strncmp(line, limiter, ft_strlen(limiter) + 1) && !g_signal)
	{
		var->hd_history = write_heredoc_line(var, fd, line, expand);
		if (!var->hd_history)
			return (free_heredoc_prompt(var), perror("heredoc"), NULL);
		line = readline(var->prompt);
	}
	sigint_handler_non_interactive_mode(var);
	free_heredoc_prompt(var);
	if (!line)
		return (heredoc_warning(var, g_signal, limiter), var->hd_history);
	line = append_char_if_missing(line, '\n');
	if (!g_signal)
		var->hd_history = ft_strjoin(var->hd_history, line);
	return (free(line), var->hd_history);
}

int	write_here_docs(t_var *var)
{
	int		fd;

	var->current = get_next_node(var->list, HEREDOC, END);
	while (var->current && !var->status)
	{
		fd = create_tmp_file(var->current);
		if (fd == -1)
			return (perror("heredoc"), status_1(var), false);
		if (var->input_after_nl)
			write_doc_from_history(var, fd);
		else
			var->hd_history = write_doc(var, var->current->content[2], fd, !(var->current->type & NO_EXPAND));
		close(fd);
		check_received_signal(var);
		if (var->status)
			return (add_heredoc_to_rl_history(var), false);
		if (!var->hd_history)
			return (perror("heredoc"), status_1(var), false);
		var->current = get_next_node(var->current->next, HEREDOC, END);
	}
	var->current = NULL;
	add_heredoc_to_rl_history(var);
	return (true);
}
