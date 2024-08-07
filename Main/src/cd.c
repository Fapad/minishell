/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 18:56:14 by bszilas           #+#    #+#             */
/*   Updated: 2024/08/07 15:32:50 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	valid_dotdot_path(char *path)
{
	while (*path)
	{
		if (!ft_strncmp(path, "../", 3))
			path += 3;
		else if (!ft_strncmp(path, "..", 3) || !ft_strncmp(path, "./", 2))
			path += 2;
		else if (!ft_strncmp(path, ".", 2))
			path += 1;
		else
			return (false);
	}
	return (true);
}

void	update_env_after_cd(t_var *var, char *env_var_name, char *path)
{
	char	*str;
	size_t	size;

	size = ft_strlen(env_var_name) + ft_strlen(path) + 1;
	str = malloc(size * sizeof (char));
	if (!str)
		return ;
	ft_strlcpy(str, env_var_name, size);
	ft_strlcat(str, path, size);
	var->env = command_export(var, str);
	free(str);
	if (!var->env)
		restore_environment(var);
}

void	cd_home(t_var *var, char *path)
{
	path = ft_getenv(var->env, "HOME");
	if (!path)
	{
		ft_putendl_fd("cd: HOME not set", STDERR_FILENO);
		return (status_1(var));
	}
	command_cd(var, path);
}

bool	too_many_arguments(t_var *var, t_node *cmd)
{
	int	i;

	i = 0;
	if (cmd && cmd->content)
	{
		while (cmd->content[i])
			i++;
	}
	if (i > 2)
	{
		error_msg(var, ": too many arguments", 1);
		return (true);
	}
	return (false);
}

void	cd_dotdot_for_istvan(t_var *var, char *path)
{
	char	*rv_path;
	char	*cwd;

	if (var->cwd && var->cwd[ft_strlen(var->cwd) - 1] == '/')
		rv_path = ft_strjoin_nofree(var->cwd, path);
	else
		rv_path = ft_strjoin_three(var->cwd, path);
	if (!rv_path)
		return (perror("cd"), status_1(var));
	free(var->cwd);
	chdir(path);
	cwd = getcwd(NULL, PATH_MAX);
	if (!cwd)
	{
		ft_putstr_fd("cd: error retrieving current directory:", STDERR_FILENO);
		perror(" getcwd: cannot access parent directories");
		var->cwd = rv_path;
	}
	else
	{
		var->cwd = cwd;
		free(rv_path);
	}
	update_env_after_cd(var, "OLDPWD=", ft_getenv(var->env, "PWD"));
	update_env_after_cd(var, "PWD=", var->cwd);
}

void	command_cd(t_var *var, char *path)
{
	if (!path)
		return (cd_home(var, path));
	if (valid_dotdot_path(path))
	{
		cd_dotdot_for_istvan(var, path);
		return ;
	}
	if (chdir(path) == -1)
	{
		ft_putstr_fd("cd: ", STDERR_FILENO);
		perror(path);
		return (status_1(var));
	}
	update_env_after_cd(var, "OLDPWD=", ft_getenv(var->env, "PWD"));
	free(var->cwd);
	var->cwd = getcwd(NULL, PATH_MAX);
	if (var->cwd)
		update_env_after_cd(var, "PWD=", var->cwd);
	var->status = EXIT_SUCCESS;
}
