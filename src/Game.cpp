
#include "Game.h"

Game::Game(int rows, int tics) {

    grid  = gridMatrix(rows,std::vector<sf::RectangleShape>(rows));

    state = new int*[rows];
    for (int i = 0; i < rows; ++i) {
        state[i] = new int[rows];
    }

    size = rows;
    streak = tics;
    playersTurn = true;

    if (!(     X.loadFromFile("X.png") &&
               O.loadFromFile("O.png") &&
               restart.loadFromFile("restart.png") )) {
        std::cerr << "Could not load textures\n";
    }

    restartButton = sf::RectangleShape(sf::Vector2f(150.f,50.f));
    restartButton.setPosition(30,G_HEIGHT+(B_OFFSET/4));
    restartButton.setTexture(&restart);
    restartButton.setTextureRect(sf::IntRect(0, 0, 150, 50));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < rows; ++j) {
            grid[i][j] = sf::RectangleShape(sf::Vector2f(G_WIDTH/rows,G_HEIGHT/rows));
            grid[i][j].setPosition(i*G_WIDTH/rows,j*G_HEIGHT/rows);
            grid[i][j].setOutlineThickness(OUTLINE_THICKNESS);
            grid[i][j].setOutlineColor(OUTLINE);
            grid[i][j].setFillColor(FILL);

            state[i][j] = 0;
        }
    }

    if (!font.loadFromFile("Roboto-Medium.ttf")) {
        std::cerr << "Could not load font\n";
    } else {
        message.setFont(font);
        message.setString("IT'S YOUR TURN");
        message.setCharacterSize(40);
        message.setFillColor(sf::Color::Black);
        message.setStyle(sf::Text::Bold);
        message.setOrigin(message.getGlobalBounds().width/2,message.getGlobalBounds().height/2);
        message.setPosition(G_WIDTH/2,G_HEIGHT+(B_OFFSET/3));
    }
}

Game::~Game() {
    for (int i = 0; i < size; ++i) {
        delete state[i];
    }
    delete state;
}

void Game::drawGrid(sf::RenderWindow& window) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            window.draw(grid[i][j]);
        }
    }
}

void Game::setMessage(std::string s) {
    message.setString(s);
    message.setOrigin(message.getGlobalBounds().width/2,message.getGlobalBounds().height/2);
    message.setPosition(G_WIDTH/2,G_HEIGHT+(B_OFFSET/3));
}

void Game::highlightHovered(sf::RenderWindow& window) {

    int i,j;
    sf::Vector2i mouse = sf::Mouse::getPosition(window);

    for (i = 0; i < size; ++i) {
        for (j = 0; j < size; ++j) {
            if (state[i][j] == 0) {
                grid[i][j].setFillColor(FILL);
            }
        }
    }

    i = mouse.x/(G_WIDTH/size);
    if (i >= size) return;

    j = mouse.y/(G_HEIGHT/size);
    if (j >= size) return;

    if (state[i][j] == 0) {
        grid[i][j].setFillColor(HOVER);
    }
}

void Game::playerTurn(sf::Event::MouseButtonEvent mouse,sf::RenderWindow& window) {

    int i = mouse.x/(G_WIDTH/size);
    if (i >= size) return;

    int j = mouse.y/(G_HEIGHT/size);
    if (j >= size) return;

    if (state[i][j] == 0 && playersTurn) {

        state[i][j] = 1;
        playersTurn = false;

        grid[i][j].setTexture(&X);
        grid[i][j].setTextureRect(sf::IntRect(0, 0, 360, 360));

        if(isOver(state,1)) {
            setMessage("YOU WIN!");
            refresh(window);
        } else if (isDraw(state)) {
            setMessage("IT'S A DRAW");
            refresh(window);
        } else {
            setMessage("IT'S AI TURN");
            refresh(window);
            AITurn();

        }
    }
}

void Game::AITurn() {

    Move move;
    int  bestScore = INT32_MAX;
    int  depth = calculateOptimalDepth();

    int** s = state;

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (s[i][j] == 0) {
                s[i][j] = -1;
                int currentScore = minimax(s, 1, -INT32_MAX, INT32_MAX, depth);
                s[i][j] = 0;

                if (currentScore < bestScore) {
                    bestScore = currentScore;
                    move.x = i;
                    move.y = j;
                }
            }
        }
    }

    state[move.x][move.y] = -1;
    grid[move.x][move.y].setTexture(&O);
    grid[move.x][move.y].setTextureRect(sf::IntRect(0, 0, 360, 360));

    if (isOver(state,-1)) {
        setMessage("AI WINS!");
    } else if (isDraw(state)) {
        setMessage("IT'S A DRAW");
    } else {
        setMessage("IT'S YOUR TURN");
        playersTurn = true;
    }
}

int Game::calculateOptimalDepth() {

    int spots = emptySpots();

    if (spots < 9) {
        return 8;
    } else {
        int depth = 0;
        long t = TRESHOLD;

        while (t > 0 && spots > 1) {
            t /= (spots--);
            depth++;
        }

        return depth;
    }
}

int Game::emptySpots() {

    int count = 0;

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (state[i][j] == 0) {
                count++;
            }
        }
    }

    return count;
}

bool Game::isOver(int** s, int player) {

    int currentStreak = 0;
    int bound = size-(streak-1);

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {

            if (s[i][j] == player && !(i >= bound && j >= bound)) {
                currentStreak = 1;

                for (int k = 1; k < streak; ++k) {
                    if (i+(streak-1) < size) {
                        //sprawdz w prawo
                        if (s[i+k][j] == player) currentStreak++;
                    }
                }
                if (currentStreak == streak) {
                    return true;
                } else {
                    currentStreak = 1;
                }

                for (int k = 1; k < streak; ++k) {
                    if (i+(streak-1) < size && j+(streak-1) < size) {
                        //sprawdz w dół prawo
                        if (s[i+k][j+k] == player) currentStreak++;
                    }
                }
                if (currentStreak == streak) {
                    return true;
                } else {
                    currentStreak = 1;
                }

                for (int k = 1; k < streak; ++k) {
                    if (j+(streak-1) < size) {
                        //sprawdz w dół
                        if (s[i][j+k] == player) currentStreak++;
                    }
                }
                if (currentStreak == streak) {
                    return true;
                } else {
                    currentStreak = 1;
                }

                for (int k = 1; k < streak; ++k) {
                    if (i-(streak-1) < size && i-(streak-1) >= 0 && j+(streak-1) < size) {
                        //sprawdz w lewo dół
                        if (s[i-k][j+k] == player) currentStreak++;
                    }
                }
                if (currentStreak == streak) return true;
            }
        }
    }
    return false;
}

bool Game::isDraw(int** s) {

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {

            if (s[i][j] == 0) {
                return false;
            }
        }
    }

    return true;
}

void Game::refresh(sf::RenderWindow& window) {
    window.clear(OUTLINE);
    drawGrid(window);
    window.draw(message);
    window.draw(restartButton);
    window.display();
}

bool Game::clicked(sf::RectangleShape& rect, sf::RenderWindow& window) {
    return rect.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
}

void Game::reset() {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            grid[i][j].setTexture(nullptr);
            grid[i][j].setFillColor(FILL);
            state[i][j] = 0;
        }
    }
    playersTurn = true;
}