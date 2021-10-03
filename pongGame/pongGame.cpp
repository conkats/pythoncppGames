#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>


class ball
{
// Declaring some public parameters
public:
    // Ball radius
    int radius;  
    // Position
    int x;
    int y;   
    // Speed in x and y
    int vx;
    int vy;
    // Creating a sfml circle object
    sf::CircleShape circle;    
    // Constructor
    ball(int r, sf::Color color):
    circle(r),
    x(100),
    y(100),
    vx(1), // ball speed x is set to 1 by default
    vy(1)  // ball speed y is set to 1 by default
    {
        std::cout << "constructing" << std::endl;
        radius = r;
        circle.setFillColor(color);
        circle.setPosition(x,y);
    }
    // Update the position of the ball
    void updatePos()
    {
        x = x + vx;
        y = y + vy;
        circle.setPosition(x,y);
    }
    // Update speed if ball goes beyong the playing area
    void updateSpeed(int frameWidth, int frameHeight)
    {
        // If ball position is out of the frame, the change speed sign
        if (x < 0 || x > frameWidth)
        {
            vx = vx*-1;
        }
        if (y < 0 || y > frameHeight)
        {
            vy = vy*-1;
        }
    }
};

int main()
{
    // Creating the window frame
    int frameWidth = 800;
    int frameHeight = 500;
    sf::VideoMode myVideo(frameWidth, frameHeight);
    sf::RenderWindow window(myVideo, "PongGame");
    
    // Create ball
    ball ball1(10,sf::Color::Green);

    // Starting while loop
    while (window.isOpen())
    {        
        // Starting event
        sf::Event event;

        // Check if there is an event
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            window.close();
        }

        window.clear();
        window.draw(ball1.circle);
        window.display();
        // Updating the position
        ball1.updatePos();
        // Updating the speeds
        ball1.updateSpeed(frameWidth,frameHeight);
    }
    return 0;
}