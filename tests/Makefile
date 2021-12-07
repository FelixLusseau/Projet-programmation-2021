

TARGET   = tests

CFLAGS=-Wall -Wextra -Werror -g -fprofile-arcs -ftest-coverage

LDLIBS=-lm


INCLUDE_PATH = ../include

SRCDIR   = ../src
OBJDIR   = obj

SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(INCLUDE_PATH)/*.h)
OBJECTS0  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
OBJECTS = $(filter-out $(OBJDIR)/bmp-msg.o,$(OBJECTS0))



tests: $(OBJECTS) obj/tests.o

	$(CC) -o $@ $^ $(CFLAGS) $(LDLIBS)
	@echo "Linking complete!"


cov: clean tests
	valgrind --leak-check=full -q ./$(TARGET)
	lcov -d obj -c -o rapport.info
	genhtml -o rapport -t "Couverture" rapport.info


$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	mkdir -p $(OBJDIR)
	$(CC) -o $@ -c $< $(CFLAGS) -I$(INCLUDE_PATH)

obj/tests.o: tests.c
	$(CC) -o $@ -c $< $(CFLAGS) -I$(INCLUDE_PATH)


.PHONY: clean cov
clean:
	rm -f $(OBJDIR)/*.o
	rm -f $(OBJDIR)/*.gcda
	rm -f $(OBJDIR)/*.gcno
	rm -f $(BINDIR)/$(TARGET)
