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
Wall trackWall1;
Wall trackWall2;
Wall trackWall3;
Wall trackWall4;
Wall trackWall5;
Wall trackWall6;
Wall trackWall7;
Wall trackWall8;
Wall trackWall9;
Wall trackWall10;
Wall trackWall11;
Wall trackWall12;
Wall trackWall13;
Wall trackWall14;
Wall trackWall15;
Wall trackWall16;
Player character;
Medal medal;
sf::RenderWindow window(sf::VideoMode(800, 600), "DP Window", sf::Style::Close | sf::Style::Resize);

// Initializes variables used for handling certain game functionality
bool finish = false;
bool moveLeft = false;
bool moveRight = true;
bool moveUp = false;
bool moveDown = false;
sf::RectangleShape *collidables[16] = {&trackWall1, &trackWall2, &trackWall3, &trackWall4, &trackWall5, &trackWall6,
&trackWall7, &trackWall8, &trackWall9, &trackWall10, &trackWall11, &trackWall12, &trackWall13, &trackWall14,
&trackWall15, &trackWall16};
sf::Vector2f characterSize;
sf::FloatRect playerBounds;
sf::Time delay = sf::milliseconds(100);

// Class used for managing and handling certain in-game events
class EventManager
{
    public:
        // Method for handling various user inputs in the game
        void input() {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                moveLeft = true;
                moveRight = false;
                moveUp = false;
                moveDown = false;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                moveLeft = false;
                moveRight = true;
                moveUp = false;
                moveDown = false;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                moveLeft = false;
                moveRight = false;
                moveUp = true;
                moveDown = false;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                moveLeft = false;
                moveRight = false;
                moveUp = false;
                moveDown = true;
            }
            if (moveLeft)
                character.move(-10.f, 0.f);
            if (moveRight)
                character.move(10.f, 0.f);
            if (moveUp)
                character.move(0.f, -10.f);
            if (moveDown)
                character.move(0.f, 10.f);
        }
        // Method for handling in-game collision detection with the player and various objects
        void collision(sf::FloatRect bounds1, sf::FloatRect bounds2) {
            if (obj.collides(bounds1, bounds2))
                death();
        }
        // Method for handling spawning the player back to the start of the track when the
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
    playerTexture.loadFromFile("spr_formula_0.png");
    playerTexture.setSmooth(true);
    character.setTexture(playerTexture);
    character.setScale(0.2, 0.2);
    characterSize.x = character.getTexture()->getSize().x * 0.2;
    characterSize.y = character.getTexture()->getSize().y * 0.2;

    sf::Texture wallTexture;
    wallTexture.loadFromFile("brick_wall.png");
    wallTexture.setSmooth(true);
    wallTexture.setRepeated(true);

    trackWall1.setSize(sf::Vector2f(800.f, 10.f)); 
    trackWall1.setPosition(0.f, 0.f);
    trackWall1.setTexture(&wallTexture);
    trackWall1.setTextureRect(sf::IntRect(0, 0, 800, 10));
    trackWall2.setSize(sf::Vector2f(800.f, 10.f)); 
    trackWall2.setPosition(0.f, 590.f);
    trackWall2.setTexture(&wallTexture);
    trackWall2.setTextureRect(sf::IntRect(0, 590, 800, 10));
    trackWall3.setSize(sf::Vector2f(10.f, 520.f)); 
    trackWall3.setPosition(0.f, 70.f);
    trackWall3.setTexture(&wallTexture);
    trackWall3.setTextureRect(sf::IntRect(0, 70, 10, 520));
    trackWall4.setSize(sf::Vector2f(10.f, 520.f)); 
    trackWall4.setPosition(790.f, 10.f);
    trackWall4.setTexture(&wallTexture);
    trackWall4.setTextureRect(sf::IntRect(790, 10, 10, 520));
    trackWall5.setSize(sf::Vector2f(400.f, 30.f)); 
    trackWall5.setPosition(10.f, 70.f);
    trackWall5.setTexture(&wallTexture);
    trackWall5.setTextureRect(sf::IntRect(10, 70, 400, 30));
    trackWall6.setSize(sf::Vector2f(430.f, 60.f)); 
    trackWall6.setPosition(90.f, 160.f);
    trackWall6.setTexture(&wallTexture);
    trackWall6.setTextureRect(sf::IntRect(90, 160, 430, 60));
    trackWall7.setSize(sf::Vector2f(30.f, 150.f)); 
    trackWall7.setPosition(490.f, 10.f);
    trackWall7.setTexture(&wallTexture);
    trackWall7.setTextureRect(sf::IntRect(490, 10, 30, 150));
    trackWall8.setSize(sf::Vector2f(220.f, 50.f)); 
    trackWall8.setPosition(10.f, 270.f);
    trackWall8.setTexture(&wallTexture);
    trackWall8.setTextureRect(sf::IntRect(10, 270, 220, 50));
    trackWall9.setSize(sf::Vector2f(70.f, 150.f)); 
    trackWall9.setPosition(310.f, 220.f);
    trackWall9.setTexture(&wallTexture);
    trackWall9.setTextureRect(sf::IntRect(310, 220, 70, 150));
    trackWall10.setSize(sf::Vector2f(170.f, 170.f)); 
    trackWall10.setPosition(90.f, 370.f);
    trackWall10.setTexture(&wallTexture);
    trackWall10.setTextureRect(sf::IntRect(90, 370, 170, 170));
    trackWall11.setSize(sf::Vector2f(120.f, 110.f)); 
    trackWall11.setPosition(260.f, 370.f);
    trackWall11.setTexture(&wallTexture);
    trackWall11.setTextureRect(sf::IntRect(260, 370, 120, 110));
    trackWall12.setSize(sf::Vector2f(160.f, 60.f)); 
    trackWall12.setPosition(340.f, 530.f);
    trackWall12.setTexture(&wallTexture);
    trackWall12.setTextureRect(sf::IntRect(330, 530, 160, 60));
    trackWall13.setSize(sf::Vector2f(40.f, 160.f)); 
    trackWall13.setPosition(460.f, 370.f);
    trackWall13.setTexture(&wallTexture);
    trackWall13.setTextureRect(sf::IntRect(460, 370, 40, 160));
    trackWall14.setSize(sf::Vector2f(110.f, 210.f)); 
    trackWall14.setPosition(600.f, 60.f);
    trackWall14.setTexture(&wallTexture);
    trackWall14.setTextureRect(sf::IntRect(600, 60, 110, 210));
    trackWall15.setSize(sf::Vector2f(250.f, 100.f)); 
    trackWall15.setPosition(460.f, 270.f);
    trackWall15.setTexture(&wallTexture);
    trackWall15.setTextureRect(sf::IntRect(460, 270, 250, 100));
    trackWall16.setSize(sf::Vector2f(210.f, 110.f)); 
    trackWall16.setPosition(580.f, 420.f);
    trackWall16.setTexture(&wallTexture);
    trackWall16.setTextureRect(sf::IntRect(580, 420, 210, 110));

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
        for (int i = 0; i < 16; i++) 
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
        for (int i = 0; i < 16; i++)
            window.draw(*collidables[i]);
        if (!finish)
            window.draw(medal);
        window.display();
    }
    return 0;
}