#### Test Makefile
# Change this to where your folder with your header files (gtthread.h) and gtthread.a are located
# relative to this makefile.
INCLIB  = -I../
LIBRARY = ../gtthread.a
# This is needed when you want to compile and debug the executable. Build your
# programs object files with the -ggdb3 flag to include debug symbols and point
# this to a folder with these object files.
OBJDIR  = ../obj

# PROBABLY NO NEED TO MODIFY ANYTHING BELOW THIS LINE.
# -std=c11 doesn't include some system specific structures (sigactions, ...).
# using gnull instead.
CFLAGS  = -Wall -std=gnu11
DFLAGS  = -ggdb3
CC      = gcc
CSRCDIR = src
BUILDD  = build
DEBUGD  = debug
LINKEDFILES =

# Get all the test source files
CSOURCES := $(wildcard $(CSRCDIR)/*.c)
OBJECTFILES := $(wildcard $(OBJDIR)/*.o)
# List of the files we want to generate
TESTS = $(patsubst $(CSRCDIR)/%.c, $(BUILDD)/%, $(CSOURCES))
DEBUGFILES = $(patsubst $(CSRCDIR)/%.c, $(DEBUGD)/%, $(CSOURCES))

# Whenever files in the sources change generate all executables:
# Then generate the test programs
all:  $(TESTS)
	bash runtests.sh

# Generic build rule for each file to build (Target=build/testX:
# Prerequisite=src/testX).
$(BUILDD)/%: $(CSRCDIR)/%.c
# Use the specified compiler (CC) with the compile flags set to (CFLAGS) and
# debug flags to (DFLAGS)
# and using includes from the specified path (INCLIB where gtthread.h
# lies) use the file $< that specifies the first prerequisite in order
# to create the executable $@ (target of the rule) by linking with the
# library (LIBRARY which points to ../gtthread.a)
	$(CC) $(CFLAGS) $(INCLIB) $< -o $@ $(LIBRARY)

builddebug: $(DEBUGFILES)

# The build the files.
debug: $(DEBUGFILES)

$(DEBUGD)/%: $(CSRCDIR)/%.c
	$(CC) $(CFLAGS) $(DFLAGS) $(INCLIB) $< -o $@ $(OBJECTFILES)



# Remove the generated files.
clean:
	$(RM) $(BUILDD)/*
	$(RM) $(DEBUGD)/*
