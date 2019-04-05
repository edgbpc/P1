CC	= g++
CFLAGS	= 

TARGET1	= scanner
OBJS1	= main.o
OBJS2	= scanner.o
OBJS3	= testScanner.o
CXXFLAGS = -std=c++11

.SUFFIXES: .c .o

all:	$(TARGET1)

scanner:	$(OBJS1) $(OBJS2) $(OBJS3)
		$(CC) $(CFLAGS) $(CXXFLAGS) $(OBJS1) $(OBJS2) $(OBJS3) -o $@


.c.o:
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	/bin/rm -f *.o *.dat  *~ $(TARGET1)
