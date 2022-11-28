# Copyright 2022 ©️ vcwild under the GNU Affero General Public License v3.0.
# Compile the RBTree files

CC = c++

CFLAGS = -Wall -Wextra -g

FILES = RBTree.cpp RBTree.hpp main.cpp

all: rbtree

rbtree: $(FILES)
	$(CC) $(CFLAGS) $(FILES) -o $@

clean:
	rm -f rbtree

v:
	valgrind --leak-check=full --show-leak-kinds=all ./rbtree
