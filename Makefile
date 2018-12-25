chan:
	gcc chan.c chan_test.c -pthread

clean:
	rm *.o *.out *.gch
