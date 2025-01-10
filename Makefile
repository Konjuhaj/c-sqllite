
DB = db

SRC = ./srcs/*

HEADERS = ./headers/*

all: $(DB)

$(DB) : $(HEADERS)
	gcc -Wall -Werror -Wextra $(SRC) -o $(DB)