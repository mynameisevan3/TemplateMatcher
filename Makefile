
################################################################################
# Makefile                                                                     #
################################################################################

# Executable Name
C_EXE       := templateMatch

# Source and Object Directories
C_SRC_DIR   := src/c
C_OBJ_DIR   := obj/c
CPP_SRC_DIR := src/c++
CPP_OBJ_DIR := obj/c++
OBJ_DIR     := obj

# Files for Executable
C_NAMES     := main.c templateMatch.c ppmTools.c

# Flags and Libraries
CFLAGS      := -std=c99 -Wall -Wextra -fopenmp -O3
CPPFLAGS    := -I.
LDFLAGS     := -fopenmp
LDLIBS      := -lm

# Translation to Directory Paths - DON'T TOUCH!
C_SRC   := $(patsubst %.c,$(C_SRC_DIR)/%.c,$(C_NAMES))
C_OBJ   := $(patsubst %.c,$(C_OBJ_DIR)/%.o,$(C_NAMES))

################################################################################
# Make All

all: $(C_EXE)

################################################################################
# Make C Executable

$(C_EXE): $(C_OBJ)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

################################################################################
# Make Objects

$(C_OBJ_DIR)/%.o: $(C_SRC_DIR)/%.c | $(C_OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(C_OBJ_DIR):
	mkdir -p $@

################################################################################
#  Clean

clean:
	$(RM) -r $(C_OBJ_DIR)
	$(RM) -r $(CPP_OBJ_DIR)
	$(RM) -r $(OBJ_DIR)
	$(RM) $(C_EXE)

.PHONY: all clean

#########################################################



# END Makefile  - EWG SDG
