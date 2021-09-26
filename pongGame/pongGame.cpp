#include <SFML/Graphics.hpp>

int main()
{
    // Creating the window frame
    int frameWidth = 400;
    int frameHeight = 500;
    sf::VideoMode myVideo(frameWidth, frameHeight);
    sf::RenderWindow window(myVideo, "PongGame");

    // Creating a red circle
    sf::CircleShape loopy(10.f);
    loopy.setFillColor(sf::Color::Red);

    // Setting initial position
    int psx = 25;
    int psy = 0;

    // Setting speeds in x and y direction (velocity components)
    int speedx = 1;
    int speedy = 1;

    // Starting while loop
    while (window.isOpen())
    {
        // Updating the position
        psx = psx + speedx;
        psy = psy + speedy;
        loopy.setPosition(psx,psy);

        // Starting event (We need to understand what is this for)
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        window.draw(loopy);
        window.display();


        // Updating the speeds if the circle goes beyond the window frame
        if (psx > frameWidth || psx < 0){
            speedx = speedx*-1;
        }
        if (psy > frameHeight || psy < 0){
            speedy = speedy*-1;
        }
    }

    return 0;
}