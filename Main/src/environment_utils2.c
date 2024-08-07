/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_utils2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 15:10:04 by bszilas           #+#    #+#             */
/*   Updated: 2024/08/05 19:14:44 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	cat_char_to_str(char *str, char c, size_t len)
{
	char	to_cat[2];

	to_cat[0] = c;
	to_cat[1] = 0;
	ft_strlcat(str, to_cat, len + 1);
}

char	*find_next_smallest(char **arr, char *current, char *max)
{
	size_t	len;
	size_t	i;
	char	*next;

	next = max;
	i = 0;
	while (arr[i])
	{
		len = ft_strlen(arr[i]) + 1;
		if (ft_strncmp(arr[i], current, len) > 0 && \
		ft_strncmp(arr[i], next, len) < 0)
			next = arr[i];
		i++;
	}
	return (next);
}

void	print_environment(t_var *var)
{
	char	*next;
	char	nul[1];
	char	max[2];
	size_t	len;
	size_t	i;

	if (!var->env || !var->env[0])
		return ;
	len = envp_string_count(var->env);
	cat_char_to_str(max, CHAR_MAX, 1);
	nul[0] = 0;
	next = nul;
	i = 0;
	while (i < len)
	{
		next = find_next_smallest(var->env, next, max);
		ft_printf("export ");
		ft_printf("%s\n", next);
		i++;
	}
}