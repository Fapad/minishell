/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 12:35:45 by bszilas           #+#    #+#             */
/*   Updated: 2024/08/13 18:27:36 by bszilas          ###   ########.fr       */
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

void	write_doc(t_var *var, char *limiter, int fd, int expand)
{
	extern sig_atomic_t	g_signal;
	size_t				limiter_size;
	char				*line;

	limiter_size = ft_strlen(limiter) + 1;
	if (!TESTER)
		heredoc_prompt(limiter, limiter_size);
	sigint_handler_interactive_mode(var);
	line = get_next_line(STDIN_FILENO);
	while (line && ft_strncmp(line, limiter, limiter_size) && !g_signal)
	{
		if (expand)
			line = expand_heredoc_line(var, line);
		if (!line)
			break ;
		write(fd, line, ft_strlen(line));
		free(line);
		if (!TESTER)
			heredoc_prompt(limiter, limiter_size);
		line = get_next_line(STDIN_FILENO);
	}
	if (!line)
		write(STDOUT_FILENO, "\n", 1);
	sigint_handler_non_interactive_mode(var);
	free(line);
}

int	write_here_docs(t_var *var)
{
	char	*limiter;
	t_node	*node;
	int		fd;

	node = get_next_node(var->list, HEREDOC, END);
	while (node && !var->status)
	{
		limiter = ft_strjoin_nofree(node->content[2], "\n");
		if (!limiter)
			return (perror("heredoc"), status_1(var), false);
		fd = create_tmp_file(node);
		if (fd == -1)
			return (perror("heredoc"), status_1(var), free(limiter), false);
		write_doc(var, limiter, fd, !(node->type & NO_EXPAND));
		free(limiter);
		close(fd);
		node = get_next_node(node->next, HEREDOC, END);
		check_received_signal(var);
	}
	if (var->status)
		return (false);
	return (true);
}
