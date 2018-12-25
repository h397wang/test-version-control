cchan:
	gcc c/*.c -pthread

cppchan:
	g++ cpp/*.cpp -std=c++17 -pthread

clean:
	rm *.o *.out *.gch
