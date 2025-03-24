TARGET = program
SOURCES = main.c
HEADERS = header.h

$(TARGET): $(SOURCES) $(HEADERS)
	gcc $(SOURCES) -o $@
	