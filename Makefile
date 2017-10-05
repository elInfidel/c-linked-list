SOURCES=addressbook.c addressbook_list.c commands.c helpers.c
HEADERS=addressbook.h addressbook_list.h commands.h helpers.h
PROGRAM=addressbook
FLAGS=-ansi -pedantic -Wall
OBJS=addressbook.o addressbook_list.o commands.o helpers.o

all: $(OBJS)
	@echo "Linking..."
	gcc $(FLAGS) -o $(PROGRAM) $(OBJS)

%.0: %.c
	@echo "Compiling..."
	gcc $(FLAGS) -c -o $@ $^

clean:
	@echo "Cleaning..."
	rm *.o $(PROGRAM)

archive:
	@echo "Creating archive..."
	zip $(USER)-a2 $(SOURCES) $(HEADERS) Makefile
