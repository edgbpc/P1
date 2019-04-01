CC = g++
CFLAGS =

TARGET1 = scanner
OBJS1 = main.o
OBJS2 = scanner.o
OBJS3 = testScanner.o

.SUFFIXES: .c .o

all: $(TARGET1)

scanner: $(OBJS1) $(OBJS2) $(OBJS3)
        $(CC) $(CFLAGS) $(OBJS1) $(OBJS2) $(OBJS3) -o $@


.c.o:
    $(CC) $(CFLAGS) -c $<

.PHONY
clean:
    /bin/rm -f *.o *~ $(TARGET1)
