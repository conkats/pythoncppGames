#include <SFML/Graphics.hpp>
#include<iostream>//lib for stdout
#include<string>
#include <ctime>
//ref:https://www.sfml-dev.org/tutorials/2.5/start-linux.php

//obsolete
//class paddle
//{
//public:
//    //initialize the attributes of the paddle
//    int height;
//    int width;
//    
//    //declare the paddle as rectangular sfml object
//    sf::RectangleShape rectangule;
//    
//    //Position of paddle
//    float x;
//    float y;
//    float minY;
//    float maxY;
//    //Speed in y
//    float vy;
//    
//    //Constructor of paddle type
//    //initialize
//    paddle(int framewidth, int frameheight, int which):
//    height(100),
//    width(20),
//    maxY(frameheight),
//    minY(0),
//    rectangule(sf::Vector2f(width,height)),
//    vy(0.5)
//    {   //rgb
//       sf::Color color(255,255,255);
//       rectangule.setFillColor(color);
//       if (which==0){
//          x = 0.05*framewidth-0.5*width; 
//       }else if (which==1)
//       {
//          x = 0.95*framewidth-0.5*width;
//       }
//       y =  0.5*frameheight-0.5*height;
//       rectangule.setPosition(x,y);
//    
//    }
//
//    //method to move up the paddle
//    void moveUp()
//    { 
//        if( y>minY )
//        {
//            y=y-vy;
//            rectangule.setPosition(x,y);
//        }
//    }
//    //method to move down the paddle
//    void moveDown()
//    { 
//        if( y< (maxY-height))
//        {
//            y=y+vy;
//            rectangule.setPosition(x,y);
//        }
//
//    }
//
//};

class paddle
{
public:
    //initialize the attributes of the paddle
    int height;
    int width;
    
    //declare the paddle as rectangular sfml object
    sf::RectangleShape rectangule;
    
    //Position of paddle
    float x;
    float y;
    float minY;
    float maxY;
    //Speed in y
    float vy;
    
    //Constructor of paddle type
    //initialize
    //&framewidth=reference pointer to avoid making a copy of the new variable
    //again in memory since only the value is needed
     
    paddle(int &framewidth, int &frameheight, int which):
    height(100),
    width(20),
    maxY(frameheight),
    minY(0),
    rectangule(sf::Vector2f(width,height)),
    vy(0.5)
    {   //rgb
        sf::Color color(255,255,255);
        rectangule.setFillColor(color);
        //use the local origin at the central coordinate of paddle
        //instead of the left corner vertex
        rectangule.setOrigin(sf::Vector2f(width/2,height/2)); 
        if (which==0){
           x = 0.05*framewidth; 
        }else if (which==1)
        {
           x = 0.95*framewidth;
        }
        y =  0.5*frameheight;
        rectangule.setPosition(x,y);
    }

    //method to move up the paddle
    //note 0,0 is the top left corner of the frame window
    void moveUp()
    { 
        if( y>  (minY+height/2) )
        {
            //y=y-vy;
            rectangule.setPosition(rectangule.getPosition().x,rectangule.getPosition().y-vy);
        }
    }
    //method to move down the paddle
    void moveDown()
    { 
        if( y< (maxY-height/2))
        {
            y=y+vy;
            rectangule.setPosition(x,y);
        }

    }

};



class ball
{
public:
    //initialize the attributes
    int radius;//int type is a class remember!
    sf::CircleShape circle;//create-declare a circle sfml object

    //Position
    int x;
    int y;

    //Speed in x and y
    int vx;
    int vy;
    
    //Special Function that builds the object
    //Constructor and initialize the ball anytime
    //does not need to define the type
    ball(int r, sf::Color color ):
    circle(r),
    x(100),
    y(100),
    vx(1),
    vy(1)

    {
        radius = r;
        circle.setFillColor(color);
        circle.setPosition(x, y);
        //std::cout<<"contructing from 1st constructor"<< std::endl;//standard out
    }
    //method=function to update position
    //of type void : does not return anything
    //just update the positions
//    void updatePos(){
//          x = x+1;
//          y = y+1;
//          circle.setPosition(x, y);
//    };
    //method=function to update the speed
        void updatePos(){
          x = x+vx;
          y = y+vy;
          circle.setPosition(x, y);
    };

