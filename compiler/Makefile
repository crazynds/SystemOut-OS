
ifeq ($(OS),Windows_NT)
	PROG = exec.exe
else
	PROG = exec
endif


all: $(PROG)
	./$(PROG)

$(PROG):
	gcc -O2 main.c -o $(PROG)
