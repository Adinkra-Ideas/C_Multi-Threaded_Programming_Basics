NAME = philosophers

SRCS =	main.c	\
		ph_utils.c	\
		feeding_table.c	\
		activity.c

OBJDIR = build/
SRCDIR = srcs/
HEADER = includes/

OBJS := $(SRCS:.c=.o)
SRCS_PREFIXED := $(addprefix $(SRCDIR), $(SRCS))
OBJECTS_PREFIXED := $(addprefix $(OBJDIR), $(OBJS))

CC = cc

CFLAGS = -Wall	-Werror	-Wextra -pedantic -g3 -fsanitize=address -D BUFFER_SIZE=1200

LINKER =	-lpthread

all: $(NAME)

$(NAME): $(OBJECTS_PREFIXED) $(OBJDIR)
	$(CC) $(CFLAGS) $(OBJECTS_PREFIXED) -o $(NAME) $(LINKER)

$(OBJECTS_PREFIXED): $(OBJDIR)%.o : $(SRCDIR)%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c -I$(HEADER) $< -o $@ 

clean:
	/bin/rm -rf $(OBJDIR)

fclean: clean
	/bin/rm -f $(NAME)

re: fclean all

.PHONY:	all	clean	fclean	re