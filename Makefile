CC := g++
SRCDIR := src
BUILDDIR := build
TARGET := main

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -g -Wall -Wshadow -std=c++20 -Wno-unused-result -Wno-sign-compare -Wno-char-subscripts 
INC := -I include/

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $(TARGET)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

test: main
	./main

clean:
	$(RM) -r $(BUILDDIR)/* $(TARGET)

.PHONY: clean
