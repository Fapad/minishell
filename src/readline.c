#include "../inc/minishell.h"

int	token_type(char c)
{
	if (strchr("|"))

}

t_token	*add_token(t_var *var, char *str)
{
	t_token	*new;

	new = malloc(sizeof (t_token));
	if (!new)
		return (NULL);
	new->str = str;
	new->type = token_type(*str);
	new->left = var->tree;
	new->right = NULL;
}

int	parser(t_var *var, char *line)
{
	t_token	*new;
	ssize_t	i;

	var->tree = NULL;
	i = 0;
	while (var->line[i])
	{
		while (ft_iswhitespace(var->line[i]))
			i++;
		if (!var->line[i])
			return (0);
		new = add_token(var, var->line + i);
		if (!new)
			return (0);
		while (ft_isprint(var->line[i]))
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
