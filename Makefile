CFLAGS = -Wall -Wextra -Werror -shared -fPIC
CC = gcc

TARGET_PATH = /lib/stdinify.so

all: stdinify.c
	${CC} ${CFLAGS} stdinify.c -o stdinify.so

install: stdinify.so
	cp stdinify.so ${TARGET_PATH}
	echo "${TARGET_PATH}" >> /etc/ld.so.preload

uninstall:
	@-grep -v "^${TARGET_PATH}$$" /etc/ld.so.preload > /tmp/cleaned
	mv /tmp/cleaned /etc/ld.so.preload
	rm ${TARGET_PATH}

clean:
	rm stdinify.so
