/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 10:35:08 by ajovanov          #+#    #+#             */
/*   Updated: 2024/07/24 11:52:07 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_token	*create_token(int type, char *str)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	token->type = type;
	token->left = NULL;
	token->right = NULL;
	token->str = NULL;

	if (token->str == NULL)
		token->str = ft_strdup(str);
	token->str_len = ft_strlen(str);
	return (token);
}

size_t	single_quote_len(char *s, char *end, size_t *i)
{
	size_t	len;

	len = 0;
	while (s + len < end)
	{
		len++;
		if (s[len] == '\'')
		{
			*i += len;
			return (len - 1);
		}
	}
	return (1);
}

size_t	env_var_len(char *s, char *end, size_t *i)
{
	size_t	len;
	char	tmp;

	end = ++s;
	while (*end && !ft_strchr("\" $", *end))
		end++;
	tmp = *end;
	*end = '\0';
	len = ft_strlen(getenv(s));
	*end = tmp;
	*i += end - s;
	return (len);
}

size_t	double_qoute_len(char *s, char *end, size_t *i)
{
	size_t	envv_len;
	size_t	len;
	size_t	j;

	len = 0;
	j = 1;
	while (s + j < end)
	{
		envv_len = 1;
		if (s[j] == '$' && s[j + 1] && s[j + 1] != '\"')
			envv_len = env_var_len(s + j, end, &j);
		if (s[j] == '\"')
		{
			*i += j;
			return (len);
		}
		len += envv_len;
		j++;
	}
	return (1);
}

size_t	interpreted_str_len(char *start, char *end)
{
	size_t	i;
	size_t	len;
	
	i = 0;
	len = 0;
	while (start + i < end)
	{
		if (start[i] == '\'')
			len += single_quote_len(start + i, end, &i);
		else if (start[i] == '\"')
			len += double_qoute_len(start + i, end, &i);
		else if (start[i] == '$' && start[i + 1])
			len += env_var_len(start + i, end, &i);
		else
			len++;
		i++;
	}
	return (len);
}

char	*copy_intrd_str(char *str, char *start, char *end, size_t len)
{
	size_t	i;
	char	*ptr;

	str = malloc((len + 1) * sizeof (char));
	if (!str)
		return (NULL);
	i = 0;
	while (start < end)
	{
		if (*start == '\'' && identify_single_quotes(&start, &ptr))
		{
			*ptr = 0;
			ft_strlcat(str, start + 1, len + 1);
			*ptr = '\'';
			start = ptr + 1;
		}
		else if (start[i] == '\"')
			len += double_qoute_len(start + i, end, &i);
		else if (start[i] == '$' && start[i + 1])
			len += env_var_len(start + i, end, &i);
		else
			len++;
		i++;
	}
	return (str);
}

char	*malloc_token_str(char *start, char *end, int type)
{
	size_t	len;
	char	*str;

	str = NULL;
	len = end - start;
	if (type != INTERPRET)
		ft_printf("%u\n", len);
	else
	{
		len = interpreted_str_len(start, end);
		str = copy_intrd_str(str, start, end, len);
		ft_printf("%u\n", len);
	}
	return (str);
}

void	add_token(t_token **head, t_token **current, char **start)
{
	t_token		*new_token;
	char	*end;
	int			type;
	char		*str;

	end = *start;
	type = identify_token_type(start, &end);
	str = malloc_token_str(*start, end, type);
	new_token = create_token(type, str);
	if (!*head)
		*head = new_token;
	else
		(*current)->right = new_token;
	*current = new_token;
	*start = end;
}

t_token	*tokenize(char *input)
{
	char	*start;
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
	add_token(&head, &current, &start);
	return (head);
}
