#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

// Classes used for representing in-game objects
class GameObjectModel
{
    public:
        bool collides(sf::FloatRect bounds1, sf::FloatRect bounds2) {
            if (bounds1.intersects(bounds2))
                return true;
            return false;
        }
};
GameObjectModel obj;

class Player : public sf::Sprite, public GameObjectModel
{
    public:
        Player() : sf::Sprite() {}
};

class Platform : public sf::RectangleShape, public GameObjectModel
{
    public:
        Platform() : sf::RectangleShape() {}
};

class Medal : public sf::CircleShape, public GameObjectModel
{
    public:
        Medal() : sf::CircleShape() {}
};

class Hidden : public sf::RectangleShape, public GameObjectModel
{
    public:
        Hidden() : sf::RectangleShape() {}
};

// Initializes the objects to be drawn in the game
Platform movPlatform;
Platform endPlatform;
Platform movPlatform2;
Platform levelPlatform;
Platform endPlatform2;
Platform ground;
Platform ground2;
Platform ground3;
Player character;
Medal medal;
Hidden spawnLoc;
Hidden deathZone;
Hidden sideBound;
sf::RenderWindow window(sf::VideoMode(800, 600), "DP Window", sf::Style::Close | sf::Style::Resize);

// Initializes variables used for handling certain game functionality
int t = 0;
bool finish = false;
bool canJump = true;
bool onTop = false;
bool pause = false;
bool nextLevel = false;
bool record = false;
bool goalRecorded = false;
bool goalRecorded2 = false;
bool transitionRecorded = false;
float velocityX = 0;
float velocityY = 0;
float gravity = 1;
int airTime = 0;
int length = 0;
int recordTime = 0;
int endGoalTime1 = 0;
int endGoalTime2 = 0;
int levelChangeTime = 0;
int goalTime = 0;
int goalTime2 = 0;
int transitionTime = 0;
sf::RectangleShape *collidables[4];
sf::Vector2f characterSize;
sf::FloatRect playerBounds;
sf::Vector2f playerPos[1000];
sf::Vector2f platformPos[1000];
sf::Time delay = sf::milliseconds(100);
sf::Mutex mutex;

// Method for moving the moving platform in level 1 in a thread
void movePlatform()
{
    mutex.lock();
    if (t % 38 < 19) {
        movPlatform.move(15.f, 0.f);
        if (onTop)
            character.move(15.f, 0.f);
    }
    else {
        movPlatform.move(-15.f, 0.f);
        if (onTop)
            character.move(-15.f, 0.f);
    }
    t++;
    mutex.unlock();
}

// Method for moving the moving platform in level 2 in a thread
void movePlatform2()
{
    mutex.lock();
    if (t % 20 < 10) {
        movPlatform2.move(0.f, -10.f);
        if (onTop)
            character.move(0.f, -10.f);
    }
    else {
        movPlatform2.move(0.f, 10.f);
        if (onTop)
            character.move(0.f, 10.f);
    }
    t++;
    mutex.unlock();
}

// Creates the threads used for moving the moving platforms and player
sf::Thread movPlatformThread(&movePlatform);
sf::Thread movPlatform2Thread(&movePlatform2);

