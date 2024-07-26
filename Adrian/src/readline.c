/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 10:36:13 by ajovanov          #+#    #+#             */
/*   Updated: 2024/07/25 19:47:20 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	**malloc_envps(char **envp)
{
	char	**heap_envp;
	size_t	len;
	size_t	i;

	len = envp_string_count(envp);
	heap_envp = malloc((len + 1) * sizeof (char *));
	if (!heap_envp)
		exit(EXIT_FAILURE);
	i = 0;
	while (i < len)
	{
		heap_envp[i] = ft_strdup(envp[i]);
		if (!heap_envp[i])
			return (free_string_array(heap_envp), exit(EXIT_FAILURE), NULL);
		i++;
	}
	heap_envp[i] = NULL;
	return (heap_envp);
}

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
