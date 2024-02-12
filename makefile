all: my_fm

my_fm: my_fm.c 
	gcc -Wall -o flame_cp my_fm.c

clean:
	$(RM) my_fm