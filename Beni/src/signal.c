/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 10:36:22 by ajovanov          #+#    #+#             */
/*   Updated: 2024/08/10 13:20:57 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

sig_atomic_t	g_signal = 0;

void	handle_sigint(int sig)
{
	g_signal = sig;
	ioctl(STDIN_FILENO, TIOCSTI, "\n");
	rl_on_new_line();
	rl_replace_line("", 0);
}

void	sigint_wait(int signal)
{
	g_signal = signal;
	rl_on_new_line();
}

void	sigquit_and_sigint_handler(t_var *var)
{
	sigaction(SIGINT, &var->sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_signal_handlers(t_var *var)
{
	var->sa.sa_handler = handle_sigint;
	var->sa.sa_flags = SA_RESTART;
	sigemptyset(&var->sa.sa_mask);
	sigquit_and_sigint_handler(var);
}
