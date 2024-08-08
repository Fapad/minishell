/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 11:04:40 by bszilas           #+#    #+#             */
/*   Updated: 2024/08/08 18:33:09 by bszilas          ###   ########.fr       */
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
	ft_strlcpy(dest, str, to_compare + 2);
	ft_strlcpy((dest + to_compare), "=", to_compare + 2);
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
		if (ft_strncmp(old_envp[i], dest, to_compare + 1) == 0)
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

char	**change_var(char **env, char *str)
{
	char	*var;
	char	tmp;
	size_t	len;
	size_t	i;

	len = ft_strchr(str, '=') + 1 - str;
	tmp = str[len];
	str[len] = 0;
	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], str, len))
		{
			str[len] = tmp;
			len = ft_strlen(str);
			var = malloc((len + 1) * sizeof (char));
			if (!var)
				return (free_string_array(env), NULL);
			ft_strlcpy(var, str, len + 1);
			free(env[i]);
			env[i] = var;
			return (env);
		}
		i++;
	}
	str[len] = tmp;
	return (NULL);
}

char	**command_export(t_var *var, char *str)
{
	char	**new_envp;
	size_t	len;
	size_t	i;

	if (!str)
		return (print_environment(var), var->env);
	if (!valid_identifier(var, str))
		return (var->env);
	if (existing_env_var(var->env, str))
		return (change_var(var->env, str));
	len = envp_string_count(var->env);
	new_envp = malloc((len + 1 + 1) * sizeof (char *));
	if (!new_envp)
		return (free_string_array(var->env), NULL);
	i = 0;
	while (i < len)
	{
		new_envp[i] = var->env[i];
		i++;
	}
	free(var->env);
	new_envp[i] = ft_strdup(str);
	if (!str)
		return (free(new_envp), NULL);
	new_envp[i + 1] = NULL;
	return (new_envp);
}