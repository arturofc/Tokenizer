cc = gcc
CFLAGS = -Wall -g

all: tokenizer
count: tokenizer.c
	$(CC) $(CFLAGS) -o tokenizer tokenizer.c 


clean:
	rm -f tokenizer tokenizer.c
