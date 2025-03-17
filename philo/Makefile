CC = gcc
CFLAGS = -Wall -Wextra -Werror

INCLUDES = -Iinclude -I$(LIBFT_FOLDER)/include

SRCS =	main.c\
		monitor.c\
		init.c

NAME = philo

all: $(NAME)

$(NAME):
	$(CC) $(CFLAGS) $(INCLUDES) $(SRCS) -o $(NAME)

clean:
	rm -f $(NAME)

fclean: clean

re: clean all