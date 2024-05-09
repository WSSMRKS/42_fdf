# Directories #
SRCDIR = src/
HDRDIR = header/
LIBFTDIR = libft
MLXDIR = mlx_linux
# Names #
NAME = fdf
# no bonus
BONUS_NAME =
# Compiler & COptions & CFlags #
CFLAGS = -g -Werror -Wall -Wextra -Lmlx_linux -lmlx_Linux -L/usr/lib -Imlx_linux -lXext -lX11 -lm -lz
TESTFLAGS = -g3
COPTIONS = -g -Werror -Wall -Wextra -I/usr/include -Imlx_linux/ -O0 -c
CC = cc
# Source Files #
SRC = src/fdf.c src/fdf_clean_closing.c src/fdf_drawing.c src/fdf_init_structs.c src/fdf_mlx.c src/fdf_parsing.c src/fdf_parsing2.c
HEADERS = fdf.h fdf_defines.h fdf_includes.h
LIBFT_SRC = libft/libft.a
# Object Files
SRC_OBJ = $(SRC:.c=.o)
BONUS_OBJ = $(BONUS_SRC:.c=.o)
MAIN_OBJ = $(MAIN_SRC:.c=.o)
TEST_OBJ = $(TEST_SRC:.c=.o)

# Targets #
all: $(NAME)

$(NAME): mlx $(LIBFT_SRC) $(SRC_OBJ)
	$(CC) $(SRC_OBJ) $(LIBFT_SRC) $(CFLAGS) -o $(NAME)

$(LIBFT_SRC):
	$(MAKE) all -C libft/

mlx:
  ifeq ("$(wildcard $(MLXDIR))", "")
	echo "Directory does not exist."
	echo "Downloading mlx_linux."
	git clone git@github.com:WSSMRKS/mlx_linux.git
	$(MAKE) all -C mlx_linux/
  else
	echo "Skipping download because mlx_linux already exists."
	$(MAKE) all -C mlx_linux/
  endif

exes: $(NAME) clean

# Compile .c to .o #
%.o: %.c
	$(CC) $^ $(COPTIONS) -o $@

# Checkers, Testers #
test:
	bash fdf_tester/tester.sh
dl_tester:
	git clone git@github.com:WSSMRKS/42_fdf_tester.git fdf_tester

# clean, fclean, re
clean:
	rm -f $(SRC_OBJ)
	rm -f $(MAIN_OBJ)
	rm -f $(BONUS_OBJ)
	rm -f $(TEST_OBJ)
  ifeq ("$(wildcard $(LIBFTDIR))", "")
	echo "libft: Directory does not exist."
  else
	$(MAKE) fclean -C libft/
	echo "libft folder cleaned"
  endif
  ifeq ("$(wildcard $(MLXDIR))", "")
	echo "mlx_linux: Directory does not exist."
  else
	$(MAKE) clean -C mlx_linux
	echo "mlx_linux folder cleaned"
  endif

fclean: clean
	rm -f $(NAME) $(MAIN_NAME) $(BONUS_NAME)
	rm -rf mlx_linux
	rm -rf fdf_tester
	echo "\"$(NAME)\" deleted"

re: fclean all

name:
	echo "$(NAME)"

help:
	echo "Possible Commands:"
	echo "all --> Compile whole project including bonus"
	echo "name --> Display project name"
	echo "exes --> Compile project and delete object files"
	echo "help --> Display this help message"
	echo "mlx --> Download mlx_linux library"
	echo "dl_tester --> Download the tester"
	echo "test --> Run all available tests"
	echo "test --> run all available tests"
	echo "clean --> Delete all object files"
	echo "fclean --> Delete everything besides source files"
	echo "re --> recompile everything (fclean, all)"
	echo "libft --> Compile libft and copy libft.h and libft.a to project folder"

.PHONY: all fclean clean re
