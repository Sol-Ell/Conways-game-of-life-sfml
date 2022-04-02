#include <SFML/Graphics.hpp>
#include <time.h>

#define WHITE sf::Color::White
#define BLACK sf::Color::Black

#define EMPTY false;
#define CELL true;

int main()
{
    const int CELL_SIZE = 20;
    const int GRID_WIDTH = 30;
    const int GRID_HEIGHT = 30;
    
    // grid size in pixels
    const int GRID_WIDTH_PIX = GRID_WIDTH * CELL_SIZE;
    const int GRID_HEIGHT_PIX = GRID_HEIGHT * CELL_SIZE;

    const int LAST_COL = GRID_WIDTH - 1;
    const int LAST_ROW = GRID_HEIGHT - 1;
    const int GRID_N = GRID_WIDTH * GRID_WIDTH;

    sf::Vector2f CELL_VECTOR(CELL_SIZE, CELL_SIZE);

    sf::RectangleShape cell;
    cell.setSize(CELL_VECTOR);
    cell.setOutlineThickness(0.5f);

    bool GRID[GRID_N]; // false - empty true - cell
    bool NEXT_GRID[GRID_N];

    // create play and pause buttons
    sf::ConvexShape playBtn;
    playBtn.setPointCount(3);

    playBtn.setPoint(0, sf::Vector2f(0, 50));
    playBtn.setPoint(1, sf::Vector2f(50, 25));
    playBtn.setPoint(2, sf::Vector2f(0, 0));
    
    playBtn.setFillColor(BLACK);
    playBtn.setPosition(sf::Vector2f(GRID_WIDTH_PIX + 5, 20));

    sf::RectangleShape pauseBtn1(sf::Vector2f(20, 50));
    sf::RectangleShape pauseBtn2(sf::Vector2f(20, 50));

    pauseBtn1.setPosition(sf::Vector2f(GRID_WIDTH_PIX + 5, 20));
    pauseBtn2.setPosition(sf::Vector2f(GRID_WIDTH_PIX + 35, 20));
    pauseBtn1.setFillColor(BLACK);
    pauseBtn2.setFillColor(BLACK);

    const int WIN_WIDTH = GRID_WIDTH_PIX + 60; // 60 - toolbar
    const int WIN_HEIGHT = GRID_HEIGHT_PIX;

    int delay = 150; // milliseconds

    srand(time(NULL));
    for (int n = 0; n < GRID_N; n++) 
    {
        GRID[n] = rand() % 2;
    }

    sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "Conway's Game of Life");

    bool isPlaying = true;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type) {
                case sf::Event::Closed: 
                    window.close();
                    break;
                
                case sf::Event::MouseButtonPressed: {
                        int x = event.mouseButton.x;
                        int y = event.mouseButton.y;
                        
                        // check if playBtn or pauseBtn(1 & 2) is pressed
                        if ((isPlaying && playBtn.getGlobalBounds().contains(x, y)) || 
                            (!isPlaying && 
                            (pauseBtn1.getGlobalBounds().contains(x, y) || pauseBtn2.getGlobalBounds().contains(x, y))))
                        {
                            isPlaying = !isPlaying;
                            break;
                        }

                        if (x <= GRID_WIDTH_PIX && y <= GRID_HEIGHT_PIX) {
                            int n = y / CELL_SIZE * GRID_WIDTH + x / CELL_SIZE;
                            if (!isPlaying) 
                                GRID[n] = !GRID[n];
                        }
                    }
                    break;

                case sf::Event::KeyPressed: {
                        switch (event.key.code) {
                            case sf::Keyboard::P: isPlaying = !isPlaying;
                            case sf::Keyboard::C: {
                                    for (int n = 0; n < GRID_N; n++) GRID[n] = EMPTY;
                                }
                                isPlaying = false;
                                break;
                            case sf::Keyboard::R: {
                                for (int n = 0; n < GRID_N; n++) {
                                    GRID[n] = rand() % 2;
                                }
                                isPlaying = false;
                                break;
                            }
                        }
                    }
                    break;
            }
        }
        
        window.clear(WHITE);
        for(int y = 0; y < GRID_HEIGHT; y++) {
            for (int x = 0; x < GRID_WIDTH; x++) {
                cell.setPosition(x * CELL_SIZE, y * CELL_SIZE);
                cell.setFillColor((GRID[y * GRID_WIDTH + x] == 1) ? BLACK : WHITE);
                cell.setOutlineColor((GRID[y * GRID_WIDTH + x] == 1) ? WHITE : BLACK);
                window.draw(cell);

                if (!isPlaying)
                    continue;

                // prepare next grid
                int gridY = y * GRID_WIDTH;
                int gridYPrev = gridY - GRID_WIDTH;
                int gridYNext = gridY + GRID_WIDTH;

                int n = 0;
                if (y != 0) {
                    if (x != 0) {
                        n += GRID[gridYPrev + x - 1];
                    }
                    
                    n += GRID[gridYPrev + x];
                    
                    if (x != LAST_COL) {
                        n += GRID[gridYPrev + x + 1];
                    }
                }
                if (x != 0) {
                    n += GRID[gridY + x - 1];
                }
                if (x != LAST_COL) {
                    n += GRID[gridY + x + 1];
                }
                
                if (y != LAST_ROW) {
                    if (x != 0) {
                        n += GRID[gridYNext + x - 1];
                    }
                    
                    n += GRID[gridYNext + x];
                    
                    if (x != LAST_COL) {
                        n += GRID[gridYNext + x + 1];
                    }
                }
                if (n < 2 || n > 3) {
                    NEXT_GRID[gridY + x] = 0;
                }
                else if (n == 3) {
                    NEXT_GRID[gridY + x] = 1;
                }
                else {
                    NEXT_GRID[gridY + x] = GRID[gridY + x];
                }
            }
        }
        if (isPlaying) {
            for (int n = 0; n < GRID_N; n++)
                GRID[n] = NEXT_GRID[n];
            window.draw(pauseBtn1);
            window.draw(pauseBtn2);
        } 
        else 
            window.draw(playBtn);
        window.display();
        sf::sleep(sf::milliseconds(delay));
    }
    return 0;
}