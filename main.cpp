#include <SFML/Graphics.hpp>
#include "Player.h"

unsigned int Brick::brickCounter = 0;

int main()
{
    //constants
    const float
    BALL_SPEED = 400.0f,
    PADDLE_SPEED = 500.0f,
    WINDOW_WIDTH = 700.0f,
    WINDOW_HEIGHT = 742.0f,
    BETWEEN_ROWS = 5.0f,
    BETWEEN_COLS = 5.0f;
    const unsigned int
    BRICKS_IN_ROW = 10,
    BRICKS_IN_COL = 6;


    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Arkanoid", sf::Style::Titlebar | sf::Style::Close);

    sf::Texture pTexture;
    pTexture.loadFromFile("img/paddle.png");
    sf::Texture bTexture;
    bTexture.loadFromFile("img/ball.png");
    sf::Texture lTexture;
    lTexture.loadFromFile("img/lives.png");

    Player player(&pTexture, sf::Vector2f(80,15), PADDLE_SPEED, WINDOW_HEIGHT - 70);
    Ball ball(&bTexture, 8.0f, BALL_SPEED);
    Lives lives(&lTexture, 3);

    Brick brick;
    Brick* bricks= new Brick[BRICKS_IN_ROW*BRICKS_IN_COL];

    for (unsigned int i=0; i<BRICKS_IN_ROW*BRICKS_IN_COL; i++)
    {
        bricks[i].setPosition(sf::Vector2f((i%BRICKS_IN_ROW)* (bricks[i].getSize().x + BETWEEN_ROWS) + (WINDOW_WIDTH - (BRICKS_IN_ROW - 1) * (bricks[i].getSize().x + BETWEEN_ROWS)) / 2.f,
                                            i/BRICKS_IN_ROW * (bricks[i].getSize().y + BETWEEN_COLS) + 120));
        Brick::brickCounter ++;
    }

    sf::RectangleShape bar(sf::Vector2f(window.getSize().x, 40.0f));
    bar.setFillColor(sf::Color(192,192,192));
    bar.setOutlineColor(sf::Color(128,128,128));
    bar.setOutlineThickness(2.0f);


    sf::Font font;
    if (!font.loadFromFile("fonts/font.ttf")) return EXIT_FAILURE;
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(80);
    text.setFillColor(sf::Color::Black);
    sf::FloatRect textRect;

    sf::Text text2;
    text2.setFont(font);
    text2.setCharacterSize(30);
    text2.setFillColor(sf::Color::Black);
    text2.setString("PRESS ENTER TO PLAY AGAIN");
    sf::FloatRect textRect2 = text2.getLocalBounds();
    text2.setOrigin(textRect2.left + textRect2.width / 2.f, textRect2.top + textRect2.height / 2.f);
    text2.setPosition(WINDOW_WIDTH / 2.f, 460);


    sf::Clock clock;
    float deltaT;

    //game loop
    while (window.isOpen())
    {

        //get delta time
        deltaT=clock.restart().asSeconds();

        //event loop
        sf::Event ev;
        while (window.pollEvent(ev))
        {
            switch (ev.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                if (ev.key.code == sf::Keyboard::Up || ev.key.code == sf::Keyboard::W || ev.key.code == sf::Keyboard::Space) ball.stop = false;
                if (ev.key.code == sf::Keyboard::Enter && (Brick::brickCounter == 0 || lives.getLives() == 0))
                {
                    Brick::brickCounter = 0;
                    lives.reset();
                    for (unsigned int i=0; i<BRICKS_IN_ROW*BRICKS_IN_COL; i++)
                    {
                        bricks[i].exists = true;
                        Brick::brickCounter++;
                    }
                }
                break;
            default:
                break;
            }
        } //end of event loop

        //update
        player.update(window, deltaT);
        ball.update(window, deltaT, lives, player);
        lives.update(deltaT);

        //clear window
        window.clear(sf::Color(224,224,224));

        //bricks update and draw
        for (unsigned int i=0; i<BRICKS_IN_ROW*BRICKS_IN_COL; i++)
        {
            if (bricks[i].exists)
            {
                bricks[i].update(ball);
                bricks[i].draw(window);
            }
        }

        //draw
        player.draw(window);
        if (Brick::brickCounter > 0 && lives.getLives() > 0) ball.draw(window);
        else
        {
            ball.stop = true;
            if (Brick::brickCounter == 0) text.setString("YOU WON!");
            else text.setString("YOU LOST");
            textRect = text.getLocalBounds();
            text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
            text.setPosition(sf::Vector2f(WINDOW_WIDTH / 2, bricks[BRICKS_IN_ROW*BRICKS_IN_COL-1].getPosition().y + 80.f));
            text2.setPosition(sf::Vector2f(WINDOW_WIDTH / 2, text.getPosition().y + 60.f));
            window.draw(text); window.draw(text2);
        }
        window.draw(bar);
        lives.draw(window);

        //display window
        window.display();

    } //end of game loop

    delete [] bricks;

    return 0;
}
