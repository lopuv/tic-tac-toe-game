#include <iostream>
#include "TicTacToe.h"

void displayMenu() {
    std::cout << "===== Tic-Tac-Toe Menu =====" << std::endl;
    std::cout << "1. Two Players" << std::endl;
    std::cout << "2. One Player (against AI)" << std::endl;
    std::cout << "Enter your choice (1 or 2): ";
}

int main() {
    int choice;
    do {
        displayMenu();
        std::cin >> choice;
    } while (choice != 1 && choice != 2);

    sf::RenderWindow window(sf::VideoMode(COLS * CELL_SIZE, ROWS * CELL_SIZE), "Tic-Tac-Toe");
    window.setFramerateLimit(60);

    TicTacToe game(choice == 1);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else {
                game.handleEvent(event);
            }
        }

        game.draw(window);

        if (game.getGameStatus() != GameStatus::InProgress) {
            sf::sleep(sf::seconds(2)); // Pause for 2 seconds before closing the window
            window.close();
        }
    }

    return 0;
}