    void moveUp()
    {
        y=y-10;
        circle.setPosition(x, y);
    }    
    void moveDown()
    {
        y=y+10;
        circle.setPosition(x, y);
    } 
     //change the velocity if the position is outside the frame 
     // to change the velocity sign
    void  updateSpeed(int frameWidth, int frameHeight) 
        {if (x< 0 || x>frameWidth){
           vx = vx*-1 ;
        }
        if (y<0 || y>frameHeight){
         vy = vy*-1 ;
          }
        }  

   
    //ball(int r) 
    //{
    //    std::cout<<"contructing from 1st constructor"<< std::endl;//standard out
    //    radius = r;
    //}
    //or
    //2nd contructor with default attributes to constructor arguments 
    //ball(int r):
    //radius(20)
    ////ballName("");
    //{   
    //  std::cout<<"contructing from 2nd constructor"<< std::endl;//standard out
    //}

    //3rd contructor with default attributes to constructor arguments 
    //ball()
    ////ballName("");
    //{   
    //  radius = 20;
    //  std::cout<<"contructing from 3rd constructor"<< std::endl;//standard out
    //}
};


int main()
{
    int frameWidth = 800;
    int frameHeight = 500;
    
    //define objects i.e. create the ball, paddles etc.
    ball ball1(10,sf::Color::Green);
    paddle paddleLeft(frameWidth,frameHeight,0);
    paddle paddleRight(frameWidth,frameHeight,1);
 
    sf::VideoMode myVideosize(frameWidth, frameHeight);// VideoMode is a class! and myVideosize is the object!
    sf::RenderWindow window(myVideosize,"PongGame!");
    //sf::CircleShape shape(20.f);
    
    //sf::RectangleShape rectangle(sf::Vector2f(10.f,80.f));
    //sf::RectangleShape rectangle2(sf::Vector2f(10.f,80.f));

    //shape.setFillColor(sf::Color::Red);
    
    //rectangle.setFillColor(sf::Color::White);
    //rectangle2.setFillColor(sf::Color::White);
    //sf::Time tdelay=sf::milliseconds(10);
    //define the time delay
    //sf::Clock clock;
     
    //set the position 
    //int psx = 0;
    //int psy = 0;
    //int speedx =1;
    //int speedy =1;
    //note that (0,0) is the top corner of the window

    //boolean to check if the key is pressed
    bool isWPressed = false;
    bool isSPressed = false;
    bool isOPressed = false;
    bool isLPressed = false;
    
    while (window.isOpen())
    {
        //psx=psx+speedx;
        //psy=psy+speedy;
        //shape.setPosition(psx, psy);
        //rectangle.setPosition(0.01,200);
        //rectangle2.setPosition(380,200);
        sf::Event event;
        //sf::Clock clock; //starts the clock

        //sf::Time elapsed= clock.getElapsedTime();    

        //Check if there is an event
        time_t beginT =time(NULL), endT = time(NULL);
        
        while (window.pollEvent(event))
        {
            if (difftime(endT, beginT)< 4.0f)
            {
              endT = time(NULL);
              //printf("Time:,%2.2e",endT);
              }

            switch(event.type)
            {
                case sf::Event::Closed://close the window
                    window.close();
                    break;
                case sf::Event::KeyPressed: 
                    //paddleRight=1             
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
                    break;
                case sf::Event::KeyReleased: 
                    //paddleRight=1             
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
            
        if (isWPressed){
            paddleLeft.moveUp();
        }
        if (isSPressed){
            paddleLeft.moveDown();
        }
        if (isOPressed){
            paddleRight.moveUp();
        }
        if (isLPressed){
            paddleRight.moveDown();
        }
        window.clear();
          //window.draw(shape);
          window.draw(ball1.circle);// draws the sfml circle object of ball1 of class ball-x,y,z,color
          window.draw(paddleLeft.rectangule);// draws the sfml rectangule object of paddle class
          window.draw(paddleRight.rectangule);// draws the sfml rectangule object of paddle class
//          shape.move(psx, psy);
//
//          using namespace sf;
//          
//           if (isKeyPressed(Keyboard::A)) {
//               psx = -2;
//           } else if (isKeyPressed(Keyboard::D) {
//               changeX = 2;
//           } else {
//               changeX = 0;
//           } if (isKeyPressed(Keyboard::W) {
//               psy = -2;
//           } else if (isKeyPressed(Keyboard::S)) {
//               psy = 2;
//           } else {
//               changeY = 0;
//           }

           //shape.move(changeX, changeY);
          //ball1.updatePos();
          //ball1.updateSpeed(frameWidth, frameHeight);
          
          
          //window.draw(rectangle);
          //window.draw(rectangle2);
          window.display();
          //if (psx>width || psx<0){
          //    speedx = speedx*-1 ;
          //}
          //if (psy>height || psy<0){
          // speedy = speedy*-1 ;
          //}

        }
        //
        // clock.restart();
       //}
              // Updating the speeds
        //std::cout <<elapsed.asSeconds() <<std::endl;

    return 0;
}