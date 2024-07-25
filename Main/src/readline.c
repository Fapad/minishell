/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 10:36:13 by ajovanov          #+#    #+#             */
/*   Updated: 2024/07/25 12:21:40 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	main(int argc, const char *argv[], const char *envp[])
{
	char	*line;
	t_token	*tokens;
	t_var	var;

	//setup_signal_handlers();
	while (1)
	{
		line = readline(PROMPT);
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (*line)
		{
			tokens = tokenize(line);
			print_tokens(tokens);
			var.tokens = tokens;
			var.line = line;
			var.list = NULL;
			if (parse_tokens(&var))
				print_exec_list(var.list);
			
			add_history(line);
			free_linked_lists(&var);
		}
		free(line);
	}
	rl_clear_history();
	return (0);
}
