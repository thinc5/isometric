FORMATTER		:= uncrustify
FORMAT_CONFIG	:= clean.cfg

#-------------- Variables
# executable name
TARGET	:= iso
# compiler to use
CC		:= gcc
# compiler flags
CFLAGS  := -std=gnu11 -Wall -pedantic -MD
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


#-------------- helper scripts
rm		:= rm -rf
mkdir	:= mkdir -p
findc	:= du -a $(SRCDIR) | grep -E '\.(c)$$' | awk '{print $$2}'
findh	:= du -a $(INCDIR) | grep -E '\.(h)$$' | awk '{print $$2}'

#-------------- populate build sources
SOURCES  	:= $(shell $(findc))
INCLUDES 	:= $(shell $(findh))
RESOURCES	:= ./$(SRCDIR)/tiles.h ./$(SRCDIR)/font.h

FORMAT_TARGETS	:= $(SOURCES) $(INCLUDES)

# what are my objects?
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# make subfolders if they do not already exist.
XTRADIR  := $(shell ls -d $(INCDIR)/*/** | sed 's/$(INCDIR)/$(OBJDIR)/g')
$(shell $(mkdir) $(OBJDIR) $(XTRADIR))


#-------------- rules
# default rules
all: build

# debug build?
debug: CFLAGS += -DDEBUG -g
debug: LFLAGS += -DDEBUG -g
debug: build

release: CFLAGS += -DRELEASE -O3
release: build

# prepare respources
$(RESOURCES) : ./res/iso_demo.png ./res/ssp-regular.otf
	@xxd -i ./res/iso_demo.png > ./$(SRCDIR)/tiles.h
	@xxd -i ./res/ssp-regular.otf > ./$(SRCDIR)/font.h

# compile and link
build: $(RESOURCES) $(OBJECTS) $(BINDIR)/$(TARGET)

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

$(FORMAT_TARGETS):
	@$(FORMATTER) -c $(FORMAT_CONFIG) -q -f $@ -o $@

format_clean:
	@$(RM) **.unc*
	@$(RM) **/**.unc*

format: $(FORMAT_TARGETS) format_clean

.PHONY:	clean format

# clean all building materials.
clean:
	$(rm) $(RESOURCES)
	@$(rm) $(OBJDIR)
	@echo "Cleanup complete!"
	@$(rm) $(BINDIR)/$(TARGET)
	@echo "Executable removed!"
