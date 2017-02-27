#makefile for Homework5

Homework5: Homework5.c
	gcc -o Homework5 Homework5.c
clean:
	rm Homework5
tar:
	tar -cf Homework5.tar Homework5.c makefile
