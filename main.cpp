#include <SFML/Graphics.hpp>
#include <time.h>

#define WHITE sf::Color::White
#define BLACK sf::Color::Black

int main()
{
    const int CELL_SIZE = 20;
    sf::Vector2f CELL_VECTOR(CELL_SIZE, CELL_SIZE);

    sf::RectangleShape cell;
    cell.setSize(CELL_VECTOR);
    cell.setOutlineThickness(0.5f);

    const int GRID_WIDTH = 30;
    const int GRID_HEIGHT = 30;
    const int LAST_COL = GRID_WIDTH - 1;
    const int LAST_ROW = GRID_HEIGHT - 1;
    const int GRID_N = GRID_WIDTH * GRID_WIDTH;
    
    bool GRID[GRID_N]; // false - empty true - cell
    bool NEXT_GRID[GRID_N];

    const int WIN_WIDTH = GRID_WIDTH * CELL_SIZE;
    const int WIN_HEIGHT = GRID_HEIGHT * CELL_SIZE;

    int delay = 150; // milliseconds

    srand(time(NULL));

    for (int n = 0; n < GRID_N; n++) {
        GRID[n] = rand() % 2;
    }

    sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "Conway's Game of Life");

    bool isPlaying = true;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed: 
                    window.close();
                    break;
                case sf::Event::MouseButtonPressed: {
                        int pressedX = event.mouseButton.x / CELL_SIZE;
                        int pressedY = event.mouseButton.y / CELL_SIZE;
                        if (!isPlaying && pressedX <= GRID_WIDTH * CELL_SIZE && pressedY <= GRID_HEIGHT * CELL_SIZE) {
                            GRID[pressedY * GRID_WIDTH + pressedX] = !GRID[pressedY * GRID_WIDTH + pressedX];
                        }
                    }
                    break;
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::P) {
                        isPlaying = !isPlaying;
                    }
                    break;
            }
        }
        
        window.clear(WHITE);
        for(int y = 0; y < GRID_HEIGHT; y++)
        {
            for (int x = 0; x < GRID_WIDTH; x++) {
                cell.setPosition(x * CELL_SIZE, y * CELL_SIZE);
                cell.setFillColor((GRID[y * GRID_WIDTH + x] == 1) ? BLACK : WHITE);
                cell.setOutlineColor((GRID[y * GRID_WIDTH + x] == 1) ? WHITE : BLACK);
                window.draw(cell);

                if (!isPlaying) {
                    continue;
                }

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
            for (int n = 0; n < GRID_N; n++) {
                GRID[n] = NEXT_GRID[n];
            }
        }
        window.display();
        sf::sleep(sf::milliseconds(delay));
    }
    return 0;
}