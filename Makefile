bindir = --target-directory=/usr/local/bin

INSTALL_PROGRAM = /usr/bin/install -c

MASTER = "uberoldman"

CC = icc

#CFLAGS = -O6 -Wall -mpentiumpro -Wstrict-prototypes -fomit-frame-pointer -malign-functions=2 -fno-strength-reduce -malign-loops=2 -malign-jumps=2
#CFLAGS = -g -O6 -Wall
#CFLAGS = -Wall -pg -O6
#CFLAGS = -tpp6 -axiM -O3 -g -ipo -w2
CFLAGS = -w2 -g

DEFS = -DMASTER=\"$(MASTER)\"
COMPILE_FLAGS = $(CXXFLAGS) $(DEFS)

OBJS = 	create.o \
	connect.o \
	errors.o \
	string.o \
	dirlist.o \
	machlist.o \
	file.o
	
SMBCDOBJS = smbcd.o

BIN =	test \
	test2 \
	sls \
	scd \
	text_index #\
#	threaded_index
	

all: 	$(OBJS) $(BIN)

create.o: 	create.c smblib.h errors.h
	$(CC) create.c -c $(CFLAGS)

connect.o:	connect.c smblib.h errors.h
	$(CC) connect.c -c $(CFLAGS)

errors.o:	errors.c smblib.h errors.h
	$(CC) errors.c -c $(CFLAGS)

string.o:	string.c smblib.h errors.h
	$(CC) string.c -c $(CFLAGS)

dirlist.o:	dirlist.c smblib.h errors.h
	$(CC) dirlist.c -c $(CFLAGS)

machlist.o:	machlist.c smblib.h errors.h
	$(CC) machlist.c -c $(CFLAGS) -DMASTER=\"$(MASTER)\"

smbcd.o:	smbcd.c smbcd.h smblib.h
	$(CC) smbcd.c -c $(CFLAGS)

file.o:		file.c smblib.h
	$(CC) file.c -c $(CFLAGS)



test:		test.c $(OBJS)
	$(CC) test.c -o test $(CFLAGS) $(OBJS)

test2:		test2.c $(OBJS)
	$(CC) test2.c -o test2 $(CFLAGS) $(OBJS)

sls:		sls.c $(SMBCDOBJS) $(OBJS)
	$(CC) sls.c -o sls $(CFLAGS) $(SMBCDOBJS) $(OBJS)

scd:		scd.c $(SMBCDOBJS) $(OBJS)
	$(CC) scd.c -o scd $(CFLAGS) $(SMBCDOBJS) $(OBJS)

index: 		index.c $(SMBCDOBJS) $(OBJS)
	$(CC) index.c -o index $(CFLAGS) $(SMBCDOBJS) $(OBJS) -lpq -I/usr/include/pgsql/

text_index:	text_index.c $(SMBCDOBJS) $(OBJS)
	$(CC) text_index.c -o text_index  $(CFLAGS) $(SMBCDOBJS) $(OBJS)

threaded_index:	threaded_index.c $(SMBCDOBJS) $(OBJS)
	$(CC) threaded_index.c -o threaded_index  $(CFLAGS) $(SMBCDOBJS) $(OBJS)

super_index:          super_index.c $(SMBCDOBJS) $(OBJS)
	$(CC) super_index.c -o super_index $(CFLAGS) $(SMBCDOBJS) $(OBJS) -lpq -I/usr/include/pgsql/

targz:
	bash -c "cd ..;tar zcvf smblib-psql-`date +%Y%m%d`.tar.gz smblib-psql/"

clean:
	rm *.o *~ $(BIN)
