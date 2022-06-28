TARGET = lib/libexpr.a
SRC = $(wildcard src/*.c) $(wildcard src/*/*.c)
OBJ = ${SRC:.c=.o}
CFLAGS = -I./include -Wall -Wextra -Werror -ansi -pedantic
CC ?= /bin/cc

all: ${TARGET}

${TARGET}: ${OBJ}
	mkdir -p $(shell dirname ${TARGET})
	ar rcs ${TARGET} ${OBJ}

%.o: %.c
	${CC} ${CFLAGS} -c ${<} -o ${@}

fclean: clean
	rm -f ${TARGET}

clean:
	rm -f ${OBJ}

re: fclean all