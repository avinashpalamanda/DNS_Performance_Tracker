#Makefile for the DNS_Performance_Tracker#
CC=g++

CFLAGS= -w -I/usr/include/mysql -I/usr/include/mysql++
CFLAGS1= -c
LDFLAGS= -L/usr/local/lib -lldns -lmysqlpp -lmysqlclient -pthread

all:	DNS_DB_Handler

#Linking all the object file
DNS_DB_Handler:	helper_main.o helper.o
				$(CC) -o DNS_DB_Handler helper_main.o helper.o $(CFLAGS) $(LDFLAGS)

#main function object file
helper_main.o: helper_main.cpp helper.hpp
				$(CC) $(CFLAGS1) $(CFLAGS) $(LDFLAGS) helper_main.cpp

#Helper object filez
helper.o: helper.cpp helper.hpp
				$(CC) $(CFLAGS1) $(CFLAGS) $(LDFLAGS) helper.cpp

#DB Removing all files
clean:
				rm -rf *o DNS_DB_Handler
