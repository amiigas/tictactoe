
#ifndef TICTACTOE_GAME_H
#define TICTACTOE_GAME_H

#include <iostream>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

#define G_HEIGHT 1080
#define G_WIDTH  1080
#define B_OFFSET  100

#define TRESHOLD 2000000

#define FILL    sf::Color(230,230,230)
#define OUTLINE sf::Color::White
#define HOVER   sf::Color(200,200,200)
#define OUTLINE_THICKNESS -5

typedef std::vector<std::vector<sf::RectangleShape>> gridMatrix;

struct Move {
    int x;
    int y;
};

class Game {
public:

    gridMatrix         grid;
    int**              state;
    int                size;
    int                streak;
    bool               playersTurn;
    sf::Text           message;
    sf::Font           font;
    sf::Texture        X;
    sf::Texture        O;
    sf::RectangleShape restartButton;
    sf::Texture        restart;

     Game(int rows, int tics);
    ~Game();

    void drawGrid(sf::RenderWindow& window);
    void highlightHovered(sf::RenderWindow& window);
    void setMessage(std::string s);
    void refresh(sf::RenderWindow& window);
    bool clicked(sf::RectangleShape& rect, sf::RenderWindow& window);
    void reset();

    void playerTurn(sf::Event::MouseButtonEvent mouse,sf::RenderWindow& window);
    void AITurn();

    bool isOver(int** s, int player);
    bool isDraw(int** s);

    int  minimax(int** s, int player, int alpha, int beta, int depth);
    int  heuristicEvaluation(int** s, int player);
    int  calculateOptimalDepth();
    int  emptySpots();
};

#endif
