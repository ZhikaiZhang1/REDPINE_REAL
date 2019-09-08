##----------------------------------------------------------------------------
# Created with genmake.pl v1.1 on Tue Jul  5 12:37:52 2016
# genmake.pl home: http://muquit.com/muquit/software/
# Copryright: GNU GPL (http://www.gnu.org/copyleft/gpl.html)
##----------------------------------------------------------------------------
rm=/bin/rm -f
CC= gcc

DEFS=  
PROGNAME= rsi_server_app
INCLUDES  = -I ./ 

DEFINES= $(INCLUDES) $(DEFS) 
CFLAGS= -g $(DEFINES) -Os

SRCS = aes.c \
			 misc.c \
			 Server_puf.c \

OBJS =  aes.o \
				misc.o \
				Server_puf.o \

.c.o:
	$(rm) $@
	$(CC) $(CFLAGS) -c $*.c

all: $(PROGNAME)

$(PROGNAME) : $(OBJS) 
	$(CC) $(CFLAGS) -o $(PROGNAME) $(OBJS)

clean:
	$(rm) $(OBJS) $(PROGNAME) 
