/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 16:20:26 by bszilas           #+#    #+#             */
/*   Updated: 2024/08/04 12:04:09 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <stdbool.h>
# include <fcntl.h>
# include <errno.h>
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
[1;32m\002c \"\001\033[0m\002"
# define TMP_PATH "/tmp/.tmp"
# define FILENAME 1
# define INT_MAX_LEN 10

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
	char	*cwd;
	char	*exec_cmd;
	pid_t	pid;
	size_t	len;
	int		pfd[2];
	int		in_fd;
	int		out_fd;
	int		cmds;
	int		status;
	int		loop;
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
size_t	env_var_len(t_var *var, char *s, char *end, size_t *i);
size_t	double_qoute_len(t_var *var, char *s, char *end, size_t *i);
size_t	interpreted_str_len(t_var *var, char *start, char *end);
char	*ft_getenv(char **env, char *s);
size_t	number_len(long stat);
void	cat_status(char *str, int status, size_t len);

// SIGNAL

extern sig_atomic_t signal_received;
void	setup_signal_handlers();
void	handle_sigint(int sig);

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
void	unexpected_token(char *str);
bool	double_pipe(t_token *token);
bool	missing_filename(t_token *token);
bool	pipe_in_front(t_token *token);
bool	valid_syntax(t_token *token);

// ERROR_HANDLING

void	init_var(t_var *var, int argc, char **argv, char **envp);
void	free_linked_lists(t_var *var);
void	free_all(t_var *var);
void	restore_environment(t_var *var);
void	free_lists_and_path(t_var *var);
void	status_1(t_var *var);
void	status_2(t_var *var);
void	error_exec_txt_file(t_var *var);
void	command_not_found(t_var *var);

// BUILTINS

void	malloc_envps(t_var *var, char **envp);
char	**command_unset(char **old_envp, char *str);
char	**command_export(t_var *var, char *str);
int		valid_identifier(t_var *var, char *str);
void	command_echo(t_node *list);
size_t	envp_string_count(char **envp);
void 	command_exit(t_var *var);
void	command_cd(t_var *var, char *path);
void	command_pwd(t_var *var);
void	command_env(t_var *var);
int		unset2(char **old_envp, char *dest, size_t to_compare, char **new_env);
int		existing_env_var(char **env, char *str);
char	**change_var(char **env, char *str);
void	update_env_after_cd(t_var *var, char *env_var_name, char *path);
void	cd_home(t_var *var, char *path);
int		valid_dotdot_path(char *path);
void	cd_dotdot_for_istvan(t_var *var, char *path);
char	*find_next_smallest(char **arr, char *current, char *max);
void	print_environment(t_var *var);
char	**set_shlvl(t_var *var, char *str);
int		get_shlvl(char *str);

// EXECUTE

void	execute(t_var *var);
int		cd_export_exit_or_unset(t_var *var);
void	one_simple_cmd(t_var *var);
void	exec_other_commands(t_var *var);
int		count_node_types(t_node *node, int type);
t_node	*get_next_node(t_node *node, int get_type, int before_type);
void	close_in_and_out(t_var *var);
void	exec_system_commands(t_var *var);
char	**splitted_path(t_var *var);
char	*ft_strjoin_three(char *s1, char *s2);
void	close_pipe(int pfd[]);
void	exec_cleanup(t_var *var);
char	*get_cmd(t_var *var);
void	first_cmd(t_var *var);
void	middle_cmd(t_var *var);
void	last_cmd(t_var *var);
void	wait_children(t_var *var);
char	*check_given_file(t_var *var);
void	set_status(t_var *var);
int		search_path(t_var *var, int access_type);

// REDIRECT

int		create_tmp_file(t_node *node);
int		open_files_in_parent(t_var *var);
void	redirect_or_exit(t_var *var);
void	redirect_infile(t_var *var, char *file);
void	redirect_outfile(t_var *var, char *file, int type);
int		write_here_docs(t_var *var);
void	write_doc(char *limiter, int fd);

#endif