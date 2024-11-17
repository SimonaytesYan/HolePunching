BIN = bin
OBJ = obj
COMPILER = g++
FLAGS = -O2

RENDEZVOUS_SERVER_IP = 192.168.1.1:8080

run_rendezvous_server: prepare $(BIN)/server
	./$(BIN)/server $(RENDEZVOUS_SERVER_IP) &

run_first_client_server: prepare $(BIN)/client
	./$(BIN)/client $(RENDEZVOUS_SERVER_IP) &

run_first_client_server: prepare $(BIN)/client
	./$(BIN)/client $(RENDEZVOUS_SERVER_IP) &


$(BIN)/server: server/main.cpp server/server.cpp
	$(COMPILER) $(FLAGS) server/main.cpp server/server.cpp -o $(BIN)/server

$(BIN)/client: client/main.cpp server/server.cpp
	$(COMPILER) $(FLAGS) client/main.cpp server/server.cpp -o $(BIN)/client

clean:
	rm $(BIN)/*
	rm $(OBJ)/*

prepare:
	-mkdir $(BIN)
	-mkdir $(OBJ)
	