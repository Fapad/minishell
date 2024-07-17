/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 16:20:26 by bszilas           #+#    #+#             */
/*   Updated: 2024/07/17 18:09:49 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"

# define END 0
# define IN_R 1
# define OUT_R 2
# define OUT_APPEND 3
# define HEREDOC 4
# define CMD 5
# define PIPE 6

typedef struct s_token
{
	int				type;
	struct s_token	*left;
	struct s_token	*right;
	char			*str;
	size_t			str_len;
} 					t_token;

typedef struct s_var
{
	t_token	*tree;
	char	*line;
} 			t_var;

#endif