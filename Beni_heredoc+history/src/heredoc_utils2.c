/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 08:36:23 by bszilas           #+#    #+#             */
/*   Updated: 2024/08/17 09:57:50 by bszilas          ###   ########.fr       */
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

void	add_heredoc_to_rl_history(t_var *var)
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
