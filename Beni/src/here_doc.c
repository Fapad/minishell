/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 12:35:45 by bszilas           #+#    #+#             */
/*   Updated: 2024/08/16 21:51:22 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	create_tmp_file(t_node *node)
{
	int		i;
	int		len;
	char	*file;

	node->content[FILENAME] = malloc(BUFFER_SIZE * sizeof (char));
	if (!node->content[FILENAME])
		return (-1);
	file = node->content[FILENAME];
	ft_strlcpy(file, TMP_PATH, BUFFER_SIZE);
	i = ft_strlen(file);
	len = i + 10;
	while (i < len)
	{
		file[i] = ft_rand_alnum((size_t)node, i);
		i++;
	}
	file[i] = 0;
	return (open(file, O_CREAT | O_WRONLY, 0644));
}

void	heredoc_expand_line_len(t_var *var, char *line)
{
	size_t	i;

	i = 0;
	var->len = 0;
	while (line[i] && line[i + 1])
	{
		if (possible_var(var, line[i], line[i + 1]))
			var->len++;
		i++;
	}
	if (var->len)
	{
		var->len = 0;
		i = 0;
		while (line[i])
		{
			if (possible_var(var, line[i], line[i + 1]))
				var->len += env_var_len(var, &line[i], line, &i);
			else
				var->len++;
			i++;
		}
	}
}

char	*expand_heredoc_line(t_var *var, char *line)
{
	size_t	len;
	char	*str;

	if (!line)
		return (NULL);
	heredoc_expand_line_len(var, line);
	if (!var->len)
		return (line);
	len = ft_strlen(line);
	str = ft_calloc(var->len + 1, sizeof (char));
	if (!str)
		return (perror("heredoc"), free(line), NULL);
	while (*line)
	{
		if (possible_var(var, *line, *(line + 1)))
			cat_env_var(var, str, &line, line);
		else
			cat_char_to_str(str, *line, var->len + 1);
		line++;
	}
	return (free(line - len), str);
}

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

char	*append_char_if_missing(char *line, char c)
{
	size_t	len;
	char	buf[2];

	buf[0] = c;
	buf[1] = 0;
	len = ft_strlen(line);
	if (!len || line[len - 1] != c)
		return (ft_strjoin(line, buf));
	return (line);
}

char	*write_doc(t_var *var, char *limiter, int fd, int expand)
{
	extern sig_atomic_t	g_signal;
	char				*line;

	sigint_handler_interactive_mode(var);
	if (TESTER)
		line = trim_nl_free(get_next_line(STDIN_FILENO));
	else
		line = readline(heredoc_prompt(var, limiter));
	while (line && ft_strncmp(line, limiter, ft_strlen(limiter) + 1) && !g_signal)
	{
		var->hd_history = write_heredoc_line(var, fd, line, expand);
		if (!var->hd_history)
			return (free_heredoc_prompt(var), perror("heredoc"), NULL);
		if (TESTER)
			line = trim_nl_free(get_next_line(STDIN_FILENO));
		else
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

void	add_heredoc_to_rl(t_var *var)
{
	char	*ptr;

	if (var->hd_history)
	{
		ptr = var->line;
		var->line = ft_strjoin_three(var->line, var->hd_history, '\n');
		free(ptr);
		free(var->hd_history);
		var->hd_history = NULL;
	}
}

int	write_here_docs(t_var *var)
{
	t_node	*node;
	int		fd;

	node = get_next_node(var->list, HEREDOC, END);
	while (node && !var->status)
	{
		fd = create_tmp_file(node);
		if (fd == -1)
			return (perror("heredoc"), status_1(var), false);
		var->hd_history = write_doc(var, node->content[2], fd, !(node->type & NO_EXPAND));
		close(fd);
		check_received_signal(var);
		if (var->status)
			return (add_heredoc_to_rl(var), false);
		if (!var->hd_history)
			return (perror("heredoc"), status_1(var), false);
		node = get_next_node(node->next, HEREDOC, END);
	}
	add_heredoc_to_rl(var);
	return (true);
}
