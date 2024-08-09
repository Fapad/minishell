/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 10:26:38 by bszilas           #+#    #+#             */
/*   Updated: 2024/08/09 20:52:23 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	exec_builtins(t_var *var)
{
	var->current = get_next_node(var->current, CMD, PIPE | END);
	if (!var->current)
		return (free_all(var), exit(EXIT_SUCCESS));
	if (ft_strncmp(var->current->content[0], "export", 7) == 0)
		var->env = command_export(var, var->current->content[1]);
	else if (ft_strncmp(var->current->content[0], "unset", 6) == 0)
		var->env = command_unset(var, var->current->content[1]);
	else if (ft_strncmp(var->current->content[0], "cd", 3) == 0)
		command_cd(var, var->current->content[1]);
	else if (ft_strncmp(var->current->content[0], "exit", 5) == 0)
		command_exit(var);
	else if (ft_strncmp(var->current->content[0], "echo", 5) == 0)
		command_echo(var->current);
	else if (ft_strncmp(var->current->content[0], "env", 4) == 0)
		command_env(var);
	else if (ft_strncmp(var->current->content[0], "pwd", 4) == 0)
		command_pwd(var);
	else
		return ;
	return (free_all(var), exit(EXIT_SUCCESS));
}

void	first_cmd(t_var *var)
{
	var->current = var->list;
	if (pipe(var->pfd) == -1)
		return (perror("pipe"), free_all(var), exit(EXIT_FAILURE));
	var->pid = fork();
	if (var->pid == 0)
	{
		dup2(var->pfd[WRITE_END], STDOUT_FILENO);
		close_pipe(var->pfd);
		redirect_or_exit(var);
		exec_builtins(var);
		exec_system_commands(var);
	}
	close(var->pfd[WRITE_END]);
	var->in_fd = var->pfd[READ_END];
	var->current = get_next_node(var->current, PIPE, END)->next;
}

void	middle_cmd(t_var *var)
{
	if (pipe(var->pfd) == -1)
		return (perror("pipe"), free_all(var),
			close(var->in_fd), exit(EXIT_FAILURE));
	var->pid = fork();
	if (var->pid == 0)
	{
		dup2(var->in_fd, STDIN_FILENO);
		close_in_and_out(var);
		dup2(var->pfd[WRITE_END], STDOUT_FILENO);
		close_pipe(var->pfd);
		redirect_or_exit(var);
		exec_builtins(var);
		exec_system_commands(var);
	}
	close_in_and_out(var);
	close(var->pfd[WRITE_END]);
	var->in_fd = var->pfd[READ_END];
	var->current = get_next_node(var->current, PIPE, END)->next;
}

void	last_cmd(t_var *var)
{
	var->pid = fork();
	if (var->pid == 0)
	{
		dup2(var->pfd[READ_END], STDIN_FILENO);
		close(var->pfd[READ_END]);
		redirect_or_exit(var);
		exec_builtins(var);
		exec_system_commands(var);
	}
	close_in_and_out(var);
	close(var->pfd[READ_END]);
	var->in_fd = STDIN_FILENO;
}

void	wait_children(t_var *var)
{
	int		status;
	int		i;
	pid_t	pid;

	pid = 0;
	i = 0;
	status = var->status;
	signal(SIGINT, &sigint_wait);
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
	{
		var->status = 128 + WTERMSIG(var->status);
		write(STDOUT_FILENO, "\n", 1);
	}
	signal(SIGINT, &handle_sigint);
}
