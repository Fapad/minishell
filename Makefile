# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bszilas <bszilas@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/16 16:17:41 by bszilas           #+#    #+#              #
#    Updated: 2024/07/17 10:38:36 by bszilas          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CFILES = 

CC = cc
LIBDIR = libft
SRCDIR = src/
INCDIR = inc/
OBJDIR = obj/
LIB = libft.a
DB = debug
RM = rm -rf
SRC = $(addprefix $(SRCDIR),$(CFILES))
OBJ = $(addprefix $(OBJDIR),$(CFILES:%.c=%.o))
CFLAGS = -Wall -Wextra -Werror -I$(INCDIR)
LFLAGS = -L$(LIBDIR) -lft -lreadline

all: $(LIB) $(OBJDIR) $(NAME)
	
$(LIB):
	make -C $(LIBDIR)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

$(OBJDIR)%.o: $(SRCDIR)%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	make clean -C $(LIBDIR)
	$(RM) $(OBJ) $(OBJDIR)

fclean: clean
	$(RM) $(LIBDIR)/$(LIB) $(NAME) $(DB) *.out

$(DB): $(LIB)
	$(CC) -g -o $(DB) $(CFLAGS) $(BSRC) $(LFLAGS)
	gdb ./$(DB)

diagram:
	cflow --main=main --depth=6 --omit-arguments -f dot $(SRC) | dot -Txlib

parser:
	$(CC) -g -o parser readline.c $(CFLAGS) $(LFLAGS)

push: fclean
	git add .
	git commit -m "quickpush $(shell date)"
	git push

re: fclean all

.PHONY: all clean fclean re push debug diagram

.SILENT: clean fclean push