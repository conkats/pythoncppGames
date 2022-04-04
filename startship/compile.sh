g++ -c starship.cpp
g++ starship.o -o starship-app -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio 
./starship-app