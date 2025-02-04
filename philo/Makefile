CC = gcc
CFLAGS = -Wall -Wextra -Werror

LIBFT_FOLDER = ../ft_lib
LIBFT = $(LIBFT_FOLDER)/libft.a

INCLUDES = -Iinclude -I$(LIBFT_FOLDER)/include
LDFLAGS = -L$(LIBFT_FOLDER)/lib -lft

SRCS =	main.c\
		monitor.c\
		init.c

NAME = philo

all: $(LIBFT) $(NAME)

$(LIBFT):
	make -C $(LIBFT_FOLDER)

$(NAME):
	$(CC) $(CFLAGS) $(INCLUDES) $(SRCS) $(LDFLAGS) -o $(NAME)

clean:
	make -C $(LIBFT_FOLDER) clean
	rm -f $(NAME)

fclean: clean
	make -C $(LIBFT_FOLDER) fclean

re: clean all