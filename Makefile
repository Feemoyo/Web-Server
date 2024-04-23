NAME				=				webserv

SRC					=				${wildcard *.cpp}
OBJ_PATH			=				./obj/

OBJ					=				${addprefix ${OBJ_PATH}, ${notdir ${SRC:.cpp=.o}}}

CXX					=				c++

INCLUDES			=				-I.

CXXFLAGS			=				-Wall -Wextra -Werror -Wpedantic -Wshadow -std=c++98 -g3

VALGRIND			=				valgrind

VALFLAGS			=				--quiet \
									--show-error-list=yes \
									--show-leak-kinds=all \
									--track-origins=yes \
									--error-exitcode=42 \
									--exit-on-first-error=yes

DEL					=				rm -rf

.DEFAULT_GOAL		=				all

${OBJ_PATH}%.o:		%.cpp
					${CXX} ${CXXFLAGS} -c $< -o $@

all:				${NAME}

${NAME}:			${OBJ}
					${CXX} ${CXXFLAGS} ${INCLUDES} ${OBJ} -o ${NAME}

${OBJ}:				| ${OBJ_PATH}

${OBJ_PATH}:
					mkdir ${OBJ_PATH}

clean:
					${DEL} ${OBJ_PATH}

fclean:				clean
					${DEL} ${NAME}

re:					fclean all

valgrind:			${NAME}
					${VALGRIND} ${VALFLAGS} ./${NAME} test.txt s1 "Hello World"

.PHONY:				all clean fclean re valgrind