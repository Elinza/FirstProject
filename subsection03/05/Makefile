EXE = thread
OBJS = thread.o

all:$(EXE)

$(EXE):$(OBJS)
	$(CXX) -o $(EXE) $(OBJS) -lpthread

$.o:$.cc
	$(CXX) -c $(CXXFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f $(EXE) $(OBJS)

