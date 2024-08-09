/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 09:21:33 by bszilas           #+#    #+#             */
/*   Updated: 2024/08/08 15:10:31 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	status_1(t_var *var)
{
	if (var->status != 2)
		var->status = EXIT_FAILURE;
}

void	status_2(t_var *var)
{
	var->status = EXIT_FAILURE << 1;
}

void	invalid_identifier(t_var *var, char *str)
{
	ft_putstr_fd("export: `", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putstr_fd("\': not a valid identifier\n", STDERR_FILENO);
	var->status = EXIT_FAILURE;
}

void	restore_environment(t_var *var)
{
	perror("Could not change environment");
	malloc_envps(var, var->stack_env);
	ft_putendl_fd("Environment has been reset", STDERR_FILENO);
}