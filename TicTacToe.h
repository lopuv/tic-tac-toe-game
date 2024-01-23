#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>

const int ROWS = 3;
const int COLS = 3;
const int CELL_SIZE = 100;

enum class Player { None, X, O };
enum class GameStatus { InProgress, PlayerWins, Draw };

class TicTacToe {
public:
    TicTacToe(bool twoPlayers) : currentPlayer(Player::X), gameStatus(GameStatus::InProgress), twoPlayers(twoPlayers) {
        board.resize(ROWS, std::vector<Player>(COLS, Player::None));
        if (!twoPlayers) {
            std::srand(static_cast<unsigned>(std::time(nullptr)));
        }
    }

    void handleEvent(sf::Event event) {
        if (gameStatus != GameStatus::InProgress) {
            return; // Don't handle events if the game has ended
        }

        if (twoPlayers || currentPlayer == Player::X) {
            if (event.type == sf::Event::MouseButtonPressed) {
                int mouseX = event.mouseButton.x;
                int mouseY = event.mouseButton.y;

                int col = mouseX / CELL_SIZE;
                int row = mouseY / CELL_SIZE;

                if (col >= 0 && col < COLS && row >= 0 && row < ROWS && board[row][col] == Player::None) {
                    makeMove(row, col);
                }
            }
        }
        else {
            // AI's turn
            if (currentPlayer == Player::O) {
                makeAIMove();
            }
        }
    }

    void draw(sf::RenderWindow& window) {
        window.clear();

        // Draw the grid
        for (int i = 1; i < ROWS; ++i) {
            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(0, i * CELL_SIZE)),
                sf::Vertex(sf::Vector2f(COLS * CELL_SIZE, i * CELL_SIZE))
            };
            window.draw(line, 2, sf::Lines);
        }

        for (int j = 1; j < COLS; ++j) {
            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(j * CELL_SIZE, 0)),
                sf::Vertex(sf::Vector2f(j * CELL_SIZE, ROWS * CELL_SIZE))
            };
            window.draw(line, 2, sf::Lines);
        }

        // Draw X and O
        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < COLS; ++j) {
                if (board[i][j] == Player::X) {
                    drawX(window, i, j);
                }
                else if (board[i][j] == Player::O) {
                    drawO(window, i, j);
                }
            }
        }

        window.display();
    }

    GameStatus getGameStatus() const {
        return gameStatus;
    }

private:

    std::vector<std::vector<Player>> board;
    Player currentPlayer;
    GameStatus gameStatus;
    bool twoPlayers;

    void drawX(sf::RenderWindow& window, int row, int col) {
        sf::VertexArray lines(sf::Lines, 4);
        lines[0].position = sf::Vector2f(col * CELL_SIZE, row * CELL_SIZE);
        lines[1].position = sf::Vector2f((col + 1) * CELL_SIZE, (row + 1) * CELL_SIZE);
        lines[2].position = sf::Vector2f((col + 1) * CELL_SIZE, row * CELL_SIZE);
        lines[3].position = sf::Vector2f(col * CELL_SIZE, (row + 1) * CELL_SIZE);

        for (int i = 0; i < 4; ++i) {
            lines[i].color = sf::Color::Red;
        }

        window.draw(lines);
    }

    void drawO(sf::RenderWindow& window, int row, int col) {
        sf::CircleShape circle(CELL_SIZE / 2 - 10);
        circle.setOutlineThickness(10);
        circle.setOutlineColor(sf::Color::Blue);
        circle.setPosition(col * CELL_SIZE + 10, row * CELL_SIZE + 10);

        window.draw(circle);
    }

    void makeMove(int row, int col) {
        board[row][col] = currentPlayer;

        if (checkWin(row, col)) {
            std::cout << "Player " << static_cast<char>(currentPlayer) << " wins!" << std::endl;
            gameStatus = GameStatus::PlayerWins;
        }
        else if (checkDraw()) {
            std::cout << "It's a draw!" << std::endl;
            gameStatus = GameStatus::Draw;
        }
        else {
            currentPlayer = (currentPlayer == Player::X) ? Player::O : Player::X;
        }
    }

    void makeAIMove() {
        // Simple AI: randomly choose an empty cell
        std::vector<std::pair<int, int>> emptyCells;

        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < COLS; ++j) {
                if (board[i][j] == Player::None) {
                    emptyCells.emplace_back(i, j);
                }
            }
        }

        if (!emptyCells.empty()) {
            int index = std::rand() % emptyCells.size();
            makeMove(emptyCells[index].first, emptyCells[index].second);
        }
    }

    bool checkWin(int row, int col) {
        // Check row
        if (board[row][0] == currentPlayer && board[row][1] == currentPlayer && board[row][2] == currentPlayer) {
            return true;
        }
        // Check column
        if (board[0][col] == currentPlayer && board[1][col] == currentPlayer && board[2][col] == currentPlayer) {
            return true;
        }
        // Check diagonals
        if ((row == col && board[0][0] == currentPlayer && board[1][1] == currentPlayer && board[2][2] == currentPlayer) ||
            (row + col == 2 && board[0][2] == currentPlayer && board[1][1] == currentPlayer && board[2][0] == currentPlayer)) {
            return true;
        }
        return false;
    }

    bool checkDraw() {
        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < COLS; ++j) {
                if (board[i][j] == Player::None) {
                    return false; // There is an empty cell, the game is not a draw yet
                }
            }
        }
        return true; // All cells are filled, it's a draw
    }
};


