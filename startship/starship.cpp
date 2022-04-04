#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <math.h>
#include <sstream>


class trace{
public:
    // Radius of trace shape
    float r;

    // trace shape
    sf::CircleShape traceShape;

    // life time
    int lifeTime;

    // alpha
    int alpha;

    // Constructor
    trace(float x, float y): 
    r(1.0),
    lifeTime(0),
    alpha(10)
    {
        sf::Color traceColor(240,180,100,alpha);
        traceShape.setFillColor(traceColor);
        traceShape.setRadius(r);
        sf::FloatRect rect = traceShape.getLocalBounds();
        traceShape.setOrigin(rect.left + rect.width/2.0f,
                              rect.top  + rect.height/2.0f);
        traceShape.setPosition(sf::Vector2f(x,y));

    }

};


class explosion{
public:
    // Position
    float x;
    float y;

    // Action radius
    float actionRadius;

    // Circle shape
    sf::CircleShape zone1;
    sf::CircleShape zone2;
    sf::CircleShape zone3;

    // Color
    sf::Color color1;
    sf::Color color2;
    sf::Color color3;

    // Texture and sprite
    //sf::Texture explosionTexture;
    sf::Sprite explosionSprite;

    // explosion level
    int level;

    // duration
    int duration;

    // Constructor
    explosion(sf::Vector2f pos, int lvl, sf::Texture &explosionTexture): 
    level(lvl),
    duration(0)
    {
        // Setting texture to sprite
        explosionSprite.setTexture(explosionTexture);
        explosionSprite.setScale(0.03,0.03);
        explosionSprite.setOrigin(explosionSprite.getLocalBounds().left + explosionSprite.getLocalBounds().width/2.0f,
                                  explosionSprite.getLocalBounds().top  + explosionSprite.getLocalBounds().height/2.0f);
        explosionSprite.setPosition(sf::Vector2f(pos.x,pos.y)); 
        explosionSprite.setColor(sf::Color(255,255,255,150));

        actionRadius = 20.0;
        // Set colors
        color1.r=240;
        color1.g=220;
        color1.b=220;
        color1.a=90;
        
        color2.r=240;
        color2.g=200;
        color2.b=170;
        color2.a=50;

        color3.r=240;
        color3.g=180;
        color3.b=100;
        color3.a=60;

        // Set size
        zone1.setRadius(actionRadius*lvl);
        zone2.setRadius(actionRadius*lvl*2);
        zone3.setRadius(actionRadius*lvl*3);

        // Set fill colors
        zone1.setFillColor(color1);
        zone2.setFillColor(color2);
        zone3.setFillColor(color3);

        // Reset origin
        zone1.setOrigin(zone1.getLocalBounds().left + zone1.getLocalBounds().width/2.0f,
                        zone1.getLocalBounds().top  + zone1.getLocalBounds().height/2.0f);
        zone2.setOrigin(zone2.getLocalBounds().left + zone2.getLocalBounds().width/2.0f,
                        zone2.getLocalBounds().top  + zone2.getLocalBounds().height/2.0f);
        zone3.setOrigin(zone3.getLocalBounds().left + zone3.getLocalBounds().width/2.0f,
                        zone3.getLocalBounds().top  + zone3.getLocalBounds().height/2.0f);

        // Set position
        zone1.setPosition(sf::Vector2f(pos.x,pos.y));        
        zone2.setPosition(sf::Vector2f(pos.x,pos.y));        
        zone3.setPosition(sf::Vector2f(pos.x,pos.y));        
    }

    // grow explosion until end of life
    bool explote(){
        if (duration < 20){
            actionRadius = actionRadius+3;
            zone1.setRadius(actionRadius*level);
            zone2.setRadius(actionRadius*level*2);
            zone3.setRadius(actionRadius*actionRadius*level*3);
            // Reset origin
            zone1.setOrigin(zone1.getLocalBounds().left + zone1.getLocalBounds().width/2.0f,
                            zone1.getLocalBounds().top  + zone1.getLocalBounds().height/2.0f);
            zone2.setOrigin(zone2.getLocalBounds().left + zone2.getLocalBounds().width/2.0f,
                            zone2.getLocalBounds().top  + zone2.getLocalBounds().height/2.0f);
            zone3.setOrigin(zone3.getLocalBounds().left + zone3.getLocalBounds().width/2.0f,
                            zone3.getLocalBounds().top  + zone3.getLocalBounds().height/2.0f);

            explosionSprite.setScale(sf::Vector2f(duration/40.0f,duration/40.0f));
            explosionSprite.setOrigin(explosionSprite.getLocalBounds().left + explosionSprite.getLocalBounds().width/2.0f,
                                  explosionSprite.getLocalBounds().top  + explosionSprite.getLocalBounds().height/2.0f);                
            duration++;
            return false;
        }else{
            //std::cout << "returning true" << std::endl;
            //std::cout << "duration : " << duration << std::endl;
            duration = 0;
            return true;
        } 
    }
};