// Class used for managing and handling certain in-game events
class EventManager
{
    public:
        // Method for handling various user inputs in the game
        void input() {
            // Handles resetting the game when the specified keyboard combo is inputted by the player
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !record && !pause) {
                t = 0;
                finish = false;
                canJump = true;
                onTop = false;
                nextLevel = false;
                velocityX = 0;
                velocityY = 0;
                airTime = 0;
                character.setPosition(-30.f, 220.f);
                movPlatform.setPosition(250.f, 250.f);
                movPlatform2.setPosition(400.f, 200.f);
            }
            // Handles recording functionality for a player's game replay
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R) && !pause)
                record = true;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && !pause) {
                record = false;
                pause = true;
                stopRecord();
                recordTime = 0;
                pause = false;
                goalRecorded = false;
                goalRecorded2 = false;
                transitionRecorded = false;
                goalTime = 0;
                goalTime2 = 0;
                endGoalTime1 = 0;
                endGoalTime2 = 0;
                transitionTime = 0;
                levelChangeTime = 0;
            }
            if (record)
                startRecord();
        
            // Handles pausing the threads and in-game objects when the game is paused by the player
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !record) {
                pause = !pause;
            }
            if (pause) {
                if (!nextLevel)
                    movPlatformThread.wait();
                else
                    movPlatform2Thread.wait();;
            }
            else {
                if (!nextLevel)
                    movPlatformThread.launch();
                else
                    movPlatform2Thread.launch();
            }
        
            // Handles changing the game speed to 0.5, 1 or 2 based on the game speed button pressed
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num0) && !pause && !record)
                delay = sf::milliseconds(200);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1) && !pause && !record)
                delay = sf::milliseconds(100);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2) && !pause && !record)
                delay = sf::milliseconds(50);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !pause)
                velocityX = -10;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !pause)
                velocityX = 10;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && canJump && !pause) {
                velocityY = -15;
                canJump = false;
            }
            if (!pause)
                character.move(velocityX, velocityY);
        }
        // Method for handling recoding a player's replay
        void startRecord() {
            playerPos[recordTime] = character.getPosition();
            if (!nextLevel) {
                platformPos[recordTime] = movPlatform.getPosition();
                if (goalRecorded)
                    endGoalTime1 = goalTime;
            }
            else {
                platformPos[recordTime] = movPlatform2.getPosition();
                if (goalRecorded2)
                    endGoalTime2 = goalTime2;
            }
            if (!goalRecorded)
                goalTime++;
            if (!goalRecorded2)
                goalTime2++;
            if (!transitionRecorded)
                transitionTime++;
            if (transitionRecorded)
                levelChangeTime = transitionTime;
            recordTime++;   
        }
        // Method for stopping and playing a player's recorded replay
        void stopRecord() {
            for (int i = 0; i < recordTime; i++) {
                window.clear(sf::Color::Black);
                character.setPosition(playerPos[i]);
                window.draw(character);
                if (transitionRecorded) {
                    nextLevel = false;
                    medal.setPosition(765.f, 185.f);
                }
                if (i >= levelChangeTime && levelChangeTime != 0) {
                    nextLevel = true;
                    medal.setPosition(765.f, 135.f);
                    finish = false;
                }
                if (!nextLevel) {
                    movPlatform.setPosition(platformPos[i]);
                    window.draw(movPlatform);
                    window.draw(endPlatform);
                    if (goalRecorded)
                        finish = false;
                    if ((i >= endGoalTime1 && endGoalTime1 != 0) || (i < levelChangeTime && levelChangeTime != 0 && endGoalTime1 == 0))
                        finish = true;
                    if ((i < endGoalTime1 || endGoalTime1 == 0) && !finish)
                        window.draw(medal);
                    window.draw(ground);
                }
                else {
                    movPlatform2.setPosition(platformPos[i]);
                    window.draw(movPlatform2);
                    window.draw(ground2);
                    window.draw(ground3);
                    window.draw(levelPlatform);
                    window.draw(endPlatform2);
                    if (goalRecorded2)
                        finish = false;
                    if (i >= endGoalTime2 && endGoalTime2 != 0)
                        finish = true;
                    if ((i < endGoalTime2 || endGoalTime2 == 0) && !finish)
                        window.draw(medal);
                }
                window.display();
                sf::sleep(delay);
            }
        }
        // Method for handling in-game collision detection with the player and various objects
        void collision(sf::FloatRect bounds1, sf::FloatRect bounds2, int index) {
            if (obj.collides(bounds1, bounds2)) {
                float bottomPos = character.getPosition().y + characterSize.y;
                float dif = bottomPos - (*collidables[index]).getPosition().y; 
                if (index == 2)
                    onTop = true;
                else
                    onTop = false;
                if (dif >= 0) {
                    canJump = true;
                    velocityY = 0;
                    airTime = 0;
                    character.setPosition(character.getPosition().x, (*collidables[index]).getPosition().y - characterSize.y + 12);
                }        
            }
        }
        // Method for handling spawning the player back to the start of level 2 when player dies
        void spawn(sf::Vector2f position) {
            if (position.y > deathZone.getPosition().y)
                character.setPosition(spawnLoc.getPosition().x, spawnLoc.getPosition().y);
        }
        // Method for handling and triggering the character spawn event when the player dies in level 2
        void death(sf::Vector2f position) {
            spawn(position);
        }
};
EventManager ems;

