#Makefile for the DNS_Performance_Tracker#
CC=g++

CFLAGS= -w -std=c++11 -I/usr/include/mysql -I/usr/include/mysql++ -I../include/
CFLAGS1= -c
LDFLAGS= -L/usr/local/lib -lldns -lmysqlpp -lmysqlclient -pthread
INCLUDES = include
SRC = src
OBJ = ofile
all:	DNS_Performance_Tracker

#Linking all the object file
DNS_Performance_Tracker:	main.o controller_query.o controller_DNS_resolver.o model_latency_db.o
				$(CC) -o DNS_Performance_Tracker main.o controller_query.o controller_DNS_resolver.o model_latency_db.o $(CFLAGS) $(LDFLAGS)

#main function object file
main.o: $(SRC)/main.cpp
				$(CC) $(CFLAGS1) $(CFLAGS) $(LDFLAGS) -I $(INCLUDES) -o $@ $^

#Controller Query object file
controller_query.o: $(SRC)/controller_query.cpp
				$(CC) $(CFLAGS1) $(CFLAGS) $(LDFLAGS) -I $(INCLUDES) -o $@ $^

#DNS resolver object file
controller_DNS_resolver.o: $(SRC)/controller_DNS_resolver.cpp
				$(CC) $(CFLAGS1) $(CFLAGS) $(LDFLAGS) -I $(INCLUDES) -o $@ $^

#DB model object file
model_latency_db.o: $(SRC)/model_latency_db.cpp
				$(CC) $(CFLAGS1) $(CFLAGS) $(LDFLAGS) -I $(INCLUDES) -o $@ $^

#DB Removing all files
clean:
				rm -rf *o DNS_Performance_Tracker
