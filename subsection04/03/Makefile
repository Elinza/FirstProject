FINAL_EXE = tcpselect
FINAL_OBJS = tcpselect.o

SER_EXE = server
SER_OBJS = server.o

CLI_EXE = clients
CLI_OBJS = clients.o
CXXFLAGS = -std=gnu++11

EXE = selectser
OBJS = selectser.o

SEL_EXE = selectcli
SEL_OBJS = selectcli.o

all:$(SER_EXE) $(CLI_EXE) $(EXE) $(SEL_EXE) $(FINAL_EXE)

$(FINAL_EXE):$(FINAL_OBJS)
	$(CXX) -o $(FINAL_EXE) $(FINAL_OBJS) -lpthread

$(SER_EXE):$(SER_OBJS)
	$(CXX) -o $(SER_EXE) $(SER_OBJS)

$(CLI_EXE):$(CLI_OBJS)
	$(CXX) -o $(CLI_EXE) $(CLI_OBJS) -lpthread

$(EXE) :$(OBJS)
	$(CC) -o $(EXE) $(OBJS)

$(SEL_EXE):$(SEL_OBJS)
	$(CXX) -o $(SEL_EXE) $(SEL_OBJS)

$.o:$.c
	$(CC) -c $(CFLAGS) $< -o $@
$.o:$.cc
	$(CXX) -c $(CXXFLAGS) $< -o $@

.PHONY:clean
clean:
	rm -f $(SER_EXE) $(SER_OBJS) $(CLI_EXE) $(CLI_OBJS) $(EXE) $(OBJS) 
	$(SEL_EXE) $(SEL_OBJS) $(FINAL_EXE) $(FINAL_OBJS)

