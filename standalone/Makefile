default: readData setPosition cleanObjects

readData: readData.o reflexHand.o
	g++ -g -o readData -I./ readData.o reflexHand.o

readData.o: readData.cpp reflexHand.h
	g++ -g -c readData.cpp -I./

reflexHand.o: reflexHand.h reflexHand.cpp
	g++ -g -c reflexHand.cpp -I./

setPosition: setPosition.o reflexHand.o
	g++ -g -o setPosition -I./ setPosition.o reflexHand.o

setPosition.o: setPosition.cpp reflexHand.h
	g++ -g -c setPosition.cpp -I./

cleanObjects:
	rm reflexHand.o readData.o setPosition.o

clean:
	rm readData setPosition reflexHand.o