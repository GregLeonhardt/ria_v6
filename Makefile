########################################################################
# Compiler settings
CC		=	gcc
#-----------------------------------------------------------------------
RM		=	rm
#-----------------------------------------------------------------------
LIBTOOLS	=	../libtools
#-----------------------------------------------------------------------
CFLAGS		=	-DDEBUG -Wall -g -O  -MMD
CFLAGS		+=	-I. -Iinclude
CFLAGS		+=	-I$(LIBTOOLS)/include/
CFLAGS		+=	-Idecode/include
CFLAGS		+=	-Iimport/include
#-----------------------------------------------------------------------
LFLAGS		=	-o 
#-----------------------------------------------------------------------

########################################################################
# Makefile settings
APPNAME = ria

########################################################################
# Generate the file lists
#-----------------------------------------------------------------------
SRC		=	main.c
#
SRC		+=	$(wildcard decode/*.c)
SRC		+=	$(wildcard decode/*/*.c)
#
SRC		+=	$(wildcard import/*.c)
SRC		+=	$(wildcard import/*/*.c)
#
SRC		+=	$(wildcard xlate/*.c)
#-----------------------------------------------------------------------
OBJ		=	$(SRC:.c=.o)
#-----------------------------------------------------------------------
DEP		=	$(SRC:.c=.d)
#-----------------------------------------------------------------------

########################################################################
# Build dependant files, Compile everything, and build the library
all: $(APPNAME)

$(APPNAME): $(OBJ) ../tools/libtools.a
	@echo "BUILD THE APPLICATION:"
	$(CC) $(LFLAGS) $(APPNAME) $(OBJ) $(LIBTOOLS)/libtools.a -lpthread -ldl

-include $(DEP)

../tools/libtools.a:
	cd $(LIBTOOLS) ; make all

########################################################################
# Cleans everything
cleanall:   clean   cleandep

########################################################################
# Cleans object and library file(s)
clean:
	@echo "CLEAN:"
	$(RM) -f $(OBJ) $(APPNAME)

########################################################################
# Cleans dependent file(s)
cleandep:
	@echo "CLEANDEP:"
	$(RM) -f $(DEP)
