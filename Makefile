#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: caupetit <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/01/05 15:46:59 by caupetit          #+#    #+#              #
#    Updated: 2016/08/09 20:58:53 by tmielcza         ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME = mod1
SRC =	Display.cpp \
		Map.cpp \
		Utils.cpp \
		main.cpp
PWD=$(shell pwd)
CC = g++ -O3 -g
FLAGS = -Wall -Wextra -Werror
SDL2=$(shell sdl2-config --cflags --libs)
OBJ = $(SRC:%.cpp=%.o)

all: $(NAME)

$(NAME): $(OBJ)
	@echo "\033[32m=>\033[0m Compiling \033[33m$(NAME)\033[0m"
	$(CC) $(FLAGS) -o $@ $(OBJ) $(SDL2)
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

sdl2:
	curl -o "SDL2.tar.gz" "https://www.libsdl.org/tmp/SDL-2.0.4-10216.tar.gz"
	tar zxvf "SDL2.tar.gz"
	cd "SDL-2.0.4-10216" && mkdir -p build bin && cd build && ../configure --prefix=$(PWD)/SDL-2.0.4-10216/bin &&make install

re: fclean all

clear:
	@rm -f `find *~`
	@echo "==> cleared ~ files"
