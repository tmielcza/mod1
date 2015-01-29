#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: caupetit <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/01/05 15:46:59 by caupetit          #+#    #+#              #
#    Updated: 2015/01/29 20:47:04 by caupetit         ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME = mod1
SRC =	Display.cpp \
		Map.cpp \
		Utils.cpp \
		main.cpp
CC = g++ -O3
FLAGS = -Wall -Wextra -Werror
SDL2 = -F ~/Library/Frameworks -framework SDL2 \
		-I ~/Library/Frameworks/SDL2.framework/Headers \
		-framework OpenGL
OBJ = $(SRC:%.cpp=%.o)

all: $(NAME)

$(NAME): $(OBJ)
	@echo "\033[32m=>\033[0m Compiling \033[33m$(NAME)\033[0m"
	@$(CC) $(FLAGS) -o $@ $(OBJ) $(SDL2)
	@echo "Compilation \033[32mSuccess\033[0m"

%.o: %.cpp
	@echo "\033[32m->\033[0m Compiling \033[33m$<\033[0m"
	@$(CC) $(FLAGS) -o $@ -c $< $(SDL2)

clean:
	@rm -f $(OBJ)
	@echo "\033[31m->\033[0m Deleted \033[33m.o\033[0m"

fclean: clear clean
	@rm -f $(NAME)
	@echo "\033[31m->\033[0m Deleted \033[33m$(NAME)\033[0m"

re: fclean all

clear:
	@rm -f `find *~`
	@echo "==> cleared ~ files"
