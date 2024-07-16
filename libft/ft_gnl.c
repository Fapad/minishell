/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_gnl.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 17:29:37 by szilas            #+#    #+#             */
/*   Updated: 2024/07/15 10:50:40 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static ssize_t	list_len(t_list **lst)
{
	ssize_t	i;
	ssize_t	list_len;
	t_list	*current;
	char	*str;

	if (!lst)
		return (0);
	list_len = 0;
	current = *lst;
	while (current)
	{
		i = 0;
		str = (char *)(current->content);
		while (str[i] != 0)
		{
			if (str[i++] == '\n')
				return (++list_len);
			++list_len;
		}
		current = current->next;
	}
	return (list_len);
}

static int	stash_to_list(t_list **head, char *stash)
{
	char	*str;
	t_list	*new;

	str = ft_strdup(stash);
	ft_bzero(stash, BUFFER_SIZE + 1);
	if (!str)
		return (0);
	new = ft_lstnew(str);
	if (!new)
		return (free(str), 0);
	ft_lstadd_front(head, new);
	return (1);
}

static char	*list_to_line(t_list **head, char *line, char *stash)
{
	t_list	*current;
	char	*str;
	ssize_t	line_len;

	str = NULL;
	line_len = list_len(head) + 1;
	line = ft_calloc(line_len, sizeof (char));
	if (!line)
		return (NULL);
	current = *head;
	while (current)
	{
		str = (char *)(current->content);
		ft_strlcat(line, str, line_len);
		current = current->next;
	}
	if (ft_strchr(str, '\n'))
		ft_strlcpy(stash, ft_strchr(str, '\n') + 1, BUFFER_SIZE + 1);
	return (line);
}

static ssize_t	file_to_list(t_list **head, int fd, ssize_t len)
{
	t_list	*node;
	char	*buf;

	buf = NULL;
	while (len == BUFFER_SIZE && !ft_strchr(buf, '\n'))
	{
		buf = ft_calloc((BUFFER_SIZE + 1), sizeof (char));
		if (!buf)
			return (ft_lstclear(head, &free), -1);
		len = read(fd, buf, BUFFER_SIZE);
		if (len == -1)
			return (ft_lstclear(head, &free), free(buf), -1);
		node = ft_lstnew(buf);
		if (!node)
			return (ft_lstclear(head, &free), free(buf), -1);
		ft_lstadd_back(head, node);
	}
	return (len);
}

char	*get_next_line(int fd)
{
	static char	stash[BUFFER_SIZE + 1];
	ssize_t		len;
	t_list		*head;
	char		*line;

	line = NULL;
	head = NULL;
	len = BUFFER_SIZE;
	if (fd < 0)
		return (NULL);
	if (!ft_strchr(stash, '\n'))
	{
		len = file_to_list(&head, fd, len);
		if (len == -1)
			return (ft_bzero(stash, BUFFER_SIZE +1), NULL);
	}
	if (ft_strlen(stash) && !stash_to_list(&head, stash))
		return (ft_lstclear(&head, &free), NULL);
	line = list_to_line(&head, line, stash);
	ft_lstclear(&head, &free);
	if (!line || ft_strlen(line) == 0)
		return (free(line), NULL);
	return (line);
}
/* 
#include <fcntl.h>

int main(int argc, char const *argv[])
{
	(void)argc;
	int fd = open(argv[1], O_RDONLY);
	if (!fd)
		return (-1);
	char *buf3;
	while (	(buf3 = get_next_line(fd)) != NULL)
	{
		ft_printf("%s", buf3);
		free(buf3);
	}
	return 0;
}
*/