class missile{
public:
    // Position
    float x;
    float y;

    // relative position
    float xrel;
    float yrel;

    // origin position
    float xo;
    float yo;

    // local time for parametric movement
    float time;

    // missile
    sf::Sprite shellSprite;

    // trace
    std::vector<trace> traceVec;
    int traceCounter;

    // Constructor
    missile(sf::Vector2f pos,sf::Texture &shellTexture): 
    traceCounter(0),
    time(0.0)
    {
        xo = pos.x;
        yo = pos.y;
        x = xo;
        y = yo;
        shellSprite.setTexture(shellTexture);
        shellSprite.setPosition(xo,yo);
        shellSprite.setScale(0.6,0.6);
        sf::FloatRect rect = shellSprite.getLocalBounds();
        shellSprite.setOrigin(rect.left + rect.width/2.0f,
                              rect.top  + rect.height/2.0f);
    }

    void move(){
        if (time<15){

            if (time>1 && rand()%100<70){
                trace newTrace(x,y);
                traceVec.push_back(newTrace);
                //traceCounter++;
                for (int i =0; i < traceVec.size(); i++){
                    if (traceVec[i].lifeTime<30){
                        //traceVec[i].alpha = 20*sin((3.1415*traceVec[i].lifeTime/80));
                        traceVec[i].alpha = -0.5*traceVec[i].lifeTime + 30;
                        traceVec[i].r = 2*sin((3.1415*(traceVec[i].lifeTime+10)/40))+2;
                        sf::FloatRect rect = traceVec[i].traceShape.getLocalBounds();
                        traceVec[i].traceShape.setOrigin(rect.left + rect.width/2.0f,
                                  rect.top  + rect.height/2.0f);
                        traceVec[i].traceShape.setRadius(traceVec[i].r);
                        sf::Color newColor(255,100+traceVec[i].lifeTime*3,50+traceVec[i].lifeTime*3,traceVec[i].alpha);
                        traceVec[i].traceShape.setFillColor(newColor);
                        traceVec[i].lifeTime++;
                    }else{
                        traceVec.erase(traceVec.begin()+i);
                    }
                }
            }
            yrel = time*time;
            y = yo - yrel*30;
            //std::cout << y << std::endl;
            shellSprite.setPosition(sf::Vector2f(x,y));
        }

        time =  time + 0.015;
    }
};




class star{
public:
    // Position
    sf::Vector2f pos;

    // Depth
    float depth;

    // Radius
    float r;
    float scale;
    float increment; // scale increment dynamic scale

    // Speed (No speed in x direction)
    float vy;

    // Color
    sf::Color starColor;

    // Star
    sf::CircleShape starShape;
    sf::Sprite starSprite;
    sf::Vector2f spriteScale;


    // Constructor
    star
    (
        int &fwidth, 
        int &fheight, 
        bool fromTop, 
        sf::Texture &starTexture
    ):
    spriteScale(sf::Vector2f(0.8,0.8))
    {
        
        // set position pos.x, pos.y and depth
        setPosition(fwidth,fheight,fromTop);
        // set speed depending on the depth
        setSpeed(depth);
        //
        setRadius();

        // initializing dynamic size
        scale = 1;
        increment = 0.02;

        // Defining star color
        starColor.a = rand() % 100 + 155;
        starColor.r = rand() % 80 + 175;
        starColor.b = rand() % 80 + 175;
        starColor.g = rand() % 80 + 175;
   

        // Setting star
        starShape.setRadius(r);
        starShape.setPosition(pos);
        starShape.setFillColor(starColor);
        starShape.setOrigin(starShape.getLocalBounds().left +
                            starShape.getLocalBounds().width/2.0f,
                            starShape.getLocalBounds().top + 
                            starShape.getLocalBounds().height/2.0f);

         // set texture
        starSprite.setTexture(starTexture);
        starSprite.setScale(spriteScale);
        starSprite.setPosition(pos);
        starColor.a = starColor.a*0.3;
        starSprite.setColor(starColor);
        starSprite.setOrigin(starSprite.getLocalBounds().left +
                             starSprite.getLocalBounds().width/2.0f,
                             starSprite.getLocalBounds().top + 
                             starSprite.getLocalBounds().height/2.0f);
    }

