vpath %.cpp src/
vpath %.h include/

.PHONY: all clean

SRCS := $(wildcard src/*.cpp)
OBJS := $(addprefix obj/,$(notdir $(SRCS:.S=.o)))

CC = g++ 
CFLAGS = -g -pthread -ldl -w -o3 -std=gnu++11
O_REP = obj/
B_REP = bin/
GUROBI_PATH = $(GUROBI_HOME)
INC = -I include -I$(GUROBI_PATH)/include
LIBRARIES = -L$(GUROBI_PATH)/lib -lgurobi_c++ -lgurobi81

all: rep Slideshow

rep:
	if [ ! -d obj ];then mkdir obj; fi; if [ ! -d bin ]; then mkdir bin; fi; if [ ! -d solutions ]; then mkdir solutions; fi

Slideshow: Slideshow.o Instance.o Solver.o Hori_verti.o Naive_greedy.o Greedy.o Random.o Stoch_descent.o Annealing.o Genetic.o Ilp.o Round.o
	$(CC) $(patsubst %,$(O_REP)%,$^) $(CFLAGS) $(INC) $(LIBRARIES) -o $(B_REP)$@

obj/%.o: sources/%.S
	$(CC) $(CFLAGS) -o $@ $< $(INCLUDES)

%.o: %.cpp
	$(CC) -c $< $(INC) $(LIBRARIES) $(CFLAGS) -o $(O_REP)$@

clean:
	rm -f obj/*.o bin/* log* vgcore*
