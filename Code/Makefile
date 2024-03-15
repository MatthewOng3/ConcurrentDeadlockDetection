# Variables
CC=mpic++
CFLAGS=-fopenmp
SRC=  util.cpp MPIDD_Manager.cpp MPIDD_DeadlockDetector.cpp MPIDD_State.cpp MPIDD_Wrapper.cpp MPIDD_Commands.cpp main.cpp
OUT=Demo

# Targets and Recipes
all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

run: $(OUT)
	mpirun -np 3 --oversubscribe ./$(OUT)  

clean:
	rm -f $(OUT)
