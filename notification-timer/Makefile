#
# Makefile for a execute
#
CC=g++
APP=notification-timer

INC=-I/usr/local/Cellar/poco/1.9.0_1/include
LIBS=-L/usr/local/opt/poco/lib/
INC_LIB = -lPocoFoundation
INC_LIB += -lPocoNet  -lPocoNetSSL -lPocoUtil -lPocoXML

all:
	$(CC) *.cpp -o $(APP) $(INC) $(LIBS) $(INC_LIB)

clean:
	rm $(APP)

exec:
	export DYLD_LIBRARY_PATH=/usr/local/Cellar/poco/1.9.0_1/lib/ && ./$(APP) 
