/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajovanov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 09:55:54 by ajovanov          #+#    #+#             */
/*   Updated: 2024/04/08 14:55:03 by ajovanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(void)
{
	char	*line;
	t_token	*tokens;

	while (1)
	{
		line = readline("\033[1;32mminishell>\033[0m ");
		if (!line)
			break ;
		tokens = tokenize(line);
		print_tokens(tokens);
		add_history(line);
		free(line);
	}
	return (0);
}

/* int main()
{
    const char *input = "ls -l | grep minishell > output.txt";
    t_token *tokens = tokenize(input);
    print_tokens(tokens);

    t_token *current = tokens;
    while (current)
    {
        t_token *next = current->right;
        free_token(current);
        current = next;
    }
	return (0);
} */
