########################################################################
#   MAKEFILE for Recipe Import Assist [RIA]
########################################################################
# Compiler settings
CC		=	gcc
#-----------------------------------------------------------------------
RM		=	rm
#-----------------------------------------------------------------------
LIBTOOLS	=	../libtools
#-----------------------------------------------------------------------
CFLAGS		=	-DDEBUG -Wall -g -MMD
CFLAGS		+=	-I. -Iinclude
CFLAGS		+=	-I$(LIBTOOLS)/include/
#-----------------------------------------------------------------------
LFLAGS		=	-o
#-----------------------------------------------------------------------
LLIBRARY	=	$(LIBTOOLS)/libtools.a -lcrypto -lpthread -ldl
#-----------------------------------------------------------------------

########################################################################
# Makefile settings
APPNAME = ria

########################################################################
# Generate the file lists
#-----------------------------------------------------------------------
SRC		=	main.c
#
SRC		+=	$(wildcard router/*.c)
#
SRC		+=	$(wildcard import/*.c)
#
SRC		+=	$(wildcard email/*.c)
#
SRC		+=	$(wildcard decode/*.c)
SRC		+=	$(wildcard decode/*/*.c)
#
SRC		+=	$(wildcard encode/*.c)
SRC		+=	$(wildcard encode/*/*.c)
#
SRC		+=	$(wildcard export/*.c)
#
SRC		+=	$(wildcard monitor/*.c)
#
SRC		+=	$(wildcard recipe/*.c)
#
SRC		+=	$(wildcard rcb/*.c)
#
SRC		+=	$(wildcard tcb/*.c)
#
SRC		+=	$(wildcard xlate/*.c)
#-----------------------------------------------------------------------
OBJ		=	$(SRC:.c=.o)
#-----------------------------------------------------------------------
DEP		=	$(SRC:.c=.d)
#-----------------------------------------------------------------------

########################################################################
# Build dependant files, Compile everything, and build the library
all: $(APPNAME) Makefile

$(APPNAME): $(OBJ) ../tools/libtools.a
	@echo "BUILD THE APPLICATION:"
	$(CC) $(LFLAGS) $(APPNAME) $(OBJ) $(LLIBRARY)

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
