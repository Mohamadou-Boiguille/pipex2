NAME= pipex
SRCS=	start.c\
		ft_utils.c\
		ft_checkers.c\
		ft_creators.c
OBJS= $(SRCS:.c=.o)
INCLUDE= -I./includes -I./libft
HEADERS= pipex.h
LIB=-L./libft/ -lft
LIBFT= libft/libft.a
CFLAGS = -g -Wall -Wextra -Werror


$(NAME) : $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LIB)

%.o: %.c $(HEADERS) Makefile
	$(CC) $(CFLAGS) -g3 -c $< -o $@

$(LIBFT): ./libft/*.c
	$(MAKE) bonus -C ./libft

clean :
	rm -rf *.o $(NAME)

all : $(NAME)

fclean : clean

re : clean $(NAME)

.PHONY: re all fclean clean
