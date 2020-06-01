#include "Player.h"


////////////////////////////////////////
////////// PLAYER CLASS ////////////////
////////////////////////////////////////
Player::Player(sf::Texture* texture, sf::Vector2f ssize, float speed, float pos) :
    collider(body)
{
    body.setSize(ssize);
    body.setTexture(texture);
    body.setOrigin(body.getSize().x / 2, body.getSize().y / 2);
    body.setPosition(sf::Vector2f(350, pos));
    this->speed = speed;
    directionR = true;
}

Player::~Player()
{
    //dtor
}

void Player::draw(sf::RenderWindow& window)
{
    window.draw(body);
}

void Player::update(sf::RenderWindow& window, float deltaT)
{
    if  (
        (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        && (body.getPosition().x > body.getSize().x / 2)
        )
            {
                body.move(sf::Vector2f(-speed * deltaT, 0.0f));
                directionR = false;
            }

    if  (
        (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        && (body.getPosition().x < window.getSize().x - body.getSize().x / 2)
        )
           {
                body.move(sf::Vector2f(speed * deltaT, 0.0f));
                directionR = true;
           }

}

sf::Vector2f Player::getPosition()
{
    return body.getPosition();
}

sf::Vector2f Player::getSize()
{
    return body.getSize();
}

////////////////////////////////////////
////////// BALL CLASS //////////////////
////////////////////////////////////////
Ball::Ball(sf::Texture* texture, float radius, float speed) :
    collider(body)
{
    body.setOrigin(radius, radius);
    body.setPosition(sf::Vector2f(200, 100));
    body.setSize(sf::Vector2f(2*radius, 2*radius));
    shape.setFillColor(sf::Color(0, 200, 200));
    shape.setOutlineColor(sf::Color(0, 220, 220));
    shape.setOutlineThickness(-2.f);
    shape.setRadius(radius);
    shape.setOrigin(radius, radius);
    shape.setPosition(body.getPosition());

    hSpeed = speed;
    vSpeed = speed;

    totalT = 0.0f;
    stopT = 3.0f;
    stop = true;
}

void Ball::draw(sf::RenderWindow& window)
{
    window.draw(shape);
}

void Ball::update(sf::RenderWindow& window, float deltaT, Lives& lives, Player& player)
{
    if (!stop) body.move(sf::Vector2f(hSpeed*deltaT, vSpeed*deltaT));
    else
    {
            body.setPosition(sf::Vector2f(player.getPosition().x, player.getPosition().y - player.getSize().y / 2 - body.getSize().y / 2 - 1.0f));
            if (player.directionR) hSpeed = abs(hSpeed);
            else hSpeed = -abs(hSpeed);
    }

    if (body.getPosition().x <= body.getSize().x / 2) goRight();
    if (body.getPosition().x >= window.getSize().x - body.getSize().x / 2) goLeft();

    if (body.getPosition().y <= body.getSize().y / 2 + 42.0f) goDown();

    switch (collider.checkCollision(player.getCollider()))
    {
        case 1: goUp(); break;
        case 2: goRight(); break;
        case 4: goLeft(); break;
    }

    if (body.getPosition().y >= window.getSize().y - body.getSize().y / 2)
    {
        stop = true;
        lives.loseLife();
        body.move(sf::Vector2f(0.0f, -1.0f));
    }

    shape.setPosition(body.getPosition());

}

////////////////////////////////////////
//////////// LIVES CLASS ///////////////
////////////////////////////////////////
Lives::Lives(sf::Texture* texture, unsigned int num)
{
    body.setPosition(sf::Vector2f(0,0));
    body.setSize(sf::Vector2f(192.0f, 64.0f));
    body.setOrigin(body.getSize() / 2.0f);
    body.setTexture(texture);
    body.setScale(sf::Vector2f(0.5f, 0.5f));
    lives=num;
}

unsigned int Lives::getLives()
{
    return lives;
}

void Lives::update(float deltaT)
{
    body.setTextureRect(sf::IntRect(0, (3-lives) * body.getSize().y, body.getSize().x, body.getSize().y));
}

void Lives::draw(sf::RenderWindow& window)
{
    if (body.getPosition() == sf::Vector2f(0,0)) body.setPosition(sf::Vector2f(window.getSize().x - body.getSize().x / 4 - 4.0f, body.getSize().y / 4 + 4.0f));
    window.draw(body);
}

void Lives::loseLife()
{
    if (lives>0) lives--;
}

void Lives::reset()
{
    lives = 3;
}


////////////////////////////////////////
////////// COLLIDER CLASS //////////////
////////////////////////////////////////

Collider::Collider(sf::RectangleShape& body) :
    body(body)
{
}

unsigned int Collider::checkCollision(Collider other)
{
    if ((abs(other.getPosition().x - getPosition().x) - (getHalfSize().x + other.getHalfSize().x) <= 0)
        && (abs(other.getPosition().y - getPosition().y) - (getHalfSize().y + other.getHalfSize().y) <= 0))
    {
        if (abs(other.getPosition().x - getPosition().x) - (getHalfSize().x + other.getHalfSize().x) > abs(other.getPosition().y - getPosition().y) - (getHalfSize().y + other.getHalfSize().y))
        {
            if (other.getPosition().x - getPosition().x > 0) return 4;
            else return 2;
        }
        else
        {
            if (other.getPosition().y - getPosition().y > 0) return 1;
            else return 3;
        }
    }
    else return 0;

}


////////////////////////////////////////
//////////// BRICK CLASS ///////////////
////////////////////////////////////////

Brick::Brick() :
    collider(body)
{
    body.setSize(sf::Vector2f(45.0f, 30.0f));
    body.setOrigin(sf::Vector2f(body.getSize().x / 2, body.getSize().y / 2));
    body.setFillColor(sf::Color(204, 102, 0));
    exists = true;
}

void Brick::setPosition(sf::Vector2f pos)
{
    body.setPosition(pos);
}

void Brick::update(Ball& ball)
{
    unsigned int coll = collider.checkCollision(ball.getCollider());
    if (coll != 0)
    {
        brickCounter--;
        exists = false;
        switch (coll)
        {
        case 1:
            ball.goDown();
            break;
        case 2:
            ball.goLeft();
            break;
        case 3:
            ball.goUp();
            break;
        case 4:
            ball.goRight();
            break;
        }
    }
}

void Brick::draw(sf::RenderWindow& window)
{
    window.draw(body);
}

sf::Vector2f Brick::getSize()
{
    return body.getSize();
}

sf::Vector2f Brick::getPosition()
{
    return body.getPosition();
}
