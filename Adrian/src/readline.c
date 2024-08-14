/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 10:36:13 by ajovanov          #+#    #+#             */
/*   Updated: 2024/08/13 18:28:57 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	init_var(t_var *var, int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	var->stack_env = envp;
	var->tokens = NULL;
	var->last_token = NULL;
	var->current = NULL;
	var->list = NULL;
	var->line = NULL;
	var->env = NULL;
	var->splitted_path = NULL;
	var->exec_cmd = NULL;
	var->cwd = getcwd(NULL, PATH_MAX);
	malloc_envps(var, envp);
	var->status = 0;
	var->loop = true;
}

void	check_received_signal(t_var *var)
{
	extern sig_atomic_t	g_signal;

	if (g_signal)
	{
		var->status = g_signal + 128;
		g_signal = 0;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_var	var;

	setup_signal_handlers(&var);
	init_var(&var, argc, argv, envp);
	while (var.loop)
	{
		if (TESTER)
		{
			char *line = get_next_line(0);
			var.line = ft_strtrim(line, "\n");
			free(line);
		}
		else
			var.line = readline(PROMPT);
		if (!var.line)
			break ;
		sigint_handler_non_interactive_mode(&var);
		if (*var.line)
			add_history(var.line);
		var.tokens = tokenize(&var);
		if (parse_tokens(&var) == true)
			execute(&var);
		exec_cleanup(&var);
		sigint_handler_interactive_mode(&var);
	}
	if (!TESTER)
		ft_printf("exit\n");
	rl_clear_history();
	free_all(&var);
	return (var.status);
}
