/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 11:57:10 by bszilas           #+#    #+#             */
/*   Updated: 2024/10/29 14:25:50 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include <stdarg.h>

# define BUFFER_SIZE 100
# define BYTE 8

typedef struct s_list
{
	int				x;
	int				y;
	void			*content;
	struct s_list	*next;
}				t_list;

int			ft_isprint(int c);
int			ft_isascii(int c);
int			ft_isalnum(int c);
int			ft_isalpha(int c);
int			ft_isdigit(int c);
void		*ft_memset(void *s, int c, size_t n);
void		ft_bzero(void *s, size_t n);
void		*ft_memcpy(void *dest, const void *src, size_t n);
void		*ft_memmove(void *dest, const void *src, size_t n);
void		*ft_memchr(const void *s, int c, size_t n);
int			ft_memcmp(const void *s1, const void *s2, size_t n);
size_t		ft_strlen(const char *s);
size_t		ft_strlcpy(char *dst, const char *src, size_t size);
size_t		ft_strlcat(char *dst, const char *src, size_t size);
int			ft_tolower(int c);
int			ft_toupper(int c);
char		*ft_strchr(const char *s, int c);
char		*ft_strrchr(const char *s, int c);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
char		*ft_strnstr(const char *big, const char *little, size_t len);
int			ft_atoi(const char *nptr);
long		ft_atol(const char *nptr);
void		*ft_calloc(size_t nitems, size_t size);
/*Retuns a heap allocated duplicate of s, or NULL if the allocation fails.*/
char		*ft_strdup(const char *s);
/*Allocates (with malloc(3)) and returns a substring
from the string ’s’.
The substring begins at index ’start’ and is of
maximum size ’len’.*/
char		*ft_substr(char const *s, unsigned int start, size_t len);
/*Allocates (with malloc(3)) and returns an array
of strings obtained by splitting ’s’ using the
character ’c’ as a delimiter. The array must end
with a NULL pointer.*/
char		**ft_split(char const *s, char c);
/*Concatenates s2 to s1 and frees s1 in the process.
Returns a pointer to the concatenated string allocated on the heap.

Returns NULL if any heap allocation fails.
If s1 is NULL, returns a heap allocated duplicate of s2.
If s2 is NULL, frees s1 and returns NULL.*/
char		*ft_strjoin(char const *s1, char const *s2);
/*Concatenates s2 to s1. Does not free either s1 or s2.
Returns a pointer to the concatenated string allocated on the heap.

Returns NULL if s1 is NULL, or if any heap allocation fails.
If s2 is NULL, returns a heap allocated duplicate of s1.
*/
char		*ft_strjoin_nofree(char const *s1, char const *s2);
/*Allocates (with malloc(3)) and returns a copy of
’s1’ with the characters specified in ’set’ removed
from the beginning and the end of the string.*/
char		*ft_strtrim(char const *s1, char const *set);
char		*ft_itoa(long long n);
/*Applies the function f to each character of the
string s, passing its index as the first argument
and the character itself as the second. A new
string is created (using malloc(3)) to collect the
results from the successive applications of f.*/
char		*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void		ft_striteri(char *s, void (*f)(unsigned int, char*));
void		ft_putchar_fd(char c, int fd);
void		ft_putstr_fd(char *s, int fd);
void		ft_putendl_fd(char *s, int fd);
void		ft_putnbr_fd(int n, int fd);
/*Allocates (with malloc(3)) and returns a new node.
The member variable ’content’ is initialized with
the value of the parameter ’content’. The variable
’next’ is initialized to NULL.*/
t_list		*ft_lstnew(void *content);
/*Adds the node ’new’ at the beginning of the list.*/
void		ft_lstadd_front(t_list **lst, t_list *new);
/*Iterates the list ’lst’ and applies the function
’f’ on the content of each node.*/
void		ft_lstiter(t_list *lst, void (*f)(void *));
int			ft_lstsize(t_list *lst);
t_list		*ft_lstlast(t_list *lst);
/*Adds the node ’new’ at the end of the list.*/
void		ft_lstadd_back(t_list **lst, t_list *new);
/*Takes as a parameter a node and frees the memory of
the node’s content using the function ’del’ given
as a parameter and free the node. The memory of
’next’ must not be freed.*/
void		ft_lstdelone(t_list **lst, void (*del)(void*));
/*Iterates the list ’lst’ and applies the function
’f’ on the content of each node. Creates a new
list resulting of the successive applications of
the function ’f’. The ’del’ function is used to
delete the content of a node if needed*/
t_list		*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));
/*Deletes and frees the given node and every
successor of that node, using the function ’del’
and free(3).
Sets, the pointer to the list to NULL.*/
void		ft_lstclear(t_list **lst, void (*del)(void*));
int			ft_printf(const char *fmt, ...);
void		print_string(char *str, int *len);
void		print_char(char c, int *len);
void		print_pointer(void *ptr, int *len);
void		print_decimal(long long n, int *len);
void		print_hex(unsigned long long n, char x, int *len);
void		decide_format(const char **fmt, va_list args, int *len);
void		free_string_array(char **array);
void		free_tripl_ptrs(char ***ptrs, int len);
int			ft_abs(int n);
/*Returns the nth pseudo-random number from a loop starting with seed.*/
int			ft_rng(size_t seed, int nth);
/*Returns the nth pseudo-random 
ASCII alphanumeric char from a loop starting with seed.*/
char		ft_rand_alnum(size_t seed, int nth);
/*Returns a heap allocated pointer to a line from file decriptor fd
including the newline ("\n") character. Repeated calls to get_next_line()
return the next line from the same fd one at a time.

Returns NULL if EOF was reached or if a read() or malloc() error occured.

The behaviour is undefined when reading a binary
or if the file or the file descriptor changed
since the last call whereas read() didn’t reach the end of file.*/
char		*get_next_line(int fd);
int			ft_iswhitespace(int c);
size_t		digits_count(long long n, int base);
int			count_words(char const *s, char c);
long long	ft_pow(long long n, int exp);

#endif