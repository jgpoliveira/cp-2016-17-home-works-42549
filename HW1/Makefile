CC=gcc
# GCC FLAGS FOR COMPILE
CFLAGS=-Wall -g -pedantic
LDFLAGS=
PROGRAMFLAGS= -n 20 -d

# OUTPUT terminologies
OUTPUTTESTS=output/
OUTTERM=.out

# INPUT
INPUTTESTS=input/

# EXECUTABLE PROGRAM
OUTPUT=glife

# SOURCES
SRC=config.c game.c main.c
OBJ=$(patsubst %.c,%.o,$(SRC))

all:
	$(CC) $(CFLAGS) -c $(SRC)
	$(CC) $(LDFLAGS) -o $(OUTPUT) $(OBJ)

clean:
	rm -f $(OUTPUT) $(OBJ)
	rm -rf $(OUTPUTTESTS)

test: clean all
	mkdir -p $(OUTPUTTESTS)
	for test in $(INPUTTESTS); do \
	  test2="$${test##*/}"; \
		out="$${test2%.*}"; \
		./$(OUTPUT) $(PROGRAMFLAGS) $$test >> $(OUTPUTTESTS)$$out$(OUTTERM) ; \
	done
