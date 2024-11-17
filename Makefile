BIN = bin
OBJ = obj
COMPILER = g++
FLAGS = -O2

RENDEZVOUS_SERVER_ADDR = 192.168.1.1:8080

run_rendezvous_server: prepare $(BIN)/server
	./$(BIN)/server $(RENDEZVOUS_SERVER_ADDR) &

run_first_client_server: prepare $(BIN)/client
	./$(BIN)/client $(RENDEZVOUS_SERVER_ADDR) $(CLIENT_ADDR) $(CLIENT_ADDR) &

$(BIN)/server: prepare server/main.cpp server/server.cpp $(BIN)/utils.o
	$(COMPILER) $(FLAGS) server/main.cpp server/server.cpp $(BIN)/utils.o -o $(BIN)/server

$(BIN)/client: prepare client/main.cpp client/client.cpp $(BIN)/utils.o
	$(COMPILER) $(FLAGS) client/main.cpp client/client.cpp $(BIN)/utils.o -o $(BIN)/client

$(BIN)/utils.o: prepare utils/utils.cpp
	$(COMPILER) -c $(FLAGS) utils/utils.cpp -o $(BIN)/utils.o

clean:
	rm $(BIN)/*
	rm $(OBJ)/*

prepare:
	-mkdir $(BIN)
	-mkdir $(OBJ)
	