#include <SFML/Graphics.hpp>
#include<iostream>
#include<string>
#include <ctime>
#include <math.h>
#include <sstream>


class Player
{   public:
    
    Player(std::string imgDirectory)
    {
        if(!pTexture.loadFromFile(imgDirectory))
        {
        std::cerr << "Error loading avatar\n";
        }
        pSprite.setTexture(pTexture);
    }
    //draw texture and reference the window we want to reference/render
    void drawPlayer(sf::RenderWindow &window, int xp, int yp, float xscal, float yscal)
    {
        pSprite.setPosition(xp, yp);
        // scale the entity relatively to its current scale
        pSprite.setScale(xscal, yscal);
        window.draw(pSprite);
    }
    private:
    sf::Texture pTexture;
    sf::Sprite pSprite;
};

class paddle
{
public:
    //initialize the attributes of the paddle
    int height;
    int width;
    
    //declare the paddle as rectangular sfml object
    sf::RectangleShape rectangule;
    
    // Position of paddle
    float x;
    float y;
    float minY;
    float maxY;

    // Speed in y
    float vy;
    
    // Constructor of paddle class objects
    // NOTE: &framewidth passes a pointer to variable framewidth
    //       to avoid making a copy as a new variable saving memory     
    paddle(int &framewidth, int &frameheight, int which):
    // Initializing some common parameters
    height(100),
    width(20),
    maxY(frameheight),
    minY(0),
    rectangule(sf::Vector2f(width,height)),
    vy(10)
    {           
        // Set origin to the middle of the rectangle instead of its corner
        rectangule.setOrigin(sf::Vector2f(width/2, height/2));

        // Settings that change between players
        // Defining the paddle size for each player
        if (which==0)
        {
            // X coordinate
            x = 0.05*framewidth;

            // Color
            rectangule.setFillColor(sf::Color::Red);
        } 
        else if (which==1)
        {
            // X coordinate
            x = 0.95*framewidth;

            // Color
            rectangule.setFillColor(sf::Color::Blue);
        }

        // Setting that are common between players

        // Starting position
        y =  0.5*frameheight;

        // Setting initial position
        rectangule.setPosition(x,y);
    }

    // Method to move up the paddle
    // Note (0,0) is the top left corner of the frame window
    void moveUp()
    { 
        if( y > (minY+height/2) )
        {
            y=y-vy;
            //rectangule.setPosition(rectangule.getPosition().x,rectangule.getPosition().y-vy);
            rectangule.setPosition(x,y);
        }
    }

    // Method to move down the paddle
    void moveDown()
    { 
        if( y < (maxY-height/2))
        {
            y=y+vy;
            rectangule.setPosition(x,y);
        }
    }
};

class ball
{
public:

    // Radius of the ball
    int radius;

    // Creating the ball as a CircleShape named circle
    sf::CircleShape circle;

    // Positions x and y
    int x;
    int y;

    // Speed in x and y
    int vx;
    int vy;

    // Constructor and initialize the ball objects
    ball(int r, sf::Color color ):
    circle(r),
    x(250),
    y(450),
    vx(-1),
    vy(-1)
    {
        // Saving the ball radius used during construction
        radius = r;

        // Setting the color
        circle.setFillColor(color);

        // Setting the initial position
        circle.setPosition(x, y);
    }
    
    // Method update the ball position
    void updatePos(){
        x = x + vx;
        y = y + vy;
        circle.setPosition(x, y);
    }

    // Collision method changing velocity sign of the ball
    void  updateSpeed(int frameWidth, int frameHeight)
    {
        // If x position is outside the frame, change its sign
        if ( x < 0 || x > frameWidth )
        {
            vx = -vx;
        }

        // If y position is outside the frame, change its sign
        if ( y < 0 || y > frameHeight  )
        {
            vy = -vy;
        }
    } 

    //Method to detect collision with paddle
    //stored in array-vector of paddles
    void collision(std::vector<paddle> &paddles, int frameWidth, int frameHeight)
    {     
        for (int i = 0; i <=paddles.size(); i++)
        
        {     
        //This is essentially a collision method that should be within the ball class          
            if ((circle.getGlobalBounds().intersects(paddles[i].rectangule.getGlobalBounds())))           
            {
                vx = -vx;
            }
        }

        if ( y < 0 || y >frameHeight)
        {
            vy = -vy;
        }
    }
    // Method to restore the ball after collision and exceeding the boundary of the framewidth, paddle etc.
    void resetBallAfterCollision(sf::RenderWindow &window,std::vector<paddle> &paddles, int frameWidth,\
                                 int frameHeight , sf::Font &font, int score)
    {   
        score +=1;
        if (circle.getPosition().x > paddles[0].rectangule.getPosition().x + 2*circle.getRadius())
        {
            circle.setPosition(x = frameWidth+vx, frameHeight);
        }
        else if (circle.getPosition().x < paddles[1].rectangule.getPosition().x - 2*circle.getRadius())
        {
            circle.setPosition(x = frameWidth+vx, frameHeight);
        }
    }    


};

