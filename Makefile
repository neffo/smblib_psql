bindir = --target-directory=/usr/local/bin

INSTALL_PROGRAM = /usr/bin/install -c

#MASTER = "uberoldman"
MASTER = "oldman"

#CC = icc
CC = gcc

#CFLAGS = -O6 -Wall -mpentiumpro -Wstrict-prototypes -fomit-frame-pointer -malign-functions=2 -fno-strength-reduce -malign-loops=2 -malign-jumps=2
#CFLAGS = -g -O6 -Wall
#CFLAGS = -Wall -pg -O6
#CFLAGS = -tpp6 -axiM -O3 -g -ipo -w2
#CFLAGS = -w2 -g
CFLAGS = -Wall -g

DEFS = -DMASTER=\"$(MASTER)\"
COMPILE_FLAGS = $(CXXFLAGS) $(DEFS)

OBJS = 	create.o \
	connect.o \
	errors.o \
	string.o \
	dirlist.o \
	machlist.o \
	file.o \
	get.o

SMBCDOBJS = smbcd.o

BIN =	test \
	test2 \
	sls \
	scd \
	text_index \
	test_download \
	batchdl \
	super_index \
	threaded_index


all: 	$(OBJS) $(BIN)

create.o: 	src/create.c src/smblib.h src/errors.h
	$(CC) src/create.c -c $(CFLAGS)

connect.o:	src/connect.c src/smblib.h src/errors.h
	$(CC) src/connect.c -c $(CFLAGS)

errors.o:	src/errors.c src/smblib.h src/errors.h
	$(CC) src/errors.c -c $(CFLAGS)

string.o:	src/string.c src/smblib.h src/errors.h
	$(CC) src/string.c -c $(CFLAGS)

dirlist.o:	src/dirlist.c src/smblib.h src/errors.h
	$(CC) src/dirlist.c -c $(CFLAGS)

machlist.o:	src/machlist.c src/smblib.h src/errors.h
	$(CC) src/machlist.c -c $(CFLAGS) -DMASTER=\"$(MASTER)\"

smbcd.o:	src/smbcd.c src/smbcd.h src/smblib.h
	$(CC) src/smbcd.c -c $(CFLAGS)

file.o:		src/file.c src/smblib.h
	$(CC) src/file.c -c $(CFLAGS)

get.o:		src/get.c src/smblib.h src/errors.h
		$(CC) src/get.c -c $(CFLAGS)

test:		src/test.c $(OBJS)
	$(CC) src/test.c -o test $(CFLAGS) $(OBJS)

test2:		src/test2.c $(OBJS)
	$(CC) src/test2.c -o test2 $(CFLAGS) $(OBJS)

sls:		src/sls.c $(SMBCDOBJS) $(OBJS)
	$(CC) src/sls.c -o sls $(CFLAGS) $(SMBCDOBJS) $(OBJS)

scd:		src/scd.c $(SMBCDOBJS) $(OBJS)
	$(CC) src/scd.c -o scd $(CFLAGS) $(SMBCDOBJS) $(OBJS)

index: 		src/index.c $(SMBCDOBJS) $(OBJS)
	$(CC) src/index.c -o index $(CFLAGS) $(SMBCDOBJS) $(OBJS) -lpq -I/usr/include/postgresql/

text_index:	src/text_index.c $(SMBCDOBJS) $(OBJS)
	$(CC) src/text_index.c -o text_index  $(CFLAGS) $(SMBCDOBJS) $(OBJS)

threaded_index:	src/threaded_index.c $(SMBCDOBJS) $(OBJS)
	$(CC) src/threaded_index.c -o threaded_index  $(CFLAGS) $(SMBCDOBJS) $(OBJS)

test_download: src/test_download.c $(SMBCDOBJS) $(OBJS)
	$(CC) src/test_download.c -o test_download $(CFLAGS) $(SMBCDOBJS) $(OBJS)

batchdl:	src/batchdl.c $(SMBCDOBJS) $(OBJS)
	$(CC) src/batchdl.c -o batchdl $(CFLAGS) $(SMBCDOBJS) $(OBJS)

super_index:          src/super_index.c $(SMBCDOBJS) $(OBJS)
	$(CC) src/super_index.c -o super_index $(CFLAGS) $(SMBCDOBJS) $(OBJS) -lpq -I/usr/include/postgresql/

targz:
	bash -c "cd ..;tar zcvf smblib-psql-`date +%Y%m%d`.tar.gz smblib-psql/"

clean:
	rm *.o *~ $(BIN)
