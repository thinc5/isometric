#-------------- Variables
# executable name
TARGET	:= iso
# compiler to use
CC		:= gcc
# compiler flags
CFLAGS  := -std=gnu11 -O3 -Wall -pedantic -MD
# linker to use
LINKER  := gcc
# linking flags
LFLAGS	:= -lm -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
LFLAGS	+= -Isrc/src
# directories
SRCDIR	:= src
INCDIR	:= src
OBJDIR	:= obj
BINDIR	:= .


#-------------- Helper scripts
rm		:= rm -rf
mkdir	:= mkdir -p
findc	:= du -a $(SRCDIR) | grep -E '\.(c)$$' | awk '{print $$2}'
findh	:= du -a $(INCDIR) | grep -E '\.(h)$$' | awk '{print $$2}'

#-------------- Populate build sources
SOURCES  := $(shell $(findc))
INCLUDES := $(shell $(findh))

# What are my objects?
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# make subfolders if they do not already exist.
XTRADIR  := $(shell ls -d $(INCDIR)/*/** | sed 's/$(INCDIR)/$(OBJDIR)/g')
$(shell $(mkdir) $(OBJDIR) $(XTRADIR))


#-------------- Rules
# default rules
all: build

# debug build?
debug: CFLAGS += -DDEBUG -g
debug: LFLAGS += -DDEBUG -g
debug: build

# compile and link
build: $(OBJECTS) $(BINDIR)/$(TARGET)

# rebuild.
rebuild: clean
	$(MAKE) build

# compile objects.
$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@
	$(info Compiled $<)


# link objects.
$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(LINKER) $(OBJECTS) $(LFLAGS) -o $@
	$(info Binary: $@)


.PHONY:	clean

# clean all building materials.
clean:
	@$(rm) $(OBJDIR)
	@echo "Cleanup complete!"
	@$(rm) $(BINDIR)/$(TARGET)
	@echo "Executable removed!"
