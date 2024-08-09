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
	int		fd;

	if (access(str, F_OK) == -1)
	{
		error_msg(var, ": No such file or directory", 127);
		return (1);
	}
	if (access(str, X_OK) == -1)
	{
		error_msg(var, ": Permission denied", 126);
		return (1);
	}
	fd = open(str, O_RDONLY);
	if (read(fd, buffer, 1) == -1)
	{
		close(fd);
		error_msg(var, ": Is a directory", 126);
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
