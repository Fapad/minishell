/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 16:20:26 by bszilas           #+#    #+#             */
/*   Updated: 2024/07/23 17:36:36 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <stdbool.h>
# include "../libft/libft.h"

# define END 0
# define IN_R 1
# define OUT_R 2
# define OUT_APPEND 3
# define HEREDOC 4
# define CMD 5
# define PIPE 6
# define INTERPRET 7
# define PROMPT "\033[1;32mminishell>\033[0m "

typedef struct s_token
{
	int				type;
	struct s_token	*left;
	struct s_token	*right;
	char			*str;
	size_t			str_len;
}				t_token;

typedef struct s_var
{
	t_token	*tree;
	char	*line;
}			t_var;

// LEXER

t_token	*create_token(int type, const char *str);
t_token	*tokenize(const char *input);
void	free_token(t_token *token);
void	skip_whitespace(const char **input);
int		identify_token_type(const char **start, const char **end);
int		identify_input_redirection(const char **start, const char **end);
int		identify_output_redirection(const char **start, const char **end);
int		identify_pipe(const char **start, const char **end);
int		identify_general_token(const char **start, const char **end);
int		identify_single_quotes(const char **start, const char **end);
int		identify_double_quotes(const char **start, const char **end);
void	print_tokens(t_token *head);
int		identify_dollar_sign(const char **start, const char **end);


// SIGNAL

void	setup_signal_handlers();


#endif