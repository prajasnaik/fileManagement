all: my_fm

my_fm: my_fm.c 
	gcc -Wall -o my_fm my_fm.c

clean:
	$(RM) my_fm