/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 10:36:13 by ajovanov          #+#    #+#             */
/*   Updated: 2024/07/22 20:23:39 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	main(void)
{
	char	*line;
	t_token	*tokens;
	t_var	var;

	setup_signal_handlers();
	while (1)
	{
		line = readline(PROMPT);
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		tokens = tokenize(line);
		print_tokens(tokens);
		var.tokens = tokens;
		var.line = line;
		var.list = NULL;
		parse_tokens(&var);
		print_exec_list(var.list);
		add_history(line);
		free(line);
	}
	return (0);
}
