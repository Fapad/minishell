/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 11:04:40 by bszilas           #+#    #+#             */
/*   Updated: 2024/07/26 11:06:42 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	command_env(t_var *var)
{
	size_t	i;

	i = 0;
	while (var->env[i])
		ft_printf("%s\n", var->env[i++]);
}

size_t	envp_string_count(char **envp)
{
	size_t string_count;

	string_count = 0;
	while (envp && envp[string_count])
		string_count++;
	return (string_count);
}

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

char	**command_unset(t_var *var)
{
	size_t	to_compare;
	size_t	len;
	char	*dest;
	char	**new_env;

	if (!var->list->content[1])
		return (var->env);
	to_compare = 0;
	len = envp_string_count(var->env);
	if (!len)
		return (var->env);
	while (var->list->content[1][to_compare])
		to_compare++;
	dest = malloc(to_compare + 2);
	if (!dest)
		return (NULL);
	strcpy(dest, var->list->content[1]);
	strcpy((dest + to_compare), "=");
	new_env = malloc(len * sizeof (char *));
	if (!new_env)
		return (free(var->env), NULL);
	if (command_unset_util(var, dest, to_compare, new_env) == 0)
		return (free(var->env), free(dest), new_env);
	return (free(new_env), free(dest), var->env);
}

int  command_unset_util(t_var *var, char *dest, size_t to_compare, char **new_env)
{
	int	i;

	i = 0;
	while (var->env[i])
	{
		new_env[i] = var->env[i];
		if (strncmp(var->env[i], dest, to_compare + 1) == 0)
		{
			free(var->env[i++]);
			while (var->env[i])
			{
				new_env[i - 1] = var->env[i];
				i++;
			}
			new_env[i - 1] = NULL;
			return (0);
		}
		i++;
	}
	return (1);
}

int	command_export_util(char	*str)
{
	int	i;

	i = 0;
	while (str[i] != '=' && str[i])
	{
		if (!isalnum(str[i]) && str[i] != '_')
		{
			return (1);
		}
		i++;
	}
	return (0);
}


char	**command_export(char **old_envp, char *str)
{
	char	**new_envp;
	size_t	len;
	size_t	i;

	if (command_export_util(str) == 1)
		return (old_envp);
	len = envp_string_count(old_envp);
	new_envp = malloc((len + 1 + 1) * sizeof (char *));
	if (!new_envp)
		return (free_string_array(old_envp), NULL);
	i = 0;
	
	while (i < len)
	{
		new_envp[i] = old_envp[i];
		i++;
	}
	free(old_envp);
	new_envp[i] = ft_strdup(str);
	if (!str)
		return (free(new_envp), NULL);
	new_envp[i + 1] = NULL;
	return (new_envp);
}