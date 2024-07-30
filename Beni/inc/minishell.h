/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 16:20:26 by bszilas           #+#    #+#             */
/*   Updated: 2024/07/30 10:16:17 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <stdbool.h>
# include <fcntl.h>
# include <sys/wait.h>
# include "../libft/libft.h"

# define END 0x1
# define IN_R 0x2
# define HEREDOC 0x4
# define CMD 0x8
# define PIPE 0x10
# define INTERPRET 0x20
# define OUT_R O_TRUNC
# define OUT_APPEND O_APPEND
# define READ_END 0
# define WRITE_END 1
# define PROMPT "\001\033[1;31m\002min\001\033[1;37m\002ish\001\033\
[1;32m\002ell\001\033[0m\002 > "
# define HD_PROMPT "\001\033[1;31m\002her\001\033[1;37m\002edo\001\033\
[1;32m\002c > \001\033[0m\002"
# define TMP_PATH "/tmp/.tmp"
# define FILENAME 1
# ifndef PATH_MAX
#  define PATH_MAX FILENAME_MAX
# endif

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
	t_node	*current;
	char	*line;
	char	**env;
	char	**stack_env;
	char	**splitted_path;
	pid_t	pid;
	pid_t	pid2;
	size_t	len;
	int		pfd[2];
	int		old_pfd[2];
	int		in_fd;
	int		out_fd;
	int		cmds;
	int		status;
	int		i;
}			t_var;

// LEXER

t_token	*create_token(int type, char *str);
t_token	*tokenize(t_var *var);
int		add_token(t_var *var, t_token **current, char **start);
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
void	cat_env_var(t_var *var, char *str, char **start, char *end);
void	cat_double_qoutes(t_var *var, char *str, char **start, char *end);
char	*cat_intrd_str(t_var *var, char *start, char *end);
size_t	single_quote_len(char *s, char *end, size_t *i);
size_t	env_var_len(char *s, char *end, size_t *i, char **env);
size_t	double_qoute_len(char *s, char *end, size_t *i, char **env);
size_t	interpreted_str_len(char **env, char *start, char *end);
char	*ft_getenv(char **env, char *s);

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

void	init_var(t_var *var, int argc, char **argv, char **envp);
bool	valid_syntax(t_token *token);
void	free_linked_lists(t_var *var);
void	unexpected_token(char *str);
void	free_all(t_var *var);
void	restore_environment(t_var *var);

// BUILTINS

void	malloc_envps(t_var *var, char **envp);
char	**command_unset(char **old_envp, char *str);
char	**command_export(t_var *var, char *str);
int		command_export_util(t_var *var, char *str);
void	command_echo(t_node *list);
size_t	envp_string_count(char **envp);
void 	command_exit(t_var *var);
void	command_cd(t_var *var);
void	command_pwd(void);
void	command_env(t_var *var);
int		unset2(char **old_envp, char *dest, size_t to_compare, char **new_env);

// EXECUTE

void	execute(t_var *var);
int		cd_export_exit_or_unset(t_var *var);
void	one_simple_cmd(t_var *var);
void	exec_other_commands(t_var *var);
void	write_here_docs(t_var *var);
int		count_node_types(t_node *node, int type);
t_node	*get_next_node(t_node *node, int get_type, int before_type);
void	in_open_or_exit(t_var *var);
void	out_open_or_exit(t_var *var);
int		out_open_return_status(t_var *var);
int		in_open_return_status(t_var *var);
void	file_redirect(t_var *var);
void	close_in_and_out(t_var *var);
void	exec_system_commands(t_var *var);
char	**splitted_path(t_var *var);
char	*ft_strjoin_three(char *s1, char *s2);
void	close_pipe(int pfd[]);

#endif