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

class Wall : public sf::RectangleShape, public GameObjectModel
{
    public:
        Wall() : sf::RectangleShape() {}
};

class Medal : public sf::CircleShape, public GameObjectModel
{
    public:
        Medal() : sf::CircleShape() {}
};

// Initializes the objects to be drawn in the game
Wall topMazeWall;
Wall bottomMazeWall;
Wall leftMazeWall;
Wall rightMazeWall;
Wall innerMazeWall1;
Wall innerMazeWall2;
Wall innerMazeWall3;
Wall innerMazeWall4;
Wall innerMazeWall5;
Wall innerMazeWall6;
Wall innerMazeWall7;
Wall innerMazeWall8;
Wall innerMazeWall9;
Wall innerMazeWall10;
Wall innerMazeWall11;
Wall innerMazeWall12;
Wall innerMazeWall13;
Wall innerMazeWall14;
Wall innerMazeWall15;
Player character;
Medal medal;
sf::RenderWindow window(sf::VideoMode(800, 600), "DP Window", sf::Style::Close | sf::Style::Resize);

// Initializes variables used for handling certain game functionality
bool finish = false;
sf::RectangleShape *collidables[19] = {&topMazeWall, &bottomMazeWall, &leftMazeWall, &rightMazeWall, &innerMazeWall1, &innerMazeWall2,
&innerMazeWall3, &innerMazeWall4, &innerMazeWall5, &innerMazeWall6, &innerMazeWall7, &innerMazeWall8, &innerMazeWall8, &innerMazeWall10,
&innerMazeWall11, &innerMazeWall12, &innerMazeWall13, &innerMazeWall14, &innerMazeWall15};
sf::Vector2f characterSize;
sf::FloatRect playerBounds;
sf::Time delay = sf::milliseconds(100);

// Class used for managing and handling certain in-game events
class EventManager
{
    public:
        // Method for handling various user inputs in the game
        void input() {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                character.move(-10.f, 0.f);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                character.move(10.f, 0.f);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) 
                character.move(0.f, -10.f);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                character.move(0.f, 10.f);
        }
        // Method for handling in-game collision detection with the player and various objects
        void collision(sf::FloatRect bounds1, sf::FloatRect bounds2) {
            if (obj.collides(bounds1, bounds2))
                death();
        }
        // Method for handling spawning the player back to the start of the maze when the
        // player hits a wall
        void spawn() {
            character.setPosition(5.f, 30.f);
        }
        // Method for handling and triggering the character spawn event when the player dies
        void death() {
            spawn();
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
        void callCollision(sf::FloatRect bounds1, sf::FloatRect bounds2) {
            ems.collision(bounds1, bounds2);
        }
        // Method for raising and handling the spawn event
        void callSpawn() {
            ems.spawn();
        }
        // Method for raising and handling the death event
        void callDeath() {
            callSpawn();
        }
};
ScriptManager sm;

