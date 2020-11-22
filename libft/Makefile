# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: macos <macos@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/03/30 03:33:28 by oelazzou          #+#    #+#              #
#    Updated: 2020/11/22 12:13:33 by macos            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
FILE = ft_atoi.c \
	   ft_bzero.c \
	   ft_is_there.c \
	   ft_isalnum.c \
	   ft_isalpha.c \
	   ft_isascii.c \
	   ft_isdigit.c \
	   ft_isneg.c \
	   ft_ispos.c \
	   ft_isprint.c \
	   ft_iswhite.c \
	   ft_itoa.c \
	   ft_lstadd.c \
	   ft_lstdel.c \
	   ft_lstdelone.c \
	   ft_lstiter.c \
	   ft_lstmap.c \
	   ft_lstnew.c \
	   ft_max.c \
	   ft_memalloc.c \
	   ft_memccpy.c \
	   ft_memchr.c \
	   ft_memcmp.c \
	   ft_memcpy.c \
	   ft_memdel.c \
	   ft_memmove.c \
	   ft_memset.c \
	   ft_numlen.c \
	   ft_putchar.c \
	   ft_putchar_fd.c \
	   ft_putendl.c \
	   ft_putendl_fd.c \
	   ft_putnbr.c \
	   ft_putnbr_fd.c \
	   ft_putstr.c \
	   ft_putstr_fd.c \
	   ft_strcat.c \
	   ft_strchr.c \
	   ft_strclr.c \
	   ft_strcmp.c \
	   ft_strcpy.c \
	   ft_strdel.c \
	   ft_strdup.c \
	   ft_strequ.c \
	   ft_striter.c \
	   ft_striteri.c \
	   ft_strjoin.c \
	   ft_strlcat.c \
	   ft_strlen.c \
	   ft_strmap.c \
	   ft_strmapi.c \
	   ft_strncat.c \
	   ft_strncmp.c \
	   ft_strncpy.c \
	   ft_strndup.c \
	   ft_strnequ.c \
	   ft_strnew.c \
	   ft_strnstr.c \
	   ft_strrchr.c \
	   ft_strsplit.c \
	   ft_strstr.c \
	   ft_strsub.c \
	   ft_strtrim.c \
	   ft_tolower.c \
	   ft_toupper.c \
	   get_arr_size.c \
	   ft_putstr_wspace.c \
	   ft_putstr_w2space.c \
	   ft_putnbr_wspace.c \
	   ft_strrchr_path.c \
	   is_blank.c \
	   free_two_dim.c \
	   ft_print_until.c \
	   ft_strjoin_four.c \
	   get_next_line.c
FLAGS = -Wall -Wextra -Werror
OBJ = $(FILE:.c=.o)

$(NAME): 
	gcc $(FLAGS) -c $(FILE) 
	ar rc $(NAME) $(OBJ)
	ranlib $(NAME)
all : $(NAME)
clean :
	rm -f $(OBJ) *.o
fclean : clean
	rm -f $(NAME) *.a
re : fclean all