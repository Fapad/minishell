/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 12:35:45 by bszilas           #+#    #+#             */
/*   Updated: 2024/08/09 19:01:42 by bszilas          ###   ########.fr       */
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

void	write_doc(char *limiter, int fd)
{
	extern sig_atomic_t	g_signal;
	size_t				limiter_size;
	char				*line;

	limiter_size = ft_strlen(limiter) + 1;
	ft_printf(HD_PROMPT);
	write(STDOUT_FILENO, limiter, limiter_size - 2);
	ft_printf("\" > ");
	line = get_next_line(STDIN_FILENO);
	while (line && ft_strncmp(line, limiter, limiter_size) && !g_signal)
	{
		write(fd, line, ft_strlen(line));
		free(line);
		ft_printf(HD_PROMPT);
		write(STDOUT_FILENO, limiter, limiter_size - 2);
		ft_printf("\" > ");
		line = get_next_line(STDIN_FILENO);
	}
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
		write_doc(limiter, fd);
		free(limiter);
		close(fd);
		node = get_next_node(node->next, HEREDOC, END);
		check_received_signal(var);
	}
	if (var->status)
		return (false);
	return (true);
}
