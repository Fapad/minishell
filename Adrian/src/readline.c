/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 10:36:13 by ajovanov          #+#    #+#             */
/*   Updated: 2024/07/26 11:05:36 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	main(int agrc, char **argv, char **envp)
{
	t_var	var;

	setup_signal_handlers();
	var.env = malloc_envps(envp);
	while (1)
	{
		var.line = readline(PROMPT);
		if (!var.line)
			return (rl_clear_history(), free_string_array(var.env), 0);
		if (*var.line)
		{
			var.tokens = tokenize(var.line);
			if (parse_tokens(&var))
			{
				print_exec_list(var.list);
				execute(&var);
			}
			add_history(var.line);
			free_linked_lists(&var);
		}
		free(var.line);
	}
	rl_clear_history();
	free_string_array(var.env);
	return (0);
}
