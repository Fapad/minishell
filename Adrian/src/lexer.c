/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajovanov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 10:35:08 by ajovanov          #+#    #+#             */
/*   Updated: 2024/07/22 10:35:10 by ajovanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

t_token	*create_token(int type, const char *str)
{
	t_token	*token;
	int	i;

	char	**evnp = __environ;
	i = 0;
	//int b = 1;
	token = (t_token *)malloc(sizeof(t_token));
	token->type = type;
	token->left = NULL;
	token->right = NULL;
	token->str = NULL;
/*  	if (token->type == 10 || token->type == 300)
	{
		int q = 0;
		if (str[0] == '\"')
		{
			char	**dest = ft_split(str, ' ')
			while (dest[q] != NULL)
			{
				if (dest[q][0] == '$')
				{
					int len = ft_strlen(dest[q]);
					while (evnp[i])
					{
						if (ft_strncmp(evnp[i], (str + 1), (len - 1)) == 0)
						{
							char *to_join = malloc(ft_strlen(evnp[i]) + 1);
							ft_strcpy(to_join, evnp[i]);
						}
						i++;
					}
					
				}
				else
					ft_strjoin(token->str, dest[q])
				q++;
			}




 */









	/* 		while (str[b] != '\0')
			{
				int sec = b;
				if (str[b] == '$')
				{
					while (str[sec] != 32 && str[sec] != '\0' && str[sec] != '\"')
						sec++;
					i = 0;

					while (evnp[i])
					{
						if (ft_strncmp(evnp[i], (str + b + 1), (sec - b - 1)) == 0)
						{	
							if (token->str == NULL)
								token->str = ft_strdup(evnp[i]);
							else 
								token->str = ft_strjoin(token->str, evnp[i]);
						}
						i++;
					} 					
				}
				b++;
			}
		} */
		while (evnp[i])
		{
		if (ft_strncmp(evnp[i], str + 1, (ft_strlen(str) - 1)) == 0)
			token->str = ft_strdup(evnp[i]);
		i++;
		}


	 
	if (token->str == NULL)
		token->str = ft_strdup(str);
	token->str_len = ft_strlen(str);
	return (token);
}

void	add_token(t_token **head, t_token **current,
						const char **start)
{
	t_token		*new_token;
	const char	*end;
	int			type;

	end = *start;
	type = identify_token_type(start, &end);
	new_token = create_token(type, strndup(*start, end - *start));
	if (!*head)
		*head = new_token;
	else
		(*current)->right = new_token;
	*current = new_token;
	*start = end;
}

t_token	*tokenize(const char *input)
{
	const char	*start;
	t_token		*head;
	t_token		*current;

	head = NULL;
	current = NULL;
	start = input;
	while (*start != '\0')
	{
		skip_whitespace(&start);
		if (*start == '\0')
			break ;
		add_token(&head, &current, &start);
	}
	return (head);
}
