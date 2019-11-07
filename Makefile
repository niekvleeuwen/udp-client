# this makefile can be used to easily compile the project.

SRCS = main.c
CFLAGS = -Wall -g -Wimplicit-function-declaration
LIBS = . 
TARGET = udp-client

all: $(SRCS)
	@echo Installing the server...
	gcc $(CFLAGS) $(SRCS) -I $(LIBS)-o $(TARGET)

clean: 
	@echo Removing the executable
	rm $(TARGET)