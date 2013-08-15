CXX = g++
CC = gcc
RM = del
CP = copy
CXXFLAGS = -Iinclude/borderless -Llib/borderless
CFLAGS = -Iinclude/borderless -Llib/borderless
LIBS = -lopenlayer -lglyph-agl -lfreetype.dll -lpng -lzdll -lagl -lalleg -luser32 -lgdi32 -lglu32 -lopengl32

.PHONY: all clean veryclean

all:  lathe.exe colors.exe

colors.exe : colors.o monitor.o window.o
	$(CXX) -ocolors.exe $(CXXFLAGS) colors.o monitor.o window.o $(LIBS)

colors.o : colors.cpp
	$(CXX) -c -ocolors.o $(CXXFLAGS) colors.cpp

lathe.exe : lathe.o monitor.o window.o
	$(CC) -olathe.exe $(CFLAGS) lathe.o monitor.o window.o $(LIBS)

lathe.o : lathe.c
	$(CXX) -c -olathe.o $(CXXFLAGS) lathe.c

monitor.o: monitor.c
	$(CC) -c -omonitor.o $(CFLAGS) monitor.c

window.o: window.c
	$(CC) -c -owindow.o $(CFLAGS) window.c

clean :
	$(RM) monitor.o
	$(RM) window.o
	$(RM) colors.o
	$(RM) lathe.o

veryclean : clean
	$(RM) colors.exe
	$(RM) lathe.exe

