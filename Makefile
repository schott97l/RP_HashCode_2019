vpath %.cpp src/
vpath %.h include/

.PHONY: all clean

CC = g++ 
CFLAGS = -g -pthread -ldl -w -std=gnu++98
O_REP = obj/
B_REP = bin/
L_REP = lib/

all: rep Checker Slideshow

rep:
	if [ ! -d obj ]; then  mkdir obj; fi; if [ ! -d bin ]; then mkdir bin; fi;

Checker: Checker.o Instance.o
	$(CC) $(patsubst %,$(O_REP)%,$^) $(CFLAGS) -o $(B_REP)$@

Slideshow: Slideshow.o Instance.o Solver.o Hori_verti.o Greedy.o Stoch_descent.o Genetic.o Ilp.o
	$(CC) $(patsubst %,$(O_REP)%,$^) $(CFLAGS) -o $(B_REP)$@

Checker.o: Checker.cpp Instance.h
Slideshow.o: Slideshow.cpp Instance.h
Instance.o: Instance.cpp Instance.h
Solver.o: Solver.cpp Solver.h
Hori_verti.o: Hori_verti.cpp Hori_verti.h
Greedy.o: Greedy.cpp Greedy.h
Stoch_descent.o: Stoch_descent.cpp Stoch_descent.h
Genetic.o: Genetic.cpp Genetic.h
Ilp.o: Ilp.cpp Ilp.h

%.o: %.cpp
	$(CC) -c $< -I include $(CFLAGS) -o $(O_REP)$@

clean:
	rm -f obj/*.o bin/* log* vgcore*
