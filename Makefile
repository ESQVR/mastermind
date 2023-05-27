CC = gcc
CFLAGS = -Wall -Wextra -Werror

SRCS = my_mastermind.c mmfunctions.c mmglobals.c
OBJS = $(SRCS:.c=.o)
HEADERS = mmheader.h mmglobals.h

NAME = my_mastermind

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
