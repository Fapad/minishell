/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 12:13:32 by bszilas           #+#    #+#             */
/*   Updated: 2024/07/26 17:20:38 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	count_node_types(t_node *node, int type)
{
	int	n;

	n = 0;
	while (node)
	{
		if (node->type & type)
			n++;
		node = node->next;
	}
	return (n);
}

t_node	*get_next_node(t_node *node, int get_type)
{
	while (node && node->type != PIPE)
	{
		if (node->type & get_type)
			return (node);
		node = node->next;
	}
	return (NULL);
}