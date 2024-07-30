/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 19:46:32 by bszilas           #+#    #+#             */
/*   Updated: 2024/07/30 10:37:01 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int cd_export_exit_or_unset(t_var *var)
{
	t_node *cmd;
	
	cmd = get_next_node(var->current, CMD, END);
	if (!cmd)
		return (close_in_and_out(var), free_all(var), EXIT_SUCCESS);
	if (ft_strncmp(cmd->content[0], "export", 7) == 0)
		var->env = command_export(var, cmd->content[1]);
	else if (ft_strncmp(cmd->content[0], "unset", 6) == 0)
		var->env = command_unset(var->env, cmd->content[1]);
	else if (ft_strncmp(cmd->content[0], "cd", 3) == 0)
		command_cd(var);
	else if (ft_strncmp(cmd->content[0], "exit", 5) == 0)
		command_exit(var);
	else
		return (false);
	if (!var->env)
		restore_environment(var);
	return (true);
}

void	exec_other_commands(t_var *var)
{
	t_node *cmd;

	cmd = get_next_node(var->current, CMD, END);
	if (!cmd)
		return (close_in_and_out(var), free_all(var), exit(EXIT_SUCCESS));
	if (ft_strncmp(cmd->content[0], "echo", 5) == 0)
		command_echo(cmd);
	else if (ft_strncmp(cmd->content[0], "env", 4) == 0)
		command_env(var);
	else if (ft_strncmp(cmd->content[0], "pwd", 4) == 0)
		command_pwd();
	else
		exec_system_commands(var);
	close_in_and_out(var);
	free_all(var);
	exit(EXIT_SUCCESS);
}

void	exec_builtins(t_var *var)
{
	var->current = get_next_node(var->current, CMD, PIPE | END);
	if (!var->current)
		return (close_in_and_out(var), free_all(var), exit(EXIT_SUCCESS));
	if (ft_strncmp(var->current->content[0], "export", 7) == 0)
		var->env = command_export(var, var->current->content[1]);
	else if (ft_strncmp(var->current->content[0], "unset", 6) == 0)
		var->env = command_unset(var->env, var->current->content[1]);
	else if (ft_strncmp(var->current->content[0], "cd", 3) == 0)
		command_cd(var);
	else if (ft_strncmp(var->current->content[0], "exit", 5) == 0)
		command_exit(var);
	else if (ft_strncmp(var->current->content[0], "echo", 5) == 0)
		command_echo(var->current);
	else if (ft_strncmp(var->current->content[0], "env", 4) == 0)
		command_env(var);
	else if (ft_strncmp(var->current->content[0], "pwd", 4) == 0)
		command_pwd();
	else
		return ;
	return (close_in_and_out(var), free_all(var), exit(EXIT_SUCCESS));
}

char	**splitted_path(t_var *var)
{
	int	i;
	char	*path;

	i = 0;
	path = NULL;
	while (var->env[i])
	{
		if (strncmp(var->env[i], "PATH=", 5) == 0)
			path = var->env[i];
		i++;
	}
	var->splitted_path = ft_split(path, ':');
	if (var->splitted_path == NULL)
		return (NULL);
	return (var->splitted_path);
}

char	*get_cmd(t_var *var)
{	
	char	*executable_command;
	int		i;

	i = 0;
	while (var->splitted_path)
	{
		executable_command = ft_strjoin_three(var->splitted_path[i], var->current->content[0]);
		if (!executable_command)
			return (NULL);
		if (access(executable_command, X_OK) == 0)
			return (executable_command);
		free(executable_command);
		i++;
	}
	return (NULL);
}

void	exec_system_commands(t_var *var)
{
	
	char	*executable_command;
	// char	*args[] = {"/bin/cat", NULL};
	
	executable_command = get_cmd(var);
	if (!executable_command)
		return (perror(var->current->content[0]), free_all(var), exit(EXIT_FAILURE));
	execve(executable_command, var->current->content, var->env);
}

void	first_cmd(t_var *var)
{
	if (pipe(var->pfd) == -1)
		return (perror("pipe"), free_all(var), close_in_and_out(var), exit(EXIT_FAILURE));
	var->pid = fork();
	if (var->pid == 0)
	{
		dup2(var->pfd[WRITE_END], STDOUT_FILENO);
		close_pipe(var->pfd);
		in_open_or_exit(var);
		out_open_or_exit(var);
		exec_builtins(var);
		exec_system_commands(var);
	}
	close(var->pfd[WRITE_END]);
	var->current = get_next_node(var->current, PIPE, END)->next;
}

void	middle_cmd(t_var *var)
{
	var->current = get_next_node(var->current, PIPE, END)->next;
}

void	last_cmd(t_var *var)
{
	var->pid2 = fork();
	if (var->pid2 == 0)
	{
		dup2(var->pfd[READ_END], STDIN_FILENO);
		close(var->pfd[READ_END]);
		in_open_or_exit(var);
		out_open_or_exit(var);
		exec_builtins(var);
		exec_system_commands(var);
	}
	close(var->pfd[READ_END]);
}

void	wait_children(t_var *var)
{
	int		status;
	pid_t	pid;

/* 	waitpid(var->pid, &status, 0);
	waitpid(var->pid2, &var->status, 0); */

	pid = wait(&status);
	if (pid == var->pid)
	{
		var->status = status;
		pid = waitpid(var->pid2, &var->status, 0);
	}
	/* else if (pid == var->pid2)
		pid = waitpid(var->pid, &status, 0); */
	if (WIFEXITED(var->status))
		var->status = WEXITSTATUS(var->status);
	return ;
}

void	one_simple_cmd(t_var *var)
{
	var->status = in_open_return_status(var);
	if (var->status)
		return ;
	var->status = out_open_return_status(var);
	if (var->status)
		return ;
	var->current = get_next_node(var->list, CMD, END | PIPE);
	if (!var->current)
		return ;
	if (cd_export_exit_or_unset(var))
		return ;
	var->pid = fork();
	if (var->pid == 0)
	{
		file_redirect(var);
		exec_other_commands(var);
		exit(EXIT_SUCCESS);
	}
	if (wait(&var->status) == -1)
		perror("wait");
	if (WIFEXITED(var->status))
		var->status = WEXITSTATUS(var->status);
}

void	execute(t_var *var)
{
	write_here_docs(var);
	var->current = var->list;
	var->cmds = count_node_types(var->list, PIPE | END);
	var->splitted_path = splitted_path(var);
	if (var->cmds == 1)
		return (one_simple_cmd(var));
	var->i = 0;
	while (var->i < var->cmds)
	{
		if (var->i == 0)
			first_cmd(var);
		else if (var->i == var->cmds - 1)
			last_cmd(var);
		var->i++;
	}
	wait_children(var);
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