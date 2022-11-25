NAME= pipex
SRCS= ft_split.c\
	start.c\
	ft_mini_libft.c\
	ft_utils1.c\
	ft_utils2.c\
	ft_creators.c
OBJS= $(SRCS:.c=.o)
HEADERS= pipex.h


$(NAME) : $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $@

%.o: %.c $(HEADERS)
	$(CC) $(FLAGS) -g3 -I . -c $< -o $@

clean :
	rm -rf *.o
	rm -rf pipex

all : $(NAME)

fclean : clean

re : clean $(NAME)

.PHONY: re all
