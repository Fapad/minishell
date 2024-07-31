/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 10:36:13 by ajovanov          #+#    #+#             */
/*   Updated: 2024/07/31 17:30:36 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	init_var(t_var *var, int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	var->stack_env = envp;
	var->tokens = NULL;
	var->current = NULL;
	var->list = NULL;
	var->line = NULL;
	var->env = NULL;
	var->cwd = getcwd(NULL, PATH_MAX);
	malloc_envps(var, envp);
	var->splitted_path = NULL;
	var->pid = 0;
	var->pfd[READ_END] = 0;
	var->pfd[WRITE_END] = 0;
	var->in_fd = STDIN_FILENO;
	var->out_fd = STDOUT_FILENO;
	var->cmds = 0;
	var->status = 0;
}

int	main(int argc, char **argv, char **envp)
{
	t_var	var;

	setup_signal_handlers();
	init_var(&var, argc, argv, envp);
	while (42)
	{
		var.line = readline(PROMPT);
		if (!var.line)
			return (rl_clear_history(), free_all(&var), 0);
		if (*var.line)
		{
			add_history(var.line);
			var.tokens = tokenize(&var);
			if (parse_tokens(&var))
				execute(&var);
			else
				var.status = EXIT_FAILURE;
		}
		free(var.line);
		var.line = NULL;
	}
	ft_printf("exit\n");
	rl_clear_history();
	free_all(&var);
	return (0);
}
