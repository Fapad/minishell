/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 12:35:45 by bszilas           #+#    #+#             */
/*   Updated: 2024/08/01 11:47:27 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	create_tmp_file(char *path)
{
	int	i;
	int	len;

	path = malloc(BUFFER_SIZE * sizeof (char));
	if (!path)
		return (-1);
	ft_strlcpy(path, TMP_PATH, BUFFER_SIZE);
	i = ft_strlen(path);
	len = i + 10;
	while (i < len)
	{
		path[i] = ft_rand_alnum((size_t)path, i);
		i++;
	}
	path[i] = 0;
	return (open(path, O_CREAT | O_WRONLY, 0644));
}

void	write_doc(t_node *node, int fd)
{
	size_t	limiter_size;
	char	*line;

	limiter_size = ft_strlen(node->content[2]) + 1;
	ft_printf(HD_PROMPT);
	line = get_next_line(STDIN_FILENO);
	line[ft_strlen(line) - 1] = 0;
	while (line && ft_strncmp(line, node->content[2], limiter_size))
	{
		line[ft_strlen(line) - 1] = '\n';
		write(fd, line, ft_strlen(line));
		free(line);
		ft_printf(HD_PROMPT);
		line = get_next_line(STDIN_FILENO);
		line[ft_strlen(line) - 1] = 0;
	}
	free(line);
}

int	write_here_docs(t_var *var)
{
	t_node *node;
	int		fd;

	node = get_next_node(var->list, HEREDOC, END);
	while (node)
	{
		fd = create_tmp_file(node->content[FILENAME]);
		if (fd == -1)
			return (perror("heredoc"), status_1(var), false);
		write_doc(node, fd);
		close(fd);
		node = get_next_node(node->next, HEREDOC, END);
	}
	return (true);
}