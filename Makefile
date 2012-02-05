all:
	g++ main.cpp timer.cpp keystrokes.cpp -o main `pkg-config opencv --cflags --libs` -lX11 -lXtst