    // set position
    void setPosition(int &fwidth, int &fheight, bool &fromTop){
        // Set random position
        pos.x = rand() % fwidth + 0;
        if (fromTop){
            pos.y=0;
        }else{
            pos.y = rand() % fheight + 0;
        }
        // set random depth
        depth = rand() % 400 + 0;
    }

    // Set radius
    void setRadius(){
        r = 0.5+(rand() % 150 + 0)/250.0f;
    }
    
    // Set speed
    void setSpeed(float &depth){
        vy = (depth+100)/2000;
    }

    // Set Scale
    void setScale(){
        // scaling is used for dynamic size of stars
        if (rand()%100<40){
            if (scale > 1.5){
                increment = - (rand()%5+5)/100.0f;
            }
            else if (scale < 0.7){
                increment = (rand()%5+5)/100.0f;
            }
        }
        scale = scale + increment;
        spriteScale.x = spriteScale.x + increment;
        spriteScale.y = spriteScale.y + increment;
    }

    // Move star
    void moveStar(){
        pos.y = pos.y + vy;
        starShape.setPosition(pos);
        starSprite.setPosition(pos);
        if (rand()%100<10){
            setScale();
            starShape.setScale(scale,scale);
            starSprite.setScale(spriteScale);
        }
    }
};



class bullet
{
public:
    // Position
    float x;
    float y;

    // Speed (No speed in x direction)
    float vy;
    
    // bullet rendering as a rectangle
    sf::RectangleShape bulletShape;
    
    // Constructor
    bullet(float x, float y, sf::Color &color, float speed):
    x(x),
    y(y)
    {
        bulletShape.setSize(sf::Vector2f(1.2, 15));
        bulletShape.setFillColor(sf::Color::White);
        bulletShape.setOutlineColor(color);
        bulletShape.setOutlineThickness(0.6);
        bulletShape.setPosition(sf::Vector2f(x,y));
        setBulletSpeed(speed);
    }
    
    // Update position
    void setBulletSpeed(int speed){
        vy = speed;
    }

    
    void updatePos()
    {
        y = y - vy;
        bulletShape.setPosition(sf::Vector2f(x,y));
    } 

    // Access function
    int getYPos(){
        return y;
    }
};



class ship
{
public:
    //Position
    float x;
    float y;

    //Speed
    float vx;
    float vy;

    // Health
    int health;
    bool live;

    // shooting-lvl
    int shootLevel;

    //Bullet color
    sf::Color bulletColor;

    // Declaring a texture to render the ship image
    sf::Texture shipTexture;

    // Declaring an sprite to render the ship image 
    sf::Sprite shipSprite;

    // Default Constructor
    ship(){};

    // Reset position
    void resetPos(int fwidth, int fheight){
        x = fwidth/2.0f - 30;
        y = fheight-100;
        shipSprite.setPosition(sf::Vector2f(x,y));
    }

    // Access functions
    int getXPos(){
        return x+28;
    }
    int getYPos(){
        return y+28;
    }

    // reset origin
    void resetOrigin(){
        sf::FloatRect rect = shipSprite.getLocalBounds();
        shipSprite.setOrigin(rect.left + rect.width/2.0f,
                             rect.top + rect.height/2.0f);
    }

    void setBulletColor(sf::Color color){
        bulletColor = color;
    }

    
};






