cchan:
	gcc chan.c chan_test.c -pthread

cppchan:
	g++ channel.cpp -std=c++17 -pthread

clean:
	rm *.o *.out *.gch
