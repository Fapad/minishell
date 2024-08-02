/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 12:13:32 by bszilas           #+#    #+#             */
/*   Updated: 2024/08/02 20:23:06 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	count_node_types(t_node *node, int type)
{
	int	n;

	n = 0;
	while (node)
	{
		if (node->type & type)
			n++;
		node = node->next;
	}
	return (n);
}

t_node	*get_next_node(t_node *node, int get_type, int before_type)
{
	while (node && !(node->type & before_type))
	{
		if (node->type & get_type)
			return (node);
		node = node->next;
	}
	return (NULL);
}

char	*ft_strjoin_three(char *s1, char *s2)
{
	int		len1;
	int		len2;
	char	*res;
	int		i;

	i = 0;
	len1 = 0;
	len2 = 0;
	if (!s1 && s2)
		return (NULL);
	if (s1)
		len1 = ft_strlen(s1);
	if (s2)
		len2 = ft_strlen(s2);
	res = (char *)malloc((len1 + len2 +2) * sizeof(char));
	if (!res)
		return (NULL);
	while (s1 && *s1)
		res[i++] = *s1++;
	res[i++] = '/';
	while (s2 && *s2)
		res[i++] = *s2++;
	res[i] = '\0';
	return (res);
}

char	**splitted_path(t_var *var)
{
	char	*path;

	path = ft_getenv(var->env, "PATH");
	if (!path)
		return (NULL);
	return (ft_split(path, ':'));
}

char	*get_cmd(t_var *var)
{	
	char	*cmd;
	int		i;

	i = 0;
	while (var->splitted_path[i])
	{
		cmd = ft_strjoin_three(var->splitted_path[i], var->current->content[0]);
		if (!cmd)
			return (NULL);
		if (access(cmd, X_OK) == 0)
			return (cmd);
		free(cmd);
		i++;
	}
	ft_putstr_fd(var->current->content[0], STDERR_FILENO);
	ft_putendl_fd(": command not found", STDERR_FILENO);
	var->status = 127;
	return (NULL);
}
