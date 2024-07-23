/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 16:20:26 by bszilas           #+#    #+#             */
/*   Updated: 2024/07/23 09:13:07 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include "../libft/libft.h"

# define END 0
# define IN_R 1
# define OUT_R 2
# define OUT_APPEND 3
# define HEREDOC 4
# define CMD 5
# define PIPE 6
# define PROMPT "\033[1;32mminishell>\033[0m "

typedef struct s_token
{
	int				type;
	struct s_token	*left;
	struct s_token	*right;
	char			*str;
	size_t			str_len;
}				t_token;

typedef struct s_node
{
	int				type;
	struct s_node	*next;
	struct s_node	*prev;
	char			**content;
}					t_node;

typedef struct s_var
{
	t_token	*tokens;
	t_node	*list;
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
void	print_tokens(t_token *head);

// SIGNAL

void	setup_signal_handlers();

// PARSER

void	check_redir_file(t_var *var, int type, int next_type);
void	check_syntax(t_var *var, t_token *current);
t_node	*last_node(t_token *current, t_node *this);
int		token_arg_count(t_token *current);
t_node	*new_command_node(t_token **current, t_node *this);
t_node	*new_pipe_node(t_token *current, t_node *this);
void	set_redirect_type(t_node *this);
t_node	*new_redirect_node(t_token **current, t_node *this);
t_node	*new_list_node(t_token **current);
void	add_to_list(t_var *var, t_node *this);
void	print_exec_list(t_node *list);
int		parse_tokens(t_var *var);

#endif