#---------------------------------------------------------------------------------------------------
# DEFINITIONS
#---------------------------------------------------------------------------------------------------

# Compiler
CC        := gcc

# Build Target Program
TARGET    := mcp3008-lsl

# Project Directories: Includes, Build, etc.
SRCDIR    := ./
INCDIR    := include/
BUILDDIR  := obj/

# Project file extensions
SRCEXT    := c
OBJEXT    := o

# Flags, Libraries and Includes
CFLAGS    := -g -Wall
LIB       := -llsl
INC       := -I$(INCDIR)

#---------------------------------------------------------------------------------------------------
# TARGETS
#---------------------------------------------------------------------------------------------------
SRCS      := $(wildcard $(SRCDIR)*.$(SRCEXT))
OBJS      := $(patsubst $(SRCDIR)%.$(SRCEXT),$(BUILDDIR)%.$(OBJEXT),$(SRCS))

.PHONY: all clean

# Default
all: directories $(TARGET)

# Target to make the needed directories
directories:
	@mkdir -p $(BUILDDIR)

clean:
	@$(RM) -rf $(BUILDDIR)

# Target to link object files
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS) $(LIB)

# Target to compile src files 
$(BUILDDIR)%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c -o $@ $^ $(LFLAGS) $(LIBS)
