CC = gcc

CFLAGS = -Wall

# wildcard関数で SRCDIR以下のcファイルをリスト
SRC = helloworld1.c


TARGET = helloworld1

$(TARGET): $(SRC)
	$(CC) -Wall -o $@ $^ 

.PHONY: tmpclean clean

tmpclean:
	rm -f *~ *~
clean: tmpclean
	rm -f $(TARGET) 
