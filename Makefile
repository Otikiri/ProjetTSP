#Compiler and flags 
CC = gcc
CFLAGS = -std=gnu11 -Wall
LDLIBS = -lpthread -lm

#Directories 
SRCDIR = src
OBJDIR = obj
BINDIR = bin

#Binaries
TSP = $(BINDIR)/tsp
TEST = $(BINDIR)/p2temp

#Source files
SRC = $(wildcard $(SRCDIR)/*.c)

#Object files
OBJ_ALL = $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

#Objects for the differents executables
OBJ_TSP = $(filter-out $(OBJDIR)/p2tempmain.o, $(OBJ_ALL))
OBJ_TEST = $(filter-out $(OBJDIR)/main.o, $(OBJ_ALL))

all: dirs $(TSP) $(TEST)

# Build demo binary
$(TSP): $(OBJ_TSP)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

# Build test binary
$(TEST): $(OBJ_TEST)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

# Compile rule
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create directories
dirs:
	@mkdir -p $(OBJDIR) $(BINDIR)

# Clean
clean:
	rm -f $(OBJDIR)/*.o
	rm -f $(BINDIR)/*
