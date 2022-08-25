# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: euyi <euyi@student.42wolfsburg.de>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/23 14:58:05 by euyi              #+#    #+#              #
#    Updated: 2022/08/05 17:19:21 by euyi             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

SRC = app/main.c	app/ph_utils.c	app/feeding_table.c	\
		app/echos.c	assets/my_atoi.c

OBJ = $(subst .c,.o,$(SRC))

CC ?= cc

CFLAGS =	-Wall	-Werror	-Wextra

LINKED =	-lpthread

all: $(NAME)

$(NAME):	$(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LINKED)

clean:
	/bin/rm -f $(OBJ)

fclean: clean
	/bin/rm -f $(NAME)

re: fclean all

.PHONY:	all	clean	fclean	re
