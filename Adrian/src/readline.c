/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 10:36:13 by ajovanov          #+#    #+#             */
/*   Updated: 2024/08/04 12:05:54 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

sig_atomic_t signal_received = 0;

void	init_var(t_var *var, int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	var->stack_env = envp;
	var->tokens = NULL;
	var->current = NULL;
	var->list = NULL;
	var->line = NULL;
	var->env = NULL;
	var->splitted_path = NULL;
	var->exec_cmd = NULL;
	var->cwd = getcwd(NULL, PATH_MAX);
	malloc_envps(var, envp);
	var->status = 0;
	var->loop = true;
}

void	malloc_envps(t_var *var, char **envp)
{
	char	*str;
	size_t	len;
	size_t	i;

	len = envp_string_count(envp);
	var->env = malloc((len + 1) * sizeof (char *));
	if (!var->env)
		return (perror("exiting"), free_all(var), exit(EXIT_FAILURE));
	i = 0;
	while (i < len)
	{
		var->env[i] = ft_strdup(envp[i]);
		if (!var->env[i])
			return (perror("exiting"), free_all(var), exit(EXIT_FAILURE));
		i++;
	}
	var->env[i] = NULL;
	str = ft_getenv(var->env, "SHLVL");
	var->env = set_shlvl(var, str);
	if (!var->env)
		return (perror("exiting"), free_all(var), exit(EXIT_FAILURE));
}

void	check_signal_received(t_var *var)
{
	if (signal_received)
		var->status = signal_received + 128;
	signal_received = 0;
}

char	*ft_strjoin_threee(char *s1, char *s2)
{
	int		len1;
	int		len2;
	char	*res;
	int		i;

	i = 0;
	len1 = 0;
	len2 = 0;
	if (!s1 && s2)
		return (NULL);
	if (s1)
		len1 = ft_strlen(s1);
	if (s2)
		len2 = ft_strlen(s2);
	res = (char *)malloc((len1 + len2 +2) * sizeof(char));
	if (!res)
		return (NULL);
	while (s1 && *s1)
		res[i++] = *s1++;
	res[i++] = ' ';
	while (s2 && *s2)
		res[i++] = *s2++;
	res[i] = '\0';
	return (res);
}
bool ends_with_pipe(char *line)
{
	size_t	len;

	if (line == NULL)
		return (false);
	len = ft_strlen(line);
	if (len == 0)
		return (false);
	len = len - 1;
	while (len > 0 && isspace(line[len]))
		len--;
	if (line[len] == '|')
	{
		printf("true\n");
		return (true);
	}
	return (false);
	
}

int	main(int argc, char **argv, char **envp)
{
	t_var	var;
	char	*test;

	setup_signal_handlers();
	init_var(&var, argc, argv, envp);
	while (var.loop)
	{
		var.line = readline(PROMPT);
		if (!var.line)
			break ;
		
		while (ends_with_pipe(var.line))
		{
			test = readline(PROMPT); //get_next_line(0);
			if (!test)
				break ;
			if (*test)
					var.line = ft_strjoin_threee(var.line, test);
			free(test);
			test = NULL;
		}
		if (*var.line)
			add_history(var.line);
		check_signal_received(&var);
		var.tokens = tokenize(&var);
		if (parse_tokens(&var))
			execute(&var);
		else
			status_1(&var);
		exec_cleanup(&var);
	}
	ft_printf("exit\n");
	rl_clear_history();
	free_all(&var);
	return (var.status);
}
