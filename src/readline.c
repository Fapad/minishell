#include "../inc/minishell.h"

int	token_type(char *str)
{
	if (*str == '\0')
		return (END);
	if (*str == '|')
		return (PIPE);
	if (*str == '<')
	{
		if (*str + 1 == '<')
			return (HEREDOC);
		else
			return (IN_R);
	}
	if (*str == '>')
	{
		if (*str + 1 == '>')
			return (OUT_APPEND);
		else
			return (OUT_R);
	}
	return (CMD);
}



t_token	*add_token(t_var *var, char *str)
{
	t_token	*new;

	new = malloc(sizeof (t_token));
	if (!new)
		return (NULL);
	new->str = str;
	new->type = token_type(str);
	add_to_tree(new, var);
}

int	parser(t_var *var, char *line)
{
	t_token	*new;
	ssize_t	i;

	var->tree = NULL;
	i = 0;
	while (line[i])
	{
		while (ft_iswhitespace(line[i]))
			i++;
		new = add_token(var, line + i);
		if (!new)
			return (0);
		while (ft_isprint(line[i]))
			i++;
		
	}
}

int	main(void)
{
	t_var	var;

	char *line = (char *)1;
	while (line)
	{
		line = readline("\033[1;32mminishell>\033[0m ");
		if (!line)
			break;
		parser(&var, line);
		print_tokens(&var);
		free(line);
	}
	return (0);
}
