#CXX = g++
INCDIR = -I/usr/local/include -L/usr/local/lib -L./mongo-cxx-driver-v2.4 -I./mongo-cxx-driver-v2.4/src -I./include
LDFLAGS = -lpthread -lmongoclient -lboost_thread -lboost_filesystem -lboost_program_options -lboost_system

TARGET = monana

all:
	$(CXX) src/main.cpp src/MongoSchema.cpp $(LDFLAGS) -o $(TARGET) $(INCDIR) 
clean:
	rm -rf *.o $(TARGET)
