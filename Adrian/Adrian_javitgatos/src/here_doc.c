/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 12:35:45 by bszilas           #+#    #+#             */
/*   Updated: 2024/07/26 14:46:33 by bszilas          ###   ########.fr       */
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

void	write_doc(t_node *node)
{
	size_t	limiter_len;
	char	*line;
	int		fd;

	fd = create_tmp_file(node->content[1]);
	if (fd == -1)
		return (perror("heredoc"));
	limiter_len = ft_strlen(node->content[2]) + 1;
	ft_printf(HD_PROMPT);
	line = get_next_line(STDIN_FILENO);
	while (line && ft_strncmp(line, node->content[2], limiter_len) != '\n')
	{
		write(fd, line, ft_strlen(line));
		free(line);
		ft_printf(HD_PROMPT);
		line = get_next_line(STDIN_FILENO);
	}
	free(line);
	close(fd);
}

void	write_here_docs(t_var *var)
{
	t_node *node;

	node = var->list;
	while (node)
	{
		if (node->type == HEREDOC)
			write_doc(node);
		node = node->next;
	}
}