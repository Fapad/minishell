/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 10:36:13 by ajovanov          #+#    #+#             */
/*   Updated: 2024/07/24 17:14:27 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void free_tokens(t_token *root)
{
	t_token *temp;

	while (root != NULL)
	{
		if (root->right != NULL)
		{
			temp = root->right;
			root->right = NULL;
		} 
		else
			temp = NULL;
		if (root->str != NULL)
			free(root->str);
		free(root);
		root = temp;
	}
}

int	main(void)
{
	char	*line;
	t_token	*tokens;

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
		add_history(line);
		free_tokens(tokens);
		free(line);
	}
	return (0);
}
