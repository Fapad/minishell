/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 19:46:32 by bszilas           #+#    #+#             */
/*   Updated: 2024/07/27 21:59:33 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	command_echo(t_node *list)
{
	//	printf("%s", var->list->content[1]);
	//printf("%s\n", var->list->content[1]);
	//printf("%s%s\n", PROMPT, var->list->content[1]);
	ft_printf("%s", list->content[1]);
		
	

}

int cd_export_exit_or_unset(t_var *var)
{
	t_node *cmd;
	
	cmd = var->current;
	if (ft_strncmp(cmd->content[0], "export", 7) == 0)
		var->env = command_export(var->env, cmd->content[1]);
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

void	exec_other_builtin(t_var *var)
{
	t_node *cmd;
	
	cmd = var->current;
	if (ft_strncmp(cmd->content[0], "echo", 5) == 0)
		command_echo(cmd);
	else if (ft_strncmp(cmd->content[0], "env", 4) == 0)
		command_env(var);
	else if (ft_strncmp(cmd->content[0], "pwd", 4) == 0)
		command_pwd();
	else
		return ;
	close_in_and_out(var);
	free_all(var);
	exit(EXIT_SUCCESS);
}

int	no_pipes(t_node *list)
{
	t_node *temp;

	temp = list;
	while (temp)
	{
		if (temp->type == PIPE)
			return (false);
		temp = temp->next;
	}
	return (true);
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
		exec_other_builtin(var);
		exit(EXIT_SUCCESS);
	}
	if (wait(&var->status) == -1)
		perror("wait");
}

void	execute(t_var *var)
{
	write_here_docs(var);
	if (no_pipes(var->list))
		one_simple_cmd(var);
	var->cmds = count_node_types(var->list, PIPE | END);
		/* else
		return ;
	}
	var->cmds = count_commands(var->list);
	node = var->list;
	while (var->cmds--)
	{
		if (var->cmds && pipe(var->pfd) == -1)
			return (free_everything(), exit(EXIT_FAILURE));
		pid = fork();
		if (pid == 0)
		{
			redir_or_exit();
			exec_builtin();
		}
		if (var->cmds)
			close_pipe();
		go_to_next_cmd(node);
	}
	wait_children(var); */
}