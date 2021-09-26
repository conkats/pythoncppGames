g++ -c pongGame.cpp
g++ pongGame.o -o pongGame-app -lsfml-graphics -lsfml-window -lsfml-system
./pongGame-app