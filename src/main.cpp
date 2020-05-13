
#include "Game.h"

int main() {

    short int size = 0, streak = 0;

    while(size < 3) {
        std::cout << "Enter board size: ";
        std::cin >> size;
        if (size < 3) std::cerr << "Board size must be greater than 2\n";
    }
    while(streak < 3 || streak > size) {
        std::cout << "Enter streak length: ";
        std::cin >> streak;
        if (streak < 3 || streak > size) std::cerr << "Streak must be greater than 2 and smaller or equal to size\n";
    }

    Game game(size,streak);

    sf::RenderWindow window(sf::VideoMode(G_WIDTH,G_HEIGHT+B_OFFSET,32),"TIC TAC TOE");

    sf::Event event;

    while(window.isOpen()) {

        while (window.pollEvent(event)) {

            switch (event.type) {

                case sf::Event::Closed: {
                    window.close();
                    break;
                }

                case sf::Event::MouseButtonPressed: {
                    if (event.mouseButton.button == sf::Mouse::Left && game.clicked(game.restartButton, window)) {
                        game.reset();
                    } else
                    if (event.mouseButton.button == sf::Mouse::Left && game.playersTurn) {
                        game.playerTurn(event.mouseButton,window);
                    }
                    break;
                }

                case sf::Event::MouseMoved: {
                    game.highlightHovered(window);
                    break;
                }

                default:
                    break;
            }
        }

        game.refresh(window);

    }
    return 0;
}