/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 16:20:26 by bszilas           #+#    #+#             */
/*   Updated: 2024/07/25 21:00:08 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

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
# define PROMPT "\001\033[1;31m\002min\001\033[1;37m\002ish\001\033\
[1;32m\002ell\001\033[0m\002 > "

typedef struct s_token
{
	int				type;
	struct s_token	*right;
	char			*str;
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
	char	**env;
	pid_t	pid;
	int		pfd[2];
	int		cmds;
}			t_var;

// LEXER

t_token	*create_token(int type, char *str);
t_token	*tokenize(char *input);
void 	free_tokens(t_token *root);
void	skip_whitespace(char **input);
int		identify_token_type(char **start, char **end);
int		identify_input_redirection(char **start, char **end);
int		identify_output_redirection(char **start, char **end);
int		identify_pipe(char **start, char **end);
int		identify_general_token(char **start, char **end);
int		identify_single_quotes(char **start, char **end);
int		identify_double_quotes(char **start, char **end);
void	print_tokens(t_token *head);
int		identify_dollar_sign(char **start, char **end);
void	ft_strncpy(char	*dest,const char *str, size_t n);
char 	*ft_strndup(const char *s, size_t n);

// INTERPRET

void	cat_char_to_str(char *str, char c, size_t len);
void	cat_single_qoutes(char *str, char **start, char *end, size_t len);
void	cat_env_var(char *str, char **start, char *end, size_t len);
void	cat_double_qoutes(char *str, char **start, char *end, size_t len);
char	*cat_intrd_str(char *str, char *start, char *end, size_t len);
size_t	single_quote_len(char *s, char *end, size_t *i);
size_t	env_var_len(char *s, char *end, size_t *i);
size_t	double_qoute_len(char *s, char *end, size_t *i);
size_t	interpreted_str_len(char *start, char *end);

// SIGNAL

void	setup_signal_handlers();

// PARSER

t_node	*last_node(t_token *current, t_node *this);
int		token_arg_count(t_token *current);
t_node	*new_command_node(t_token **current, t_node *this);
t_node	*new_pipe_node(t_token *current, t_node *this);
void	set_redirect_type(t_node *this);
t_node	*new_redirect_node(t_token **current, t_node *this);
t_node	*new_list_node(t_token **current);
void	add_to_list(t_var *var, t_node *this);
void	print_exec_list(t_node *list);
bool	parse_tokens(t_var *var);

// ERROR_HANDLING

bool	valid_syntax(t_token *token);
void	free_linked_lists(t_var *var);
void	unexpected_token(char *str);

#endif