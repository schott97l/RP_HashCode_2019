vpath %.cpp src/
vpath %.h include/
vpath server bin/
vpath client bin/

.PHONY: all clean test

CC = g++ 
CFLAGS = -g -pthread -ldl -w -std=gnu++98
O_REP = obj/
B_REP = bin/
L_REP = lib/

CHECKER = Checker
HORI_VERTI = Hori_verti
GLOUTON = Glouton
STOCH_DESCENTE = Stoch_descente
GENETIC = Genetic
PLNE = Plne

all: $(CHECKER) $(HORI_VERTI) #$(GLOUTON) $(STOCH_DESCENTE) $(GENETIC) $(PLNE)

$(CHECKER): Checker.o Instance.o
	$(CC) $(patsubst %,$(O_REP)%,$^) $(CFLAGS) -o $(B_REP)$@

$(HORI_VERTI): Hori_verti.o Instance.o
	$(CC) $(patsubst %,$(O_REP)%,$^) $(CFLAGS) -o $(B_REP)$@

Checker.o: Checker.cpp Instance.h
Instance.o: Instance.cpp Instance.h
Hori_verti.o: Hori_verti.cpp Hori_verti.h

%.o: %.cpp
	$(CC) -c $< -I include $(CFLAGS) -o $(O_REP)$@

clean:
	rm -f obj/*.o bin/* log*

test:
	./test.sh
