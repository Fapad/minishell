/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 10:36:13 by ajovanov          #+#    #+#             */
/*   Updated: 2024/08/08 17:47:04 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

sig_atomic_t	signal_received = 0;

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

void	malloc_envps(t_var *var, char **envp)
{
	char	*str;
	size_t	len;
	size_t	i;

	len = envp_string_count(envp);
	var->env = malloc((len + 1) * sizeof (char *));
	if (!var->env)
		return (perror("exiting"), free_all(var), exit(EXIT_FAILURE));
	i = 0;
	while (i < len)
	{
		var->env[i] = ft_strdup(envp[i]);
		if (!var->env[i])
			return (perror("exiting"), free_all(var), exit(EXIT_FAILURE));
		i++;
	}
	var->env[i] = NULL;
	str = ft_getenv(var->env, "SHLVL");
	var->env = set_shlvl(var, str);
	if (!var->env)
		return (perror("exiting"), free_all(var), exit(EXIT_FAILURE));
}

void	check_signal_received(t_var *var)
{
	if (signal_received)
		var->status = signal_received + 128;
	signal_received = 0;
}

int	main(int argc, char **argv, char **envp)
{
	t_var	var;

	setup_signal_handlers();
	init_var(&var, argc, argv, envp);
	while (var.loop)
	{
		if (TESTER)
		{
			if (isatty(fileno(stdin)))
				var.line = readline(PROMPT);
			else
			{
				var.line = get_next_line(0);
				var.line = ft_strtrim(var.line, "\n");
			}
		}
		else
			var.line = readline(PROMPT);
		if (!var.line)
			break ;
		if (*var.line)
			add_history(var.line);
		check_signal_received(&var);
		var.tokens = tokenize(&var);
		if (parse_tokens(&var))
			execute(&var);
		else
			status_1(&var);
		exec_cleanup(&var);
	}
	if (!TESTER)
		ft_printf("exit\n");
	rl_clear_history();
	free_all(&var);
	return (var.status);
}