int main()
{
    // Setting game frame size
    int frameWidth = 800;
    int frameHeight = 500;

    // Loading font file and alert in case the font is not found
    sf::Font font;
    if(!font.loadFromFile("arial.ttf"))
    {
        std::cout << "can't load found" <<std::endl; 
    }

    // Creating text to display when pausing the game
    sf::Text pause;
    pause.setCharacterSize(34);
    pause.setFillColor(sf::Color::White);
    pause.setFont(font);
    pause.setString("Pause");

    sf::Text versus;
    versus.setCharacterSize(22);
    versus.setFillColor(sf::Color::White);
    versus.setFont(font);
    versus.setPosition(370,5);
    versus.setString("VS");
    
    // Setting players scores via text object
    int playerscore = 0;
    int player2score = 0;
    sf::Text labelScore;
    labelScore.setCharacterSize(22);
    labelScore.setFillColor(sf::Color::White);
    labelScore.setPosition(230,5);
    labelScore.setFont(font);

    std::ostringstream ssScore;
    labelScore.setString(ssScore.str());

    sf::Text label2Score;
    label2Score.setCharacterSize(22);
    label2Score.setFillColor(sf::Color::White);
    label2Score.setPosition(450,5);
    label2Score.setFont(font);

    std::ostringstream ss2Score;
    label2Score.setString(ss2Score.str());

    // Starts the clock
    // NOTE: DAA: I did not understand what clock does.
    sf::Clock clock;

    // Creating the ball
    ball ball1(10,sf::Color::Green);
    
    // Creating the paddles
    paddle paddleLeft(frameWidth,frameHeight,0);
    paddle paddleRight(frameWidth,frameHeight,1);

    //define an array-vector container class to store all object-paddles
    std::vector<paddle> paddles;

    //add paddles to array
    paddles.push_back(paddleRight);
    paddles.push_back(paddleLeft);


    //create player instance
    Player player1("p2.png");
    Player player2("p1.png");
    // NOTE: These methods should go within the ball class! 
    //sf::RectangleShape topBorder;
    //topBorder.setSize(sf::Vector2f(frameWidth,paddleLeft.x));
    //topBorder.setPosition(0,0);
 
    // NOTE: These methods should go within the ball class! 
    //sf::RectangleShape botBorder;
    //botBorder.setSize(sf::Vector2f(frameWidth,paddleLeft.x));
    //botBorder.setPosition(0,frameHeight-paddleLeft.x);


    // Creating the window to render the game on screen
    // NOTE: VideoMode is a class and myVideosize is the object!
    sf::VideoMode myVideosize(frameWidth, frameHeight);
    sf::RenderWindow window(myVideosize,"PongGame!");
    window.setFramerateLimit(200);
    // NOTE: These methods should go within the ball class! 
    //sf::Vector2f velocity;
    //float ballSpeed = 400.f;
    //float ballAngle=75.f;

    // Boolean variables to check if keys are pressed
    bool isWPressed     = false;
    bool isSPressed     = false;
    bool isOPressed     = false;
    bool isLPressed     = false;
    bool isGamePause    = false;

    // While loop
    while (window.isOpen())
    {   
        // DAA: I did not understand what clock does.
        // I think the idea is to have velocity changes based on actual time rather than frames.
        // This is exactly what we needed.
        // Still, we need to move these methods to their corresponding classes.
        //float deltaTime = clock.restart().asSeconds();
        //float factor = deltaTime * ballSpeed;

        // DAA: We need to create a player class and have all these methods inside the player objects.
        unsigned int life_count = 3;

        // Creating event to check in every iteration
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch(event.type)
            {
                // If window is closed, the break and finish the loop
                case sf::Event::Closed:
                    window.close();
                    break;    
                // Otherwise check what keys are pressed
                case sf::Event::KeyPressed:   
                    if(event.key.code == sf::Keyboard::W)  
                    {
                        isWPressed = true;
                    } 
                    if(event.key.code == sf::Keyboard::S)  
                    {
                        isSPressed = true;
                    } 
                    if(event.key.code == sf::Keyboard::O)  
                    {
                        isOPressed = true;
                    } 
                    if(event.key.code == sf::Keyboard::L)  
                    {
                        isLPressed = true;
                    } 
                    if(event.key.code == sf::Keyboard::Escape)
                    {
                        window.close();
                    }
                    if(event.key.code == sf::Keyboard::P)  
                    {
                        isGamePause = !isGamePause;                     
                    } 
                break;
                
                // or check what keys were released
                case sf::Event::KeyReleased: 
                    if(event.key.code == sf::Keyboard::W)  
                    {
                        isWPressed = false;
                    } 
                    if(event.key.code == sf::Keyboard::S)  
                    {
                        isSPressed = false;
                    } 
                    if(event.key.code == sf::Keyboard::O)  
                    {
                        isOPressed = false;
                    } 
                    if(event.key.code == sf::Keyboard::L)  
                    {
                        isLPressed = false;
                    } 
                break;
            }
            
        }
        
        // Updating paddles positions
        if (isWPressed){
            paddles[1].moveUp();
        }
        if (isSPressed){
            paddles[1].moveDown();
        }
        if (isOPressed){
            paddles[0].moveUp();
        }
        if (isLPressed){
            paddles[0].moveDown();
        }

        // Clearing the entire window
        window.clear();

        //Draw player sprites
        player1.drawPlayer(window,325,5, 0.05f,0.05f);
        window.draw(versus);
        window.draw(labelScore);
        window.draw(label2Score);

        player2.drawPlayer(window,405,5,0.09f,0.09f);
        // Drawing the ball
        window.draw(ball1.circle);


        // Drawing the paddles
        window.draw(paddles[0].rectangule);
        window.draw(paddles[1].rectangule);
        
           
        // Option 2 to move the ball 
        //velocity.x = std::cos(ballAngle)*factor;
        //velocity.y = std::sin(ballAngle)*factor;
        //ball1.circle.move(velocity.x,velocity.y);

        // DAA: This is essentially a collision method that should be within the ball class
        //if ((ball1.circle.getGlobalBounds().intersects(paddleLeft.rectangule.getGlobalBounds())) || (ball1.circle.getGlobalBounds().intersects(paddleRight.rectangule.getGlobalBounds())))           
        //{
        //    ball1.circle.move(-velocity.x,-velocity.y);
        //    velocity.x = -(velocity.y);
        //    ballAngle = -ballAngle;
        //    ballSpeed = -ballSpeed;               
        //}
        //// DAA: This is essentially a collision method that should be within the ball class
        //if ((ball1.circle.getGlobalBounds().intersects(topBorder.getGlobalBounds()))||(ball1.circle.getGlobalBounds().intersects(botBorder.getGlobalBounds())))
        //{   velocity.x = -(velocity.x);
        //    ballAngle = -ballAngle;
        //}
        
        ball1.updatePos();
        //ball1.collision(paddles, frameWidth, frameHeight);
        ball1.collision(paddles, frameWidth, frameHeight);

        if (ball1.circle.getPosition().x > paddles[0].rectangule.getPosition().x + 2*ball1.circle.getRadius())
        {   playerscore++;
            ssScore.str("");
            ssScore << "Score: " <<playerscore;
            labelScore.setString(ssScore.str());

            //std::cout <<"Player 1 scored:"<<playerscore << std::endl;
        }
        else if (ball1.circle.getPosition().x < paddles[1].rectangule.getPosition().x - 2*ball1.circle.getRadius())
        {
           player2score++;
           ss2Score.str("");
           ss2Score << "Score: " <<player2score;
           label2Score.setString(ss2Score.str());
        }
        //after collision calling the reset ball to restart the game  
        ball1.resetBallAfterCollision(window, paddles, frameWidth/2, frameHeight/2,font, playerscore);

        // CK: When scoring we need to create as suggested a player class to store score, text and dispaly it 
        //    and consider adding the pause/restart of the game as well as clock-timer
        // as DAA suggested
        //if (ball1.circle.getPosition().x > paddleRight.rectangule.getPosition().x + 2*ball1.radius)
        //{
        //    // DAA: We should use a player object to store its scoreboard
        //    player1score++;
        //    
        //    // DAA: We can create a reset method within the ball class to reset its position
        //    //ball1.circle.setPosition(frameHeight/2,frameWidth/2);
        //    // DAA: We have to add a beautiful scoreboard within the game to display the score
        //    //std::cout <<"Player 1 scored:"<<player1score << std::endl;
        //}
        //else if (ball1.circle.getPosition().x < paddleLeft.rectangule.getPosition().x - 2*ball1.radius)
        //{
        //    // DAA: We should use a player object to store its scoreboard
        //    player2score++;
        //    // DAA: We can create a reset method within the ball class to reset its position
        //    //ball1.circle.setPosition(frameHeight/2,frameWidth/2);
        //    // DAA: We have to add a beautiful scoreboard within the game to display the score
        //    //std::cout <<"Player 2 scored:"<<player1score << std::endl;
        //}
        // If pause, write pause on screen.
        // NOTE: You do not need to use boolean variables with equal sign.
        //       if ( booleanVariable == True ) { }
        //       is exactly the same as
        //       if ( booleanVariable ) {}
        //       The  "== True" is not needed
        //Pausing the Game
        if ( isGamePause ){
            pause.setPosition(350,80);
            ball1.circle.setPosition(380,150);
            window.draw(pause);
        }

        // Display window    
        window.display();
    }

    return 0;
}