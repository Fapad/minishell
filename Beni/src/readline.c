/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 10:36:13 by ajovanov          #+#    #+#             */
/*   Updated: 2024/08/01 19:46:02 by bszilas          ###   ########.fr       */
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
	var->status = 0;
	var->loop = true;
}

int	main(int argc, char **argv, char **envp)
{
	t_var	var;

	setup_signal_handlers();
	init_var(&var, argc, argv, envp);
	while (var.loop)
	{
		var.line = readline(PROMPT);
		if (!var.line)
			break ;
		if (*var.line)
			add_history(var.line);
		var.tokens = tokenize(&var);
		if (parse_tokens(&var))
			execute(&var);
		else
			status_1(&var);
		exec_cleanup(&var);
	}
	ft_printf("exit\n");
	rl_clear_history();
	free_all(&var);
	return (var.status);
}
