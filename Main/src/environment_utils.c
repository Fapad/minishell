/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 15:06:25 by bszilas           #+#    #+#             */
/*   Updated: 2024/08/07 13:53:44 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

size_t	envp_string_count(char **envp)
{
	size_t string_count;

	string_count = 0;
	while (envp && envp[string_count])
		string_count++;
	return (string_count);
}

int	get_shlvl(char *str)
{
	size_t	i;

	if (!str)
		return (0);
	skip_whitespace(&str);
	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (ft_strlen(&str[i]) > digits_count(LONG_MAX, 10))
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i++]))
			return (0);
	}
	return (ft_atoi(str));
}

char	**set_shlvl(t_var *var, char *str)
{
	int		lvl;
	char	envvar_str[BUFFER_SIZE];

	ft_bzero(envvar_str, BUFFER_SIZE);
	ft_strlcpy(envvar_str, "SHLVL=", BUFFER_SIZE);
	lvl = get_shlvl(str) + 1;
	str = ft_itoa(lvl);
	if (!str || lvl < 1)
		ft_strlcat(envvar_str, "0", BUFFER_SIZE);
	else if (lvl > 999)
	{
		ft_putstr_fd("warning: shell level (", STDERR_FILENO);
		ft_putstr_fd(str, STDERR_FILENO);
		ft_putendl_fd(") too high, resetting to 1", STDERR_FILENO);
		ft_strlcat(envvar_str, "1", BUFFER_SIZE);
	}
	else
		ft_strlcat(envvar_str, str, BUFFER_SIZE);
	free(str);
	return (command_export(var, envvar_str));
}

int	valid_identifier(t_var *var, char *str)
{
	size_t	i;

	i = 0;
	if (str[i] == '=')
		return (invalid_identifier(var, str), false);
	while (str[i])
	{
		if (str[i] == '=')
			break ;
		if (i == 0 && !ft_isalpha(str[i]) && str[i] != '_')
			return (invalid_identifier(var, str), false);
		if (i > 0 && !ft_isalnum(str[i]) && str[i] != '_')
		{
			invalid_identifier(var, str);
			return (false);
		}
		i++;
	}
	return (true);
}

int	existing_env_var(char **env, char *str)
{
	char	*tmp;
	char	*var;

	tmp = ft_strchr(str, '=');
	if (!tmp)
		return (false);
	tmp[0] = '\0';
	var = ft_getenv(env, str);
	tmp[0] = '=';
	return (var != NULL);
}
