vpath %.cpp src/
vpath %.h include/

.PHONY: all clean

SRCS := $(wildcard src/*.cpp)
OBJS := $(addprefix obj/,$(notdir $(SRCS:.S=.o)))

CC = g++ 
CFLAGS = -g -pthread -ldl -w -o3 -std=gnu++11
O_REP = obj/
B_REP = bin/

all: rep Checker Slideshow

rep:
	if [ ! -d obj ]; then  mkdir obj; fi; if [ ! -d bin ]; then mkdir bin; fi;

Checker: Checker.o Instance.o
	$(CC) $(patsubst %,$(O_REP)%,$^) $(CFLAGS) -o $(B_REP)$@

Slideshow: Slideshow.o Instance.o Solver.o Hori_verti.o Naive_greedy.o Greedy.o Stoch_descent.o Genetic.o Ilp.o
	$(CC) $(patsubst %,$(O_REP)%,$^) $(CFLAGS) -o $(B_REP)$@

obj/%.o: sources/%.S
	$(CC) $(CFLAGS) -o $@ $< $(INCLUDES)

%.o: %.cpp
	$(CC) -c $< -I include $(CFLAGS) -o $(O_REP)$@

clean:
	rm -f obj/*.o bin/* log* vgcore*
