CC = g++
INCDIR = -I/usr/local/include -L/usr/local/lib -L./lib -I./lib/include -I./include
LDFLAGS = -lpthread -lmongoclient -lboost_thread -lboost_filesystem -lboost_program_options -lboost_system

TARGET = monana

all:
	$(CC) src/main.cpp src/MongoSchema.cpp $(LDFLAGS) -o $(TARGET) $(INCDIR) 
clean:
	rm -rf *.o $(TARGET)
