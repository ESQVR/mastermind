CC = gcc
CFLAGS = -Wall -Wextra -Werror

SRCS = my_mastermind.c
OBJS = $(SRCS:.c=.o)

NAME = my_mastermind

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: clean all