/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_utils2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 15:10:04 by bszilas           #+#    #+#             */
/*   Updated: 2024/08/13 13:38:29 by bszilas          ###   ########.fr       */
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

void	print_env_string(char *str, int fd)
{
	int		i;
	int		flag;
	
	i = 0;
	flag = 0;
	write(fd, "declare -x ", ft_strlen("declare -x "));	
	while (str[i])
	{
		write(fd, &str[i], 1);
		if (str[i] == '=' && flag == 0)
		{
			flag = 1;
			write(fd, "\"", 1);
		}
		i++;
	}
	write(fd, "\"", 1);
	write(fd, "\n", 1);

}

int	open_fd_export(t_var *var)
{
	t_node *outfile;
	int		fd;
	
	outfile = get_next_node(var->list, O_APPEND | O_TRUNC, PIPE | END);
	while (outfile)
	{
		fd = open(outfile->content[FILENAME], O_WRONLY | outfile->type);
		outfile = get_next_node(outfile->next, O_APPEND | O_TRUNC, PIPE | END);
		if (outfile)
			close(fd);
	}
	return (fd);
}


void	print_environment(t_var *var)
{
	char	*next;
	char	nul[1];
	char	max[2];
	size_t	len;
	size_t	i;
	int		fd;

	len = envp_string_count(var->env);
	ft_bzero(max, 2);
	cat_char_to_str(max, CHAR_MAX, 1);
	nul[0] = 0;
	next = nul;
	i = 0;
	
	fd = open_fd_export(var);
	while (i < len)
	{
		next = find_next_smallest(var->env, next, max);
		if (ft_strnstr(next, "_=", 2) == next)
		{
			next = find_next_smallest(var->env, next, max);
			if (++i == len)
				break ;
		}
		print_env_string(next, fd);
		i++;
	}
}

char	*last_arg(char **args)
{
	size_t	i;

	i = 0;
	while (args[i] && args[i + 1])
		i++;
	return (args[i]);
}

void	update_last_cmd(t_var *var)
{
	t_node	*cmd;
	char	*str;
	char	*envvar;

	cmd = get_next_node(var->current, CMD, PIPE | END);
	if (!cmd)
		str = NULL;
	else
		str = last_arg(cmd->content);
	envvar = ft_strjoin_nofree("_=", str);
	if (!envvar)
		return (perror("update _"));
	var->env = command_export(var, envvar);
	free(envvar);
	if (!var->env)
		restore_environment(var);
}
