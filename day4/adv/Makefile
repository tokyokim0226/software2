CC = gcc

BINDIR = bin
INCLUDE = include
LIBDIR = lib
SRCDIR = src

CFLAGS = -Wall -I$(INCLUDE) 
LDFLAGS = -L$(LIBDIR)
LDLIBS = -lfuji

SRC = $(SRCDIR)/adv_regression.c
OBJS = $(SRCDIR)/func.o $(SRCDIR)/optimize.o 
LIB = $(LIBDIR)/libfuji.a

TARGET = $(BINDIR)/adv_regression

$(TARGET): $(SRC) $(LIB)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS) $(LDLIBS)

$(LIB): $(OBJS)
	$(AR) rsv $@ $^

.PHONY: tmpclean clean

tmpclean:
	rm -f $(SRCDIR)/*~
clean: tmpclean
	rm -f $(TARGET) $(LIB) $(OBJS)