/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 12:13:32 by bszilas           #+#    #+#             */
/*   Updated: 2024/08/08 15:19:44 by bszilas          ###   ########.fr       */
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

int	search_path(t_var *var, int access_type)
{
	char	*cmd;
	int		i;

	i = 0;
	while (var->splitted_path && var->splitted_path[i])
	{
		cmd = ft_strjoin_three(var->splitted_path[i], var->current->content[0]);
		if (!cmd)
			return (status_1(var), false);
		if (access(cmd, access_type) == 0)
		{
			var->exec_cmd = cmd;
			return (true);
		}
		free(cmd);
		i++;
	}
	return (false);
}

int	txt_file(char *file)
{
	char	buf[BUFFER_SIZE];
	ssize_t	len;
	int		txt;
	int		fd;
	int		i;

	txt = true;
	ft_bzero(buf, BUFFER_SIZE);
	fd = open(file, O_RDONLY);
	if (fd == -1)
		return (!txt);
	len = read(fd, buf, BUFFER_SIZE);
	if (len == -1)
		return (!txt);
	i = 0;
	while (i < len)
	{
		if (!ft_isprint(buf[i]) || buf[i] == '\t' || buf[i] == '\n')
			txt = false;
		i++;
	}
	close(fd);
	return (txt);
}

char	*get_cmd(t_var *var)
{
	if (!var->splitted_path || !var->splitted_path[0])
		return (check_given_file(var));
	if (search_path(var, X_OK))
	{
		if (txt_file(var->exec_cmd))
			error_exec_txt_file(var);
		return (var->exec_cmd);
	}
	if (search_path(var, F_OK))
		return (var->exec_cmd);
	command_not_found(var);
	return (NULL);
}

int	check_files(t_var *var, char *str)
{
	char	buffer[1];

	if (access(str, F_OK) == -1)
	{
		error_msg(var,": No such file or directory", 127);
		return (1);
	}
	
	if (access(str, X_OK) == -1)
	{
		error_msg(var,": Permission denied", 126);
		return (1);
	}
	int fd = open(str, O_RDONLY);
	if (read(fd,buffer, 1) == -1)
	{
		close(fd);
		error_msg(var,": Is a directory", 126);
		return (1);
	}
	return (0);
}

char	*check_given_file(t_var *var)
{
	char	*str;

	str = ft_strdup(var->current->content[0]);
	if (!str)
		return (status_1(var), NULL);
	if ((check_files(var, str)) == 1)
		return (free(str), NULL);
	if (access(str, X_OK) == 0)
	{
		if (txt_file(str))
			return (error_exec_txt_file(var), free(str), NULL);
		return (str);
	}
	if (access(str, F_OK) == 0)
		return (str);
	command_not_found(var);
	free(str);
	return (NULL);
}

void	wait_children(t_var *var)
{
	int		status;
	int		i;
	pid_t	pid;

	pid = 0;
	i = 0;
	status = var->status;
	while (pid != var->pid)
	{
		pid = wait(&status);
		i++;
	}
	var->status = status;
	while (i++ < var->cmds)
		wait(&status);
	if (WIFEXITED(var->status))
		var->status = WEXITSTATUS(var->status);
	else if (WIFSIGNALED(var->status))
		var->status = 128 + WTERMSIG(var->status);
}
