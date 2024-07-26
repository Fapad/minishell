/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 19:46:32 by bszilas           #+#    #+#             */
/*   Updated: 2024/07/26 10:58:21 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	command_echo(t_node *list)
{
	int	i;

	i = 1;
	if (strcmp(list->content[1], "-n") == 0)
	{
		i++;
		while (list->content[i] != NULL)
		{
			ft_printf("%s", list->content[i]);
			if (list->content[i + 1] != NULL)
				ft_printf(" ");
			i++;
		}
		return ;
	}
	while (list->content[i] != NULL)
	{
		ft_printf("%s", list->content[i]);
		if (list->content[i + 1] != NULL)
			ft_printf(" ");
		i++;
	}
	ft_printf("\n");
		
	

}


int cd_export_exit_or_unset(t_var *var)
{
	t_node	*node;

	node = var->list;
	if (ft_strncmp(var->list->content[0], "echo", 5) == 0)
		command_echo(var->list);
	if (ft_strncmp(var->list->content[0], "export", 7) == 0)
		var->env = command_export(var->env, var->list->content[1]);
	if (ft_strncmp(var->list->content[0], "env", 4) == 0)
		command_env(var);
	if (ft_strncmp(var->list->content[0], "unset", 6) == 0)
		 var->env = command_unset(var);
	if (ft_strncmp(var->list->content[0], "pwd", 4) == 0)
		command_pwd();
	if (ft_strncmp(var->list->content[0], "cd", 3) == 0)
		command_cd(var);
	if (ft_strncmp(var->list->content[0], "exit", 5) == 0)
		command_exit(var);
	return (1);
}

int	no_pipes(t_node *list)
{
	t_node *temp;

	temp = list;
	while (temp)
	{
		if (temp->type == PIPE)
			return (1);
		temp = temp->next;
	}
	return (0);
}

void	execute(t_var *var)
{
	//t_node	*node;
	//pid_t	pid;

	
	//cd_export_exit_or_unset(var);
	
	//write_here_docs(var);
	
	if (no_pipes(var->list) == 0)
	{
		cd_export_exit_or_unset(var);
		/* if (cd_export_exit_or_unset(var))
			return ; */
	}
		/* else
		pid = fork();
		if (pid == 0)
		{
			redir_or_exit();
			exec_builtin();
			
		}
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