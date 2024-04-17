# Directories #
SRCDIR =
HDRDIR =
OBJDIR =
LIBFTDIR = libft/
# Names #
NAME = fdf
BONUS_NAME =
# Compiler & COptions & CFlags #
CFLAGS = -g -Werror -Wall -Wextra -Lmlx_linux -lmlx_Linux -L/usr/lib -Imlx_linux -lXext -lX11 -lm -lz
TESTFLAGS = -g3
COPTIONS = -Werror -Wall -Wextra -I/usr/include -Imlx_linux -O3 -c
CC = cc
# Source Files #
SRC = 
MAIN_SRC =
BONUS_SRC =
TEST_SRC =
HEADERS = mlx.h libft.h
LIBFT_SRC = libft.a
# Object Files #remove
SRC_OBJ = $(SRC:.c=.o)
BONUS_OBJ = $(BONUS_SRC:.c=.o)
MAIN_OBJ = $(MAIN_SRC:.c=.o)
TEST_OBJ = $(TEST_SRC:.c=.o)
# Tester directorys #
# No Tester yet
TESTDIR =

# Targets #
all: $(NAME) bonus

fully: $(NAME) bonus

$(NAME): $(LIBFT_SRC) $(SRC_OBJ)
	$(CC) $(CFLAGS) $(SRC_OBJ) $(LIBFT_SRC) -o $(NAME)

bonus: $(BONUS_NAME)

$(BONUS_NAME): $(LIBFT_SRC) $(BONUS_OBJ)
	$(CC) $(CFLAGS) $(BONUS_OBJ) $(LIBFT_SRC) -o $(BONUS_NAME)

$(LIBFT_SRC):
  ifeq ("$(wildcard $(LIBFTDIR))", "")
	@echo "Directory does not exist."
#	@git submodule add git@github.com:WSSMRKS/libft.git
  else
	@echo "Skipping download because directory already exists."
	$(MAKE) all -C libft/
	@cp -rf libft/libft.h ./
	@cp -rf libft/libft.a ./
  endif

exes: $(NAME) bonus clean

ex: $(NAME) clean

ex_bonus: bonus clean

# Compile .c to .o #
%.o: %.c
	@$(CC) $(COPTIONS) $^ -o $@

# Checkers, Testers #

# clean, fclean, re
clean:
	@rm -f $(SRC_OBJ)
	@rm -f $(MAIN_OBJ)
	@rm -f $(BONUS_OBJ)
	@rm -f $(TEST_OBJ)
  ifeq ("$(wildcard $(LIBFTDIR))", "")
	@echo "libft: Directory does not exist."
  else
	$(MAKE) fclean -C libft/
	@echo "libft folder cleaned"
  endif
	@rm -f libft.a
	@rm -f libft.h

fclean: clean
	@rm -f $(NAME) $(MAIN_NAME) $(BONUS_NAME)
	@echo "\"$(NAME)\" deleted"

re: fclean all

name:
	@echo "$(NAME)"

help:
	@echo "Possible Commands:"
	@echo "all --> Compile whole project including bonus"
	@echo "fully --> Compile whole project including bonus and testers"
	@echo "name --> Display project name"
	@echo "bonus --> Compile bonus"
	@echo "test --> run all available tests"
	@echo "clean --> Delete all object files"
	@echo "fclean --> Delete everything besides source files"
	@echo "re --> recompile everything (fclean, all)"
	@echo "libft --> Compile libft and copy libft.h and libft.a to project folder"

.PHONY: all fclean clean re
