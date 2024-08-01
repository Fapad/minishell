/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 19:46:32 by bszilas           #+#    #+#             */
/*   Updated: 2024/08/01 12:55:56 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	exec_system_commands(t_var *var)
{
	char	*executable_command;
	
	executable_command = get_cmd(var);
	if (!executable_command)
		return (perror(var->current->content[0]), free_all(var), exit(1));
	free_string_array(var->splitted_path);
	var->splitted_path = NULL;
	execve(executable_command, var->current->content, var->env);
	perror("execve");
	free(executable_command);
	free_all(var);
	exit(EXIT_FAILURE);
}

void	exec_other_commands(t_var *var)
{
	if (ft_strncmp(var->current->content[0], "echo", 5) == 0)
		command_echo(var->current);
	else if (ft_strncmp(var->current->content[0], "env", 4) == 0)
		command_env(var);
	else if (ft_strncmp(var->current->content[0], "pwd", 4) == 0)
		command_pwd(var);
	else
		exec_system_commands(var);
	free_all(var);
	exit(EXIT_SUCCESS);
}

int cd_export_exit_or_unset(t_var *var)
{
	if (ft_strncmp(var->current->content[0], "export", 7) == 0)
		var->env = command_export(var, var->current->content[1]);
	else if (ft_strncmp(var->current->content[0], "unset", 6) == 0)
		var->env = command_unset(var->env, var->current->content[1]);
	else if (ft_strncmp(var->current->content[0], "cd", 3) == 0)
		command_cd(var);
	else if (ft_strncmp(var->current->content[0], "exit", 5) == 0)
		command_exit(var);
	else
		return (false);
	if (!var->env)
		restore_environment(var);
	return (true);
}

void	one_simple_cmd(t_var *var)
{
	if (!open_files_in_parent(var))
		return (status_1(var));
	var->current = get_next_node(var->list, CMD, END);
	if (!var->current)
		return ;
	if (cd_export_exit_or_unset(var))
		return ;
	var->pid = fork();
	if (var->pid == 0)
	{
		open_files_or_exit(var);
		file_redirect(var);
		exec_other_commands(var);
	}
	wait_children(var);
}

void	execute(t_var *var)
{
	int	i;
	
	if (!write_here_docs(var))
		return ;
	var->status = EXIT_SUCCESS;
	var->cmds = count_node_types(var->list, PIPE | END);
	if (var->cmds == 1)
		return (one_simple_cmd(var));
	i = 0;
	while (i < var->cmds)
	{
		if (i == 0)
			first_cmd(var);
		else if (i == var->cmds - 1)
			last_cmd(var);
		else
			middle_cmd(var);
		i++;
	}
	wait_children(var);
}
