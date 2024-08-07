/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 16:05:03 by bszilas           #+#    #+#             */
/*   Updated: 2024/08/01 17:01:42 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	free_lists_and_path(t_var *var)
{
	free_linked_lists(var);
	free_string_array(var->splitted_path);
	var->splitted_path = NULL;
}

void	free_all(t_var *var)
{
	free_lists_and_path(var);
	free_string_array(var->env);
	var->env = NULL;
	free(var->cwd);
	var->cwd = NULL;
	if (var->was_pipe_on_end != 0)
		free(var->line);
}

void	free_linked_lists(t_var *var)
{
	t_node	*node;

	free_tokens(var->tokens);
	var->tokens = NULL;
	while (var->list)
	{
		node = var->list;
		var->list = node->next;
		if (node->type == HEREDOC)
			free(node->content[FILENAME]);
		free(node->content);
		free(node);
	}
	var->list = NULL;
}

void	free_tokens(t_token *root)
{
	t_token *temp;

	while (root != NULL)
	{
		if (root->right != NULL)
		{
			temp = root->right;
			root->right = NULL;
		} 
		else
			temp = NULL;
		if (root->str != NULL)
			free(root->str);
		free(root);
		root = temp;
	}
}
