#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>

const int NUM_ROWS = 9;    // ʳ������ ������
const int ROW_LENGTH = 12; // ������� ������� ������
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int BALL_SIZE = 40;

sf::Color getColor(int value) {
    switch (value) {
    case 1: return sf::Color::Red;
    case 2: return sf::Color::Green;
    case 3: return sf::Color::Blue;
    case 4: return sf::Color::Yellow;
    case 5: return sf::Color::Magenta;
    default: return sf::Color::White;
    }
}

int getRandomColorValue() {
    return rand() % 5 + 1;
}

// ������� ��� ��������� �������� ������ ������ ������� �� ���������� �� ��������
void removeMatchingBalls(int arrays[NUM_ROWS][ROW_LENGTH]) {
    bool toRemove[NUM_ROWS][ROW_LENGTH] = { false };

    // �������� �������������� ���
    for (int i = 0; i < NUM_ROWS; ++i) {
        int count = 1;
        for (int j = 1; j < ROW_LENGTH; ++j) {
            if (arrays[i][j] != 0 && arrays[i][j] == arrays[i][j - 1]) {
                ++count;
                if (count >= 3) {
                    for (int k = j; k > j - count; --k) {
                        toRemove[i][k] = true;
                    }
                }
            }
            else {
                count = 1;
            }
        }
    }

    // �������� ������������ ���
    for (int j = 0; j < ROW_LENGTH; ++j) {
        int count = 1;
        for (int i = 1; i < NUM_ROWS; ++i) {
            if (arrays[i][j] != 0 && arrays[i][j] == arrays[i - 1][j]) {
                ++count;
                if (count >= 3) {
                    for (int k = i; k > i - count; --k) {
                        toRemove[k][j] = true;
                    }
                }
            }
            else {
                count = 1;
            }
        }
    }

    // �������� ����������� ���
    for (int i = 0; i < NUM_ROWS - 2; ++i) {
        for (int j = 0; j < ROW_LENGTH - 2; ++j) {
            if (arrays[i][j] != 0) {
                if (arrays[i][j] == arrays[i + 1][j + 1] && arrays[i][j] == arrays[i + 2][j + 2]) {
                    toRemove[i][j] = toRemove[i + 1][j + 1] = toRemove[i + 2][j + 2] = true;
                }
                if (j >= 2 && arrays[i][j] == arrays[i + 1][j - 1] && arrays[i][j] == arrays[i + 2][j - 2]) {
                    toRemove[i][j] = toRemove[i + 1][j - 1] = toRemove[i + 2][j - 2] = true;
                }
            }
        }
    }

    // ��������� ������
    for (int i = 0; i < NUM_ROWS; ++i) {
        for (int j = 0; j < ROW_LENGTH; ++j) {
            if (toRemove[i][j]) {
                arrays[i][j] = 0;
            }
        }
    }
}

int main() {
    srand(static_cast<unsigned>(time(0)));

    // ��������� ������ � 9 ��������
    int arrays[NUM_ROWS][ROW_LENGTH];

    // ���������� ������ ����� ������� ������� ������ ����������� ������� �� 1 �� 5
    for (int i = 0; i < NUM_ROWS; ++i) {
        for (int j = 0; j < ROW_LENGTH; ++j) {
            if (j < 3) {
                arrays[i][j] = getRandomColorValue(); // ��������� ����������� ����� � ������� 1-5
            }
            else {
                arrays[i][j] = 0; // �������� ����� ������� ������
            }
        }
    }

    // ��������� ����
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Color Balls");

    // ��������� ������ ����� ���������
    int bottomBallColorValue = getRandomColorValue();
    sf::CircleShape bottomBall(BALL_SIZE / 2);
    bottomBall.setFillColor(getColor(bottomBallColorValue));
    bottomBall.setPosition((WINDOW_WIDTH - BALL_SIZE) / 2, WINDOW_HEIGHT - BALL_SIZE - 10);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    // �������� ������� ����
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    // ���������� ���������� ��������
                    int closestCol = (mousePos.x - (WINDOW_WIDTH - NUM_ROWS * (BALL_SIZE + 10)) / 2) / (BALL_SIZE + 10);
                    if (closestCol < 0) closestCol = 0;
                    if (closestCol >= NUM_ROWS) closestCol = NUM_ROWS - 1;

                    // ��������� ���� ������ �� ������
                    for (int j = 0; j < ROW_LENGTH; ++j) {
                        if (arrays[closestCol][j] == 0) {
                            // ���������, �� � �������� ������ ����
                            bool canPlace = true;
                            for (int k = j + 1; k < ROW_LENGTH; ++k) {
                                if (arrays[closestCol][k] != 0) {
                                    canPlace = false;
                                    break;
                                }
                            }
                            if (canPlace) {
                                arrays[closestCol][j] = bottomBallColorValue;
                                break;
                            }
                        }
                    }

                    // ��������� �������� ������ ������ �������
                    removeMatchingBalls(arrays);

                    // ������ ���� ������ �����
                    bottomBallColorValue = getRandomColorValue();
                    bottomBall.setFillColor(getColor(bottomBallColorValue));
                }
            }
        }

        window.clear(sf::Color::White);

        // ��������� ������
        for (int i = 0; i < NUM_ROWS; ++i) {
            for (int j = 0; j < ROW_LENGTH; ++j) {
                if (arrays[i][j] != 0) {
                    sf::CircleShape ball(BALL_SIZE / 2);
                    ball.setFillColor(getColor(arrays[i][j]));

                    // ����������� ������� ������
                    float x = (WINDOW_WIDTH - NUM_ROWS * (BALL_SIZE + 10)) / 2 + i * (BALL_SIZE + 10);
                    float y = j * (BALL_SIZE + 10) + 50;
                    ball.setPosition(x, y);

                    window.draw(ball);
                }
            }
        }

        // ��������� ������ �����
        window.draw(bottomBall);

        window.display();
    }

    return 0;
}
