SER_EXE = server
SER_OBJS = server.o

CLI_EXE = clients
CLI_OBJS = clients.o

all:$(SER_EXE) $(CLI_EXE)

$(SER_EXE):$(SER_OBJS)
	$(CC) -o $(SER_EXE) $(SER_OBJS)

$(CLI_EXE):$(CLI_OBJS)
	$(CC) -o $(CLI_EXE) $(CLI_OBJS)

$.o:$.c
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY:clean
clean:
	rm -f $(SER_EXE) $(SER_OBJS) $(CLI_EXE) $(CLI_OBJS)

