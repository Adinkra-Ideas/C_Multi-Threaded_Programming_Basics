# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: euyi <euyi@student.42wolfsburg.de>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/23 14:58:05 by euyi              #+#    #+#              #
#    Updated: 2022/08/08 17:31:57 by euyi             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

BONUS = philo_bonus

BSRC = app/main.c	app/ph_utils.c	app/feeding_table.c	\
		app/echos.c	assets/my_atoi.c

BOBJ = $(subst .c,.o,$(BSRC))

CC ?= cc

CFLAGS =	-Wall	-Werror	-Wextra

LINKED =	-lpthread

all: $(NAME)

bonus: $(BONUS)

$(BONUS):	$(BOBJ)
	$(CC) $(CFLAGS) $(BOBJ) -o $(BONUS) $(LINKED)

clean:
	/bin/rm -f $(BOBJ)

fclean: clean
	/bin/rm -f $(BONUS)

re: fclean all

.PHONY:	all	clean	fclean	re bonus
