#include <SFML/Graphics.hpp>

class Player;
class Ball;
class Lives;
class Collider;
class Brick;


class Collider
{
public:
    Collider(sf::RectangleShape&);

    unsigned int checkCollision(Collider);
    sf::Vector2f getPosition() {return body.getPosition();}
    sf::Vector2f getHalfSize() {return body.getSize() / 2.0f;}

private:
    sf::RectangleShape& body;
};

class Player
{
    public:
        Player(sf::Texture*, sf::Vector2f=sf::Vector2f(100,100), float=100.0f, float=672);
        ~Player();

        void draw(sf::RenderWindow&);
        void update(sf::RenderWindow&, float);
        sf::Vector2f getPosition();
        sf::Vector2f getSize();
        Collider getCollider() {return collider;}

        bool directionR;

    protected:

    private:
        sf::RectangleShape body;
        float speed;
        Collider collider;
};


class Ball
{
    public:
        Ball(sf::Texture*, float=15.0f, float=100.0f);

        void draw(sf::RenderWindow&);
        void update(sf::RenderWindow&, float, Lives&, Player&);
        Collider getCollider() {return collider;}

        void goUp() { vSpeed = -abs(vSpeed); }
        void goDown() { vSpeed = abs(vSpeed); }
        void goLeft() { hSpeed = -abs(hSpeed); }
        void goRight() { hSpeed = abs(hSpeed); }

        bool stop;

    protected:

    private:
        unsigned int moreLives;
        sf::RectangleShape body;
        sf::CircleShape shape;
        float hSpeed, vSpeed, totalT, stopT;

        Collider collider;
};


class Lives
{
public:
    Lives(sf::Texture*, unsigned int);

    void reset();
    void loseLife();
    unsigned int getLives();
    void update(float);
    void draw(sf::RenderWindow&);


private:
    unsigned int lives;
    sf::RectangleShape body;
};


class Brick
{
public:
    static unsigned int brickCounter;

    Brick();

    sf::Vector2f getSize(), getPosition();
    void setPosition(sf::Vector2f);
    void update(Ball&);
    void draw(sf::RenderWindow&);
    Collider getCollider() {return collider;}

    bool exists;

private:
    sf::RectangleShape body;
    Collider collider;
};

