UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
  STATIC := 
else
  STATIC := -static
endif

CC=gcc
CFLAGS=-I.
LIBS=-lm

cbmage: cbmage.o
	$(CC) $(STATIC) -o cbmage cbmage.o $(CFLAGS) $(LIBS)