class playerShip:
public ship
{
public:
    // Constructor
    playerShip(int &fwidth, int &fheight, sf::Texture &playerShipTexture)
    {
        shipSprite.setTexture(playerShipTexture);
        shipSprite.setScale(sf::Vector2f(0.8,0.8));
        resetPos(fwidth,fheight);
        resetOrigin();
        setBulletColor(sf::Color::Cyan);
        health = 20;
        live=true;
        vx=1.7;
        shootLevel=1;
    }
    // Moving functions
    void moveLeft(int &fwidth)
    {   
        if (shipSprite.getPosition().x > 0.05*fwidth){
            shipSprite.move(sf::Vector2f(-vx,0));
        }
    }
    void moveRight(int &fwidth)
    {
        if (shipSprite.getPosition().x < 0.95*fwidth){
            shipSprite.move(sf::Vector2f(vx,0));
        }
    }
    // Fire bullet
    void fireBullet(std::vector<bullet> &bulletVec){
        sf::Vector2f pos = shipSprite.getPosition();
        if (shootLevel==1){
            bullet newBullet(pos.x , pos.y-50,bulletColor,2);
            bulletVec.push_back(newBullet);
        } else if (shootLevel ==2){
            bullet newBullet1(pos.x + 10, pos.y - 50,bulletColor,2);
            bullet newBullet2(pos.x - 10, pos.y - 50,bulletColor,2);
            bulletVec.push_back(newBullet1);
            bulletVec.push_back(newBullet2);
        } else if (shootLevel ==3){
            bullet newBullet1(pos.x + 15, pos.y - 50,bulletColor,2);
            bullet newBullet2(pos.x     , pos.y - 53,bulletColor,2);
            bullet newBullet3(pos.x - 15, pos.y - 50,bulletColor,2);
            bulletVec.push_back(newBullet1);
            bulletVec.push_back(newBullet2);
            bulletVec.push_back(newBullet3);
        } else if (shootLevel ==4){
            bullet newBullet1(pos.x + 20, pos.y - 50,bulletColor,2);
            bullet newBullet2(pos.x + 7 , pos.y - 53,bulletColor,2);
            bullet newBullet3(pos.x - 7 , pos.y - 53,bulletColor,2);
            bullet newBullet4(pos.x - 20, pos.y - 50,bulletColor,2);
            bulletVec.push_back(newBullet1);
            bulletVec.push_back(newBullet2);
            bulletVec.push_back(newBullet3);
            bulletVec.push_back(newBullet4);
        }
    }

    bool updateHealth
    (
        std::vector<bullet> &bulletVector
    )
    {
        bool hit = false;
        for (int i = 0; i < bulletVector.size(); i++){
            if (health>0)
            {
                if (shipSprite.getGlobalBounds().intersects(bulletVector[i].bulletShape.getGlobalBounds())){
                health = health - 1;
                // If a bullet hits a ship the bullet should be deleted
                bulletVector.erase(bulletVector.begin()+i);
                hit = true;
                }  
            } else {
                live = false;
            }
        }
        // if damage received
        return hit;
    }

};



class alienShip:
public ship
{
public:
    int counter;
    bool moveForward;
    int lineShift;


    // Constructor
    alienShip(int &fwidth, int &fheight, sf::Texture &alienShipTexture):
    counter(0),
    moveForward(true)
    {
        shipSprite.setTexture(alienShipTexture);
        shipSprite.setScale(sf::Vector2f(0.8,0.8));
        shipSprite.setOrigin(shipSprite.getLocalBounds().left + shipSprite.getLocalBounds().width/2.0f,
                        shipSprite.getLocalBounds().top  + shipSprite.getLocalBounds().height/2.0f);
        setPos(fwidth,fheight);
        if (rand()%100<50){
            setSpeed(1,1);
            lineShift = 0;
        }
        else{
            setSpeed(-1,1);
            lineShift = 50;
        }
        shipSprite.setPosition(x,y);
        setBulletColor(sf::Color::Red);
        health = 6;
        live=true;
    }


    // Setting functions
    void setPos(int &fwidth, int &fheight){
        int xrange = fwidth - 200;
        int yrange = fheight - 400;
        x = rand() % xrange + 100;
        y = -80;
    }

    // Moving functions
    void move(int &fwidth, int &fheight)
    {   
        updateSpeed(fwidth,fheight);
        x = x+vx;
        y = y+vy;
        shipSprite.setPosition(x,y);
    }

    void updateSpeed(int &fwidth, int &fheight){
        if (getXPos()<50 || getXPos()>(fwidth-50)){
            vx=vx*-1;
            counter++;
        }

        int xrange = fwidth - 200;
        int randomx = rand() % xrange + 100;
        if (getXPos()==randomx){
                    vx = vx*-1;
        }

        if (moveForward){
            vy=1;
        }
        else{
            vy=0;
        }

        int nBounces = 2;
        if (getYPos()<fheight/2 && getYPos() > 0){
            if ((getYPos()+lineShift)%100==0){
                moveForward = !moveForward;
            }
            if (counter%nBounces==0){
                moveForward = !moveForward;
            }
        }
        else{
            vy=1;
        }
    }

    // Fire bullet
    bool fireBullet(std::vector<bullet> &bulletVec){
        if (rand()%100 <1){
            if (rand()%100 <15){
            //bulletShift = bulletShift * -1;
            bullet newBullet(getXPos(), getYPos(), bulletColor,-2);
            bulletVec.push_back(newBullet);
            return true;
            }
        }
        return false;
    }

    int updateHealth
    (
        std::vector<bullet> &bulletVector, 
        std::vector<explosion> &explosionVec,
        std::vector<missile> &missileVec,
        sf::Texture &explosionTexture
    )
    {
        int hitType = 0;
        for (int i = 0; i < bulletVector.size(); i++){
            if (health>0)
            {
                if (shipSprite.getGlobalBounds().intersects(bulletVector[i].bulletShape.getGlobalBounds())){
                health = health - 1;
                // If a bullet hits a ship the bullet should be deleted
                bulletVector.erase(bulletVector.begin()+i);
                hitType = 1;
                }  
            } else {
                live = false;
            }
        }

        for (int i = 0; i < missileVec.size(); i++){
            if (shipSprite.getGlobalBounds().intersects(missileVec[i].shellSprite.getGlobalBounds())){
                explosion newExplosion(shipSprite.getPosition(),1, explosionTexture);
                explosionVec.push_back(newExplosion);
                live = false;
                health = 0;
                // If a missile hits a ship it should be deleted
                missileVec.erase(missileVec.begin()+i);
                hitType = 2;
            }    
        }

        // if damage received
        return hitType;
    }


    // Settings
    void setSpeed(int setvx, int setvy){
        vx=setvx;
        vy=setvy;
    }
};





