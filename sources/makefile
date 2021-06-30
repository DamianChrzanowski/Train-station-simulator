CC=g++
CFLAGS=-pthread -std=c++17 -lncurses

all: main

run: main

main: main.cpp
	$(CC) -o main main.cpp transfer_center.cpp screen_printer.cpp sync_channel.cpp passenger.cpp place.cpp bus.cpp subway.cpp ticket_machine.cpp stairs.cpp bench.cpp door.cpp $(CFLAGS)

clean: main.cpp
	rm main
	$(CC) -o main main.cpp transfer_center.cpp screen_printer.cpp sync_channel.cpp passenger.cpp place.cpp bus.cpp subway.cpp ticket_machine.cpp stairs.cpp bench.cpp door.cpp $(CFLAGS)