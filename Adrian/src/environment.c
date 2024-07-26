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

void	command_env(char **envp)
{
	size_t	i;

	i = 0;
	while (envp[i])
		ft_printf("%s\n", envp[i++]);
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

char	**command_unset(char **env, char *line)
{
	size_t	i;
	size_t	to_compare;
	size_t	start;
	size_t	len;
	char	**new_env;

	i = 0;
	to_compare = 0;
	while (line[i] != 32 && line[i])
		i++;
	while (line[i] == 32 && line)
		i++;
	start = i;
	while (line[i] > 32)
	{
		i++;
		to_compare++;
	}
	len = envp_string_count(env);
	if (!len)
		return (free(env), NULL);
	new_env = malloc(len * sizeof (char *));
	if (!new_env)
		return (free(env), NULL);
	i = 0;
	while (env[i])
	{
		new_env[i] = env[i];
		if (strncmp(env[i], line + start, to_compare) == 0)
		{
			printf("%s\n", env[i]);
			free(env[i++]);
			while (env[i])
			{
				new_env[i - 1] = env[i];
				i++;
			}
			new_env[i - 1] = NULL;
			return (free(env), new_env);
		}
		i++;
	}
	return (free(new_env), env);
}

char	**malloc_envps(char **envp)
{
	char	**heap_envp;
	size_t	len;
	size_t	i;

	len = envp_string_count(envp);
	heap_envp = malloc((len + 1) * sizeof (char *));
	if (!heap_envp)
		return (NULL);
	i = 0;
	while (i < len)
	{
		heap_envp[i] = ft_strdup(envp[i]);
		if (!heap_envp[i])
			return (free_string_array(heap_envp), NULL);
		i++;
	}
	heap_envp[i] = NULL;
	return (heap_envp);
}

char	**command_export(char **old_envp, char *str)
{
	char	**new_envp;
	size_t	len;
	size_t	i;

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
	char *nl = ft_strchr(str, '\n'); //DELETE FOR MINISHELL!!!!!!!!!
	*nl = 127; //DELETE FOR MINISHELL!!!!!!!
	new_envp[i] = ft_strdup(str);
	if (!str)
		return (free(new_envp), NULL);
	new_envp[i + 1] = NULL;
	return (new_envp);
}