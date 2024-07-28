/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 11:04:40 by bszilas           #+#    #+#             */
/*   Updated: 2024/07/27 21:45:30 by bszilas          ###   ########.fr       */
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

void	malloc_envps(t_var *var, char **envp)
{
	size_t	len;
	size_t	i;

	len = envp_string_count(envp);
	var->env = malloc((len + 1) * sizeof (char *));
	if (!var->env)
		return (free_all(var), exit(EXIT_FAILURE));
	i = 0;
	while (i < len)
	{
		var->env[i] = ft_strdup(envp[i]);
		if (!var->env[i])
			return (free_all(var), exit(EXIT_FAILURE));
		i++;
	}
	var->env[i] = NULL;
}

char	**command_unset(char **old_envp, char *str)
{
	size_t	to_compare;
	size_t	len;
	char	*dest;
	char	**new_env;

	if (!str)
		return (old_envp);
	len = envp_string_count(old_envp);
	if (!len)
		return (old_envp);
	to_compare = 0;
	while (str[to_compare])
		to_compare++;
	dest = malloc(to_compare + 2);
	if (!dest)
		return (free(old_envp), NULL);
	strcpy(dest, str);
	strcpy((dest + to_compare), "=");
	new_env = malloc(len * sizeof (char *));
	if (!new_env)
		return (free(old_envp), free(dest), NULL);
	if (unset2(old_envp, dest, to_compare, new_env))
		return (free(old_envp), free(dest), new_env);
	return (free(new_env), free(dest), old_envp);
}

int  unset2(char **old_envp, char *dest, size_t to_compare, char **new_env)
{
	int	i;

	i = 0;
	while (old_envp[i])
	{
		new_env[i] = old_envp[i];
		if (strncmp(old_envp[i], dest, to_compare + 1) == 0)
		{
			free(old_envp[i++]);
			while (old_envp[i])
			{
				new_env[i - 1] = old_envp[i];
				i++;
			}
			new_env[i - 1] = NULL;
			return (true);
		}
		i++;
	}
	return (false);
}

int	command_export_util(char *str)
{
	size_t	i;

	i = 0;
	if (!str)
		return (true);
	while (str[i] != '=' && str[i])
	{
		if (!ft_isalpha(str[i]) && str[i] != '_')
		{
			return (true);
		}
		i++;
	}
	return (false);
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