int main()
{
    // Creating the window frame
    int frameWidth = 1400;
    int frameHeight = 800;
    sf::VideoMode myVideo(frameWidth, frameHeight);
    sf::RenderWindow window(myVideo, "starship");

    window.setFramerateLimit(240);
    // kills counter
    int kills(0);
    int shootLevel(1);

    // game over screen fade counter
    int countFade(0);
    sf::Color gameOverFadeColor(200,0,0,1);
    sf::RectangleShape fadeScreen(sf::Vector2f(frameWidth-2,frameHeight-2));
    fadeScreen.setPosition(1,1);
    fadeScreen.setOutlineColor(sf::Color::Red);
    fadeScreen.setFillColor(gameOverFadeColor);
    fadeScreen.setOutlineThickness(1);




    //Loading images of ships as textures --------------------------------------

    // Loading player ship image as texture
    sf::Texture playerShipTexture;
    if(!playerShipTexture.loadFromFile("blueship-2.png"))
    {
        std::cout << "Loading playerShipTexture Failed!" << std::endl;
        system("pause");
    }

    // Loading alien ship image as texture
    sf::Texture alienShipTexture;
    if(!alienShipTexture.loadFromFile("alienship.png"))
    {
        std::cout << "Loading alienShipTexture Failed!" << std::endl;
        system("pause");
    }

    // Loading player ship image as texture
    sf::Texture missileTexture;
    if(!missileTexture.loadFromFile("missile.png"))
    {
        std::cout << "Loading playerShipTexture Failed!" << std::endl;
        system("pause");
    }

    // Loading explosion texture
    sf::Texture explosionTexture;
    if(!explosionTexture.loadFromFile("explosion.png"))
    {
        std::cout << "Loading explosionTexture Failed!" << std::endl;
        system("pause");
    }

    // Loading star texture
    sf::Texture starTexture;
    if(!starTexture.loadFromFile("star.png"))
    {
        std::cout << "Loading starTexture Failed!" << std::endl;
        return -1;
    }



    // Loading music -----------------------------------------------------------
    sf::Music music;
    if (!music.openFromFile("soundEffects/boss-battle-loop-3.wav")){
        std::cout << "Loading music Failed!" << std::endl;
        return -1;
    }
    music.setVolume(7);
    music.setLoop(true);
    music.play();


    // Loading sound effects ---------------------------------------------------

    // - player shooting
    sf::SoundBuffer playerShootBuffer;
    if (!playerShootBuffer.loadFromFile("soundEffects/retro-laser-shot-03.wav"))
    {
        std::cout << "Loading soundEffect playerShootBuffer Failed!" << std::endl;
        return -1;
    }
    sf::Sound playerShootSound;
    playerShootSound.setBuffer(playerShootBuffer);
    playerShootSound.setVolume(10);

    // - player missile
    sf::SoundBuffer playerMissileBuffer;
    if (!playerMissileBuffer.loadFromFile("soundEffects/alarm-short-b.wav"))
    {
        std::cout << "Loading soundEffect playerMissileBuffer Failed!" << std::endl;
        return -1;
    }
    sf::Sound playerMissileSound;
    playerMissileSound.setBuffer(playerMissileBuffer);
    playerMissileSound.setVolume(10);

    // - missile explosion sound
    sf::SoundBuffer playerMissileExploteBuffer;
    if (!playerMissileExploteBuffer.loadFromFile("soundEffects/space-blast-wave-picture.wav"))
    {
        std::cout << "Loading soundEffect playerMissileExploteBuffer Failed!" << std::endl;
        return -1;
    }
    sf::Sound explosionExploteSound;
    explosionExploteSound.setBuffer(playerMissileExploteBuffer);
    explosionExploteSound.setVolume(20);

    // - player exploting
    sf::SoundBuffer playerShipExploteBuffer;
    if (!playerShipExploteBuffer.loadFromFile("soundEffects/blast.ogg"))
    {
        std::cout << "Loading soundEffect playerShipExploteBuffer Failed!" << std::endl;
        return -1;
    }
    sf::Sound  playerShipExplote;
    playerShipExplote.setBuffer( playerShipExploteBuffer);
    playerShipExplote.setVolume(30);
    playerShipExplote.setPitch(0.5);


    // - player receiving damage
    sf::SoundBuffer playerShipDamageBuffer;
    if (!playerShipDamageBuffer.loadFromFile("soundEffects/impact-010.wav"))
    {
        std::cout << "Loading soundEffect playerShipDamageBuffer Failed!" << std::endl;
        return -1;
    }
    sf::Sound  playerShipDamage;
    playerShipDamage.setBuffer( playerShipDamageBuffer);
    playerShipDamage.setVolume(80);
    playerShipDamage.setPitch(0.5);


    // - player severe damage
    int criticalDamagePlayedTimes = 0;
    sf::SoundBuffer playerShipSevereDamageBuffer;
    if (!playerShipSevereDamageBuffer.loadFromFile("soundEffects/robot-severe-damage.wav"))
    {
        std::cout << "Loading soundEffect playerShipDamageBuffer Failed!" << std::endl;
        return -1;
    }
    sf::Sound  playerShipSevereDamage;
    playerShipSevereDamage.setBuffer( playerShipSevereDamageBuffer);
    playerShipSevereDamage.setVolume(30);
    playerShipSevereDamage.setPitch(1);

    // - alien shooting
    sf::SoundBuffer alienShipShootBuffer;
    if (!alienShipShootBuffer.loadFromFile("soundEffects/retro-video-game-sfx-laser.wav"))
    {
        std::cout << "Loading soundEffect alienShipShootBuffer Failed!" << std::endl;
        return -1;
    }
    sf::Sound  alienShipShoot;
    alienShipShoot.setBuffer( alienShipShootBuffer);
    alienShipShoot.setPitch(0.7);
    alienShipShoot.setVolume(10);

    // - alien exploting
    sf::SoundBuffer alienShipExploteBuffer;
    if (!alienShipExploteBuffer.loadFromFile("soundEffects/blast.ogg"))
    {
        std::cout << "Loading soundEffect alienShipExploteBuffer Failed!" << std::endl;
        return -1;
    }
    sf::Sound  alienShipExplote;
    alienShipExplote.setBuffer( alienShipExploteBuffer);
    alienShipExplote.setVolume(20);

    // Adding text -------------------------------------------------------------
    
    // Loading font for text rendering
    sf::Font font;
    if (!font.loadFromFile("Courier_Prime/CourierPrime-Regular.ttf"))
    {
        std::cout << "Loading font  Courier_Prime/CourierPrime-Regular.ttf Failed!" << std::endl;
        system("pause");
    }

    // Adding player name text
    sf::Text playerName;
    playerName.setFont(font);
    playerName.setString("Player 1");
    playerName.setColor(sf::Color::Green);
    playerName.setCharacterSize(28);
    playerName.setPosition(10,frameHeight-42);

    // Adding kills: text title
    sf::Text killTextTitle;        
    killTextTitle.setString("Kills : ");
    killTextTitle.setFont(font);
    killTextTitle.setColor(sf::Color::Yellow);
    killTextTitle.setCharacterSize(28);
    killTextTitle.setPosition(30,20);

    // Healt rectangle ---------------------------------------------------------

    // Adding health rectangle
    sf::RectangleShape healthFrame(sf::Vector2f(frameWidth*0.5,15));
    healthFrame.setPosition(0.5*frameWidth-0.25*frameWidth,frameHeight-30);
    healthFrame.setOutlineColor(sf::Color::Green);
    healthFrame.setFillColor(sf::Color::Transparent);
    healthFrame.setOutlineThickness(1);

    // Adding the actual remaining health defined in while loop
    sf::RectangleShape healthRect(sf::Vector2f(frameWidth*0.5,15));
    healthRect.setPosition(0.5*frameWidth-0.25*frameWidth,frameHeight-30);
    healthRect.setFillColor(sf::Color::Green);


    // Creating game elements -------------------------------------------------
    playerShip playerShip(frameWidth,frameHeight,playerShipTexture);
    std::vector<alienShip> alienShipVec;
    std::vector<bullet> alienBullets;
    std::vector<bullet> playerBullets;
    std::vector<missile> missileVec;
    std::vector<explosion> explosionVec;
    
    bool rightKeyState = false;
    bool leftKeyState = false;

    // Creating stars in the background 
    std::vector<star> starVec;
    for (int i = 0; i < 30; i++){
        star newStar(frameWidth,frameHeight, false, starTexture);
        starVec.push_back(newStar);
    }


    

    

    // Starting while loop
    while (window.isOpen())
    {   
        // Starting event
        sf::Event event;

        // Loop is breaking into two section, player is alive or death
        if (playerShip.health>0){
            // Check if there is an event
            while (window.pollEvent(event))
            {
                switch(event.type)
                {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    if(event.key.code == sf::Keyboard::Left)
                    {
                        leftKeyState = true;
                    }
                    if(event.key.code == sf::Keyboard::Right)
                    {
                        rightKeyState = true;
                    }
                    if(event.key.code == sf::Keyboard::N)
                    {
                        playerShip.fireBullet(playerBullets);
                        playerShootSound.play();
                    }
                    if(event.key.code == sf::Keyboard::M)
                    {
                        missile newMissile( playerShip.shipSprite.getPosition(), missileTexture);
                        missileVec.push_back(newMissile);
                        playerMissileSound.play();
                    }
                    break;
                case sf::Event::KeyReleased:
                    if(event.key.code == sf::Keyboard::Left)
                    {
                        leftKeyState = false;
                    }
                    if(event.key.code == sf::Keyboard::Right)
                    {
                        rightKeyState = false;
                    }
                    break;
                }
            }

            // First things first
            window.clear(sf::Color::Transparent);


            // Adding new stars from the top
            if (rand()%100<2){
                if (rand()%100<50){
                    star newStar(frameWidth,frameHeight, true, starTexture);
                    starVec.push_back(newStar);
                }
            }

            // Drawing stars
            for (int i = 0; i < starVec.size() ; i++){
                window.draw(starVec[i].starShape);
                window.draw(starVec[i].starSprite);
                starVec[i].moveStar();
                if (starVec[i].starShape.getPosition().y>frameHeight){
                    starVec.erase(starVec.begin()+i);
                }

            }


            // Drawing and moving missiles
            for (int i = 0; i < missileVec.size(); i++){
                window.draw(missileVec[i].shellSprite);
                for (int j=0; j <missileVec[i].traceVec.size(); j++ ) {
                    window.draw(missileVec[i].traceVec[j].traceShape);
                }
                missileVec[i].move();
            }

            // Deleting missiles out of screen
            for (int i = 0; i < missileVec.size(); i++){
               if (missileVec[i].shellSprite.getPosition().y < -200){
                    missileVec.erase(missileVec.begin()+i);
               }
            }
    

            window.draw(playerName);

            // Displaying players health
            sf::Text healthText;
            std::ostringstream sh;
            sh << playerShip.health;
            healthText.setString(sh.str());
            healthText.setFont(font);
            healthText.setColor(sf::Color::Green);
            healthText.setCharacterSize(28);
            healthText.setPosition(frameWidth*0.25-70,frameHeight-42);
            window.draw(healthText);

            // Displaying kills: text 
            window.draw(killTextTitle);

            // Displaying number of kills
            sf::Text killsText;
            std::ostringstream sk;
            sk << kills;
            killsText.setString(sk.str());
            killsText.setFont(font);
            killsText.setColor(sf::Color::Yellow);
            killsText.setCharacterSize(28);
            killsText.setPosition(180,20);
            window.draw(killsText);

            // Displaying health rectangle
            healthRect.setSize(sf::Vector2f(playerShip.health*7*5,15));
            window.draw(healthRect);
            window.draw(healthFrame);


            // Drawing player ship
            window.draw(playerShip.shipSprite);

            // Drawing player bullets
            for (int i = 0; i < playerBullets.size(); i++){
                // drawing bullets for player's ship
                window.draw(playerBullets[i].bulletShape);
                // updating bullets positions
                playerBullets[i].updatePos();
                // If bullets go out of the frame they are deleted
                if (playerBullets[i].getYPos()<0){
                    playerBullets.erase(playerBullets.begin()+i);
                }
            }


            // Moving the player ship
            if (rightKeyState){playerShip.moveRight(frameWidth);}
            if (leftKeyState){playerShip.moveLeft(frameWidth);}

            // Adding new alien ships randomly
            if (rand()%100<2){
                if (rand()%100< 10 + (10+kills)/2-alienShipVec.size()){
                    alienShip newAlienShip(frameWidth,frameHeight,alienShipTexture);
                    alienShipVec.push_back(newAlienShip);
                }
            }

            // Moving alien ships and fire bullets randomly
            for (int i = 0;i < alienShipVec.size(); i++){                
                alienShipVec[i].move(frameWidth,frameHeight);
                // Fire bullets
                if (alienShipVec[i].fireBullet(alienBullets)){
                    alienShipShoot.play();
                }
            }

            // Drawing alien ships
            for (int i = 0;i < alienShipVec.size(); i++){
                // If alive draw it otherwise erase it
                if (alienShipVec[i].live){
                    window.draw(alienShipVec[i].shipSprite);
                } else {
                    // Removing death alienships
                    alienShipVec.erase(alienShipVec.begin()+i);
                    alienShipExplote.play();
                    kills = kills +1;
                }
                alienShipVec[i].updateHealth(playerBullets,explosionVec,missileVec,explosionTexture);
                // Deleting out of frame alienships
                if(alienShipVec[i].getYPos()>frameHeight){
                    alienShipVec.erase(alienShipVec.begin()+i);
                }
            }

            // Drawing explosions
            for (int i=0; i < explosionVec.size(); i++){
                window.draw(explosionVec[i].zone1);
                window.draw(explosionVec[i].zone2);
                window.draw(explosionVec[i].zone3);
                window.draw(explosionVec[i].explosionSprite);
                if (explosionVec[i].duration==0){
                    explosionExploteSound.play();
                }
                if(explosionVec[i].explote()){
                    explosionVec.erase(explosionVec.begin()+i);
                    //std::cout << "deleting" << std::endl;
                }
            }

            // Destroying alienship within explosion action radius
            for (int i = 0; i < alienShipVec.size() ; i++){
                for (int j = 0; j < explosionVec.size(); j++){
                    if (alienShipVec[i].shipSprite.getGlobalBounds().intersects(explosionVec[j].zone2.getGlobalBounds())){
                        alienShipVec[i].live = false;
                        alienShipVec[i].health = 0;
                        kills++;
                    }
                }
            }

            // Updating player health
            if(playerShip.updateHealth(alienBullets)){
                playerShipDamage.play();
            };

            // Playing critical damage sound
            if (playerShip.health ==1 && criticalDamagePlayedTimes == 0){
                playerShipSevereDamage.play();
                criticalDamagePlayedTimes = 1;
            }

            // Drawing alien bullets
            for (int i = 0; i < alienBullets.size(); i++){
                // Draw each bullet for each alien ship
                window.draw(alienBullets[i].bulletShape);
                // Update position of each alien bullet
                alienBullets[i].updatePos();
                // Deleting out of frame bullets
                if (alienBullets[i].getYPos()>frameHeight){
                    alienBullets.erase(alienBullets.begin()+i);
                }
            }




        }
        else{
            while (window.pollEvent(event)){
                switch(event.type)
                    {
                    case sf::Event::Closed:
                        window.close();
                        break;
                    }
            }
            
            // if player looses all its health window is cleared
            //window.getView()

            if (countFade==0){
                playerShipExplote.play();
            }

            healthRect.setFillColor(sf::Color::Black);
            window.draw(healthRect);

            if (countFade < 1000){
                if (countFade%20==0){
                    window.draw(fadeScreen);
                }
                countFade++;
            }
            //window.clear(sf::Color::Transparent);
            
            // Creating game over text to be displayed
            sf::Text gameOverText;
            gameOverText.setString("Game Over!");
            gameOverText.setFont(font);
            gameOverText.setColor(sf::Color::Red);
            gameOverText.setCharacterSize(100);
            
            sf::FloatRect textRect = gameOverText.getLocalBounds();
            gameOverText.setOrigin(textRect.left + textRect.width/2.0f,
                                   textRect.top + textRect.height/2.0f);
            
            gameOverText.setPosition(sf::Vector2f(frameWidth/2.0f,frameHeight/2.0f));
            window.draw(gameOverText);
        }
        window.display();

    }
    //std::cout << alienShipVec.size() << std::endl;
    std::cout << explosionVec.size() << std::endl;
    return 0;
}