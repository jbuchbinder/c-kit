
clean:
	rm -f *.o

.c.o:
	gcc -c -o $@ $<

