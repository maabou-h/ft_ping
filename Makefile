# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maabou-h <maabou-h@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/02/13 18:42:02 by maabou-h          #+#    #+#              #
#    Updated: 2019/08/16 15:04:04 by maabou-h         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

### COLORS ###
NOC         = \033[0m
BOLD        = \033[1m
UNDERLINE   = \033[4m
RED         = \033[0;31m
GREEN       = \033[0;32m
YELLOW      = \033[0;33m
BLUE        = \033[0;34m
VIOLET      = \033[0;35m
WHITE       = \033[0;37m

SRC_PATH = srcs

SRC_NAME = checker.c\
		   checksum.c\
		   ft_ping.c\
		   helper.c\
		   init.c\
		   options.c\
		   packer.c\
		   statistics.c\
		   timestamp.c\
		   unpacker.c

NAME = ft_ping

CC = clang

CFLAGS = -Iincs -Werror -Wall -Wextra

OBJ_PATH = .obj

OBJ_NAME = $(SRC_NAME:%.c=%.o)
	OBJ = $(addprefix $(OBJ_PATH)/,$(OBJ_NAME))
	SRC = $(addprefix $(SRC_PATH)/,$(SRC_NAME))

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(LDFLAGS) $^ -o $@
	@printf "$(GREEN)Project successfully compiled$(NOC)"

$(OBJ): incs/ft_ping.h

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	@mkdir -p $(OBJ_PATH)
	@$(CC) $(CFLAGS) -o $@ -c $<
	@printf "$(BLUE)Creating object file -> $(WHITE)$(notdir $@)... $(RED)[Done]\n"

clean:
	@printf "$(RED)Supressing libraries files$(RED)\n"
	@rm -rfv $(OBJ_PATH)

fclean: clean
	@rm -fv $(NAME)

re: fclean all

.PHONY: all, clean, fclean, re, ft_ping