// Class for managing scripted event handling and scripted game object behavior
class ScriptManager 
{
    public:
        // Method for raising and handling the input event
        void callInput() {
            ems.input();
        }
        // Method for raising and handling the collsion event 
        void callCollision(sf::FloatRect bounds1, sf::FloatRect bounds2, int index) {
            ems.collision(bounds1, bounds2, index);
        }
        // Method for raising and handling the spawn event
        void callSpawn(sf::Vector2f position) {
            ems.spawn(position);
        }
        // Method for raising and handling the death event
        void callDeath(sf::Vector2f position) {
            callSpawn(position);
        }
};
ScriptManager sm;

int main()
{
    // Sets the position, texture, and size of the in-game objects
    movPlatform.setSize(sf::Vector2f(50.f, 20.f)); 
    movPlatform.setPosition(250.f, 250.f);
    sf::Texture movPlatformTexture;
    movPlatformTexture.loadFromFile("ground2.png");
    movPlatformTexture.setSmooth(true);
    movPlatformTexture.setRepeated(true);
    movPlatform.setTexture(&movPlatformTexture);
    movPlatform.setTextureRect(sf::IntRect(250, 250, 50, 20));

    endPlatform.setSize(sf::Vector2f(100.f, 75.f));
    endPlatform.setPosition(700.f, 225.f);
    sf::Texture endPlatformTexture;
    endPlatformTexture.loadFromFile("ground1.png");
    endPlatformTexture.setSmooth(true);
    endPlatformTexture.setRepeated(true);
    endPlatform.setTexture(&endPlatformTexture);
    endPlatform.setTextureRect(sf::IntRect(700, 225, 100, 75));

    movPlatform2.setSize(sf::Vector2f(75.f, 25.f)); 
    movPlatform2.setPosition(400.f, 200.f);
    sf::Texture movPlatformTexture2;
    movPlatformTexture2.loadFromFile("OrangeBlockFX.png");
    movPlatformTexture2.setSmooth(true);
    movPlatformTexture2.setRepeated(true);
    movPlatform2.setTexture(&movPlatformTexture2);
    movPlatform2.setTextureRect(sf::IntRect(400, 200, 75, 25));

    levelPlatform.setSize(sf::Vector2f(50.f, 50.f));
    levelPlatform.setPosition(300.f, 250.f);
    sf::Texture levelPlatformTexture;
    levelPlatformTexture.loadFromFile("RedBlockFX.png");
    levelPlatformTexture.setSmooth(true);
    levelPlatformTexture.setRepeated(true);
    levelPlatform.setTexture(&levelPlatformTexture);
    levelPlatform.setTextureRect(sf::IntRect(300, 250, 50, 50));

    endPlatform2.setSize(sf::Vector2f(200.f, 125.f));
    endPlatform2.setPosition(600.f, 175.f);
    sf::Texture endPlatformTexture2;
    endPlatformTexture2.loadFromFile("BlueBlockFX.png");
    endPlatformTexture2.setSmooth(true);
    endPlatformTexture2.setRepeated(true);
    endPlatform2.setTexture(&endPlatformTexture2);
    endPlatform2.setTextureRect(sf::IntRect(600, 175, 200, 125));

    medal.setRadius(15.f);
    medal.setFillColor(sf::Color::Yellow);
    medal.setPointCount(60.f);

    spawnLoc.setPosition(-30.f, 220.f);
    deathZone.setPosition(500.f, 600.f);
    sideBound.setPosition(window.getSize().x, 0.f);

    ground.setSize(sf::Vector2f(800.f, 300.f));
    ground.setPosition(0.f, 300.f);
    sf::Texture groundTexture;
    groundTexture.loadFromFile("grass.png");
    groundTexture.setSmooth(true);
    groundTexture.setRepeated(true);
    ground.setTexture(&groundTexture);
    ground.setTextureRect(sf::IntRect(0, 300, 800, 300));
    
    ground2.setSize(sf::Vector2f(350.f, 300.f));
    ground2.setPosition(0.f, 300.f);
    ground2.setTexture(&groundTexture);
    ground2.setTextureRect(sf::IntRect(0, 300, 350, 300));

    ground3.setSize(sf::Vector2f(200.f, 300.f));
    ground3.setPosition(600.f, 300.f);
    ground3.setTexture(&groundTexture);
    ground3.setTextureRect(sf::IntRect(600, 300, 200, 300));
    
    character.setPosition(-30.f, 220.f);
    sf::Texture playerTexture;
    playerTexture.loadFromFile("player.png");
    playerTexture.setSmooth(true);
    character.setTexture(playerTexture);
    character.setScale(0.15, 0.15);
    characterSize.x = character.getTexture()->getSize().x * 0.15;
    characterSize.y = character.getTexture()->getSize().y * 0.15;

    sf::FloatRect medalBounds = medal.getGlobalBounds();
    playerBounds = character.getGlobalBounds();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color::Black);

        sm.callInput();
        
        playerBounds = character.getGlobalBounds();
        if (!nextLevel)
            medal.setPosition(765.f, 185.f);
        else
            medal.setPosition(765.f, 135.f);

        if (!nextLevel) {
            collidables[0] = &ground;
            collidables[1] = &endPlatform;
            collidables[2] = &movPlatform;
        }
        else {
            collidables[0] = &ground2;
            collidables[1] = &levelPlatform;
            collidables[2] = &movPlatform2;
            collidables[3] = &endPlatform2;
        }  
        // Handles collision detetction event for various objects that can 
        // collide with the player
        if (!nextLevel)
            length = 3;
        else 
            length = 4;
        for (int i = 0; i < length; i++) {
            sm.callCollision(playerBounds, (*collidables[i]).getGlobalBounds(), i);
            if (velocityY == 0)
                break;
        }

        // Handles screen collision detection to prevent player from going off-screen
        // and character death/spawn events when player dies in level 2 
        sf::Vector2f position = character.getPosition();
        if (position.x < -30.f)
            character.move(10.f, 0.f);
        if (position.x > (sideBound.getPosition().x - characterSize.x + 10)) {
            if (nextLevel)
                character.move(-10.f, 0.f);
            else {
                if (record)
                    transitionRecorded = true;
                nextLevel = true;
                finish = false;
                t = 0;
                movPlatformThread.wait();
                character.setPosition(-30.f, 220.f);
            }
        }
        sm.callDeath(position);
        
        // Checks if player collected medal to make it not appear on screen
        playerBounds = character.getGlobalBounds();
        medalBounds = medal.getGlobalBounds();
        if (medalBounds.intersects(playerBounds) && !finish) {
            finish = true;
            if (record) {
                if (!nextLevel)
                    goalRecorded = true;
                else
                    goalRecorded2 = true;
            }
        }
        sf::sleep(delay);
        // Draws the in-game objects and adjust velocity of character accordingly
        if (!nextLevel) {
            window.draw(movPlatform);
            window.draw(endPlatform);
            if (!finish)
                window.draw(medal);
            window.draw(ground);
        }
        else {
            window.draw(ground2);
            window.draw(ground3);
            window.draw(movPlatform2);
            window.draw(levelPlatform);
            window.draw(endPlatform2);
            if (!finish)
                window.draw(medal);
        }
        window.draw(character);
        window.display();
        velocityX = 0;
        if (!pause) {
            velocityY += airTime * gravity;
            if (!canJump)
                airTime++;
        }
    }
    return 0;
}