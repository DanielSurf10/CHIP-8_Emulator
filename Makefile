NAME	= chip8
CC		= gcc
# CFLAGS	= -Wall -Wextra -Werror -g3
CFLAGS	= -g3
INCLDE	= include

LIB_SDL	= $(shell sdl2-config --cflags --libs)
LIBS	= $(LIB_SDL)

SRC 	= $(shell find src -name "*.c")
OBJECTS	= ${SRC:src/%.c=obj/%.o}

$(NAME): all

obj:
	@mkdir -p obj

all: $(OBJECTS)
	@printf "Compiling $(NAME)\n"
	@$(CC) $(CFLAGS) -I $(INCLDE) $(OBJECTS) $(LIBS) -o $(NAME)
	@printf "Done!\n"

obj/%.o: src/%.c obj
	@printf "Compiling $(notdir $<)\n"
	@$(CC) $(CFLAGS) -I $(INCLDE) -c $< -o $@

clean:
	@printf "Deleting object files!\n"
	@rm -rf obj

fclean: clean
	@printf "Deleting $(NAME)\n"
	@rm -rf $(NAME)

re: fclean all

.PHONY: clean fclean all