int main()
{
    // Sets the position, texture, and size of the in-game objects
    medal.setRadius(10.f);
    medal.setFillColor(sf::Color::Yellow);
    medal.setPointCount(60.f);
    medal.setPosition(775.f, 550.f);
    
    character.setPosition(5.f, 30.f);
    sf::Texture playerTexture;
    playerTexture.loadFromFile("character.png");
    playerTexture.setSmooth(true);
    character.setTexture(playerTexture);
    character.setScale(0.5, 0.5);
    characterSize.x = character.getTexture()->getSize().x * 0.5;
    characterSize.y = character.getTexture()->getSize().y * 0.5;

    sf::Texture wallTexture;
    wallTexture.loadFromFile("brick_wall-red.png");
    wallTexture.setSmooth(true);
    wallTexture.setRepeated(true);

    topMazeWall.setSize(sf::Vector2f(800.f, 10.f)); 
    topMazeWall.setPosition(0.f, 0.f);
    topMazeWall.setTexture(&wallTexture);
    topMazeWall.setTextureRect(sf::IntRect(0, 0, 800, 10));
    bottomMazeWall.setSize(sf::Vector2f(800.f, 10.f)); 
    bottomMazeWall.setPosition(0.f, 590.f);
    bottomMazeWall.setTexture(&wallTexture);
    bottomMazeWall.setTextureRect(sf::IntRect(0, 590, 800, 10));
    leftMazeWall.setSize(sf::Vector2f(10.f, 520.f)); 
    leftMazeWall.setPosition(0.f, 70.f);
    leftMazeWall.setTexture(&wallTexture);
    leftMazeWall.setTextureRect(sf::IntRect(0, 70, 10, 520));
    rightMazeWall.setSize(sf::Vector2f(10.f, 520.f)); 
    rightMazeWall.setPosition(790.f, 10.f);
    rightMazeWall.setTexture(&wallTexture);
    rightMazeWall.setTextureRect(sf::IntRect(790, 10, 10, 520));

    innerMazeWall1.setSize(sf::Vector2f(400.f, 30.f)); 
    innerMazeWall1.setPosition(10.f, 70.f);
    innerMazeWall1.setTexture(&wallTexture);
    innerMazeWall1.setTextureRect(sf::IntRect(10, 70, 400, 30));
    innerMazeWall2.setSize(sf::Vector2f(670.f, 70.f)); 
    innerMazeWall2.setPosition(60.f, 150.f);
    innerMazeWall2.setTexture(&wallTexture);
    innerMazeWall2.setTextureRect(sf::IntRect(60, 150, 670, 70));
    innerMazeWall3.setSize(sf::Vector2f(320.f, 90.f)); 
    innerMazeWall3.setPosition(470.f, 10.f);
    innerMazeWall3.setTexture(&wallTexture);
    innerMazeWall3.setTextureRect(sf::IntRect(470, 10, 320, 90));
    innerMazeWall4.setSize(sf::Vector2f(250.f, 50.f)); 
    innerMazeWall4.setPosition(10.f, 270.f);
    innerMazeWall4.setTexture(&wallTexture);
    innerMazeWall4.setTextureRect(sf::IntRect(10, 270, 250, 50));
    innerMazeWall5.setSize(sf::Vector2f(70.f, 150.f)); 
    innerMazeWall5.setPosition(310.f, 220.f);
    innerMazeWall5.setTexture(&wallTexture);
    innerMazeWall5.setTextureRect(sf::IntRect(310, 220, 70, 150));
    innerMazeWall6.setSize(sf::Vector2f(200.f, 170.f)); 
    innerMazeWall6.setPosition(60.f, 370.f);
    innerMazeWall6.setTexture(&wallTexture);
    innerMazeWall6.setTextureRect(sf::IntRect(60, 370, 200, 170));
    innerMazeWall7.setSize(sf::Vector2f(200.f, 60.f)); 
    innerMazeWall7.setPosition(260.f, 420.f);
    innerMazeWall7.setTexture(&wallTexture);
    innerMazeWall7.setTextureRect(sf::IntRect(260, 420, 200, 60));
    innerMazeWall8.setSize(sf::Vector2f(130.f, 50.f)); 
    innerMazeWall8.setPosition(380.f, 320.f);
    innerMazeWall8.setTexture(&wallTexture);
    innerMazeWall8.setTextureRect(sf::IntRect(380, 320, 130, 50));
    innerMazeWall8.setSize(sf::Vector2f(400.f, 10.f)); 
    innerMazeWall8.setPosition(320.f, 530.f);
    innerMazeWall8.setTexture(&wallTexture);
    innerMazeWall8.setTextureRect(sf::IntRect(320, 530, 400, 10));
    innerMazeWall10.setSize(sf::Vector2f(10.f, 60.f)); 
    innerMazeWall10.setPosition(720.f, 530.f);
    innerMazeWall10.setTexture(&wallTexture);
    innerMazeWall10.setTextureRect(sf::IntRect(720, 530, 10, 60));
    innerMazeWall11.setSize(sf::Vector2f(290.f, 100.f)); 
    innerMazeWall11.setPosition(440.f, 270.f);
    innerMazeWall11.setTexture(&wallTexture);
    innerMazeWall11.setTextureRect(sf::IntRect(440, 270, 290, 100));
    innerMazeWall12.setSize(sf::Vector2f(350.f, 10.f)); 
    innerMazeWall12.setPosition(460.f, 420.f);
    innerMazeWall12.setTexture(&wallTexture);
    innerMazeWall12.setTextureRect(sf::IntRect(460, 420, 350, 10));
    innerMazeWall13.setSize(sf::Vector2f(20.f, 50.f)); 
    innerMazeWall13.setPosition(440.f, 370.f);
    innerMazeWall13.setTexture(&wallTexture);
    innerMazeWall13.setTextureRect(sf::IntRect(440, 370, 20, 50));
    innerMazeWall14.setSize(sf::Vector2f(220.f, 50.f)); 
    innerMazeWall14.setPosition(510.f, 480.f);
    innerMazeWall14.setTexture(&wallTexture);
    innerMazeWall14.setTextureRect(sf::IntRect(510, 480, 220, 50));
    innerMazeWall15.setSize(sf::Vector2f(60.f, 50.f)); 
    innerMazeWall15.setPosition(380.f, 320.f);
    innerMazeWall15.setTexture(&wallTexture);
    innerMazeWall15.setTextureRect(sf::IntRect(380, 320, 60, 50));

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

        // Handles collision detetction event for various objects that can 
        // collide with the player
        for (int i = 0; i < 19; i++)
            sm.callCollision(playerBounds, (*collidables[i]).getGlobalBounds());
        // Handles screen collision detection to prevent player from going off-screen
        sf::Vector2f position = character.getPosition();
        if (position.x < 5.f)
            character.move(10.f, 0.f);
        if (position.x > window.getSize().x - characterSize.x)
            character.move(-10.f, 0.f);
        
        // Checks if player collected medal to make it not appear on screen
        playerBounds = character.getGlobalBounds();
        medalBounds = medal.getGlobalBounds();
        if (medalBounds.intersects(playerBounds) && !finish) {
            finish = true;
        }
        sf::sleep(delay);
        // Draws and displays the in-game objects
        window.draw(character);
        for (int i = 0; i < 19; i++)
            window.draw(*collidables[i]);
        if (!finish)
            window.draw(medal);
        window.display();
    }
    return 0;
}