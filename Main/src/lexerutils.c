/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexerutils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 10:35:25 by ajovanov          #+#    #+#             */
/*   Updated: 2024/08/01 16:05:47 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	print_tokens(t_token *head)
{
	t_token	*current;

	current = head;
	while (current)
	{
		printf("Type: %d, Str: %s\n", current->type, current->str);
		current = current->right;
	}
}

void	skip_whitespace(char **input)
{
	while (**input == ' ')
		(*input)++;
}

void	ft_strncpy(char	*dest,const char *str, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && str[i])
	{
		dest[i] = str[i];
		i++;
	}
	dest[i] = '\0';

}

char *ft_strndup(const char *s, size_t n)
{
	char	*dest;

	if (!s)
		return (NULL);
	dest = malloc(n + 1);
	if (!dest)
		return (NULL);
	ft_strncpy(dest, s, n);
	return (dest);
}
