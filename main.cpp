#include <SFML/Graphics.hpp>
#include <time.h>

#define WHITE sf::Color::White
#define BLACK sf::Color::Black

int main()
{
    const int CELL_SIZE = 10;
    sf::Vector2f CELL_VECTOR(CELL_SIZE, CELL_SIZE);
    const int GRID_WIDTH = 50;
    const int GRID_HEIGHT = 50;
    const int LAST_COL = GRID_WIDTH - 1;
    const int LAST_ROW = GRID_HEIGHT - 1;
    const int GRID_N = GRID_WIDTH * GRID_WIDTH;
    int GRID[GRID_N];
    int NEXT_GRID[GRID_N];

    const int WIN_WIDTH = GRID_WIDTH * CELL_SIZE;
    const int WIN_HEIGHT = GRID_HEIGHT * CELL_SIZE;

    int delay = 150; // milliseconds

    srand(time(NULL));

    for (int n = 0; n < GRID_N; n++) {
        GRID[n] = rand() % 2;
    }

    sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "Conway's Game of Life Implementation using C++ and SFML");
    window.setFramerateLimit(50);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                {
                    window.close();
                    return 0;
                }
            }
        }
        
        window.clear(WHITE);

        for(int y = 0; y < GRID_HEIGHT; y++)
        {
            for (int x = 0; x < GRID_WIDTH; x++) {
                sf::RectangleShape cell;
                cell.setSize(CELL_VECTOR);
                cell.setPosition(x * CELL_SIZE, y * CELL_SIZE);
                cell.setFillColor((GRID[y * GRID_WIDTH + x] == 1) ? BLACK : WHITE);
                window.draw(cell);

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
        for (int n = 0; n < GRID_N; n++) {
            GRID[n] = NEXT_GRID[n];
        }
        window.display();
        sf::sleep(sf::milliseconds(delay));
    }
    return 0;
}
/*
#include <stddef.h>
#include <stdbool.h>

void evolve(int GRID_HEIGHT, int GRID_WIDTH, bool GRID[GRID_HEIGHT][GRID_WIDTH], bool NEXT_GRID[GRID_HEIGHT][GRID_WIDTH])
{
    int LAST_ROW = GRID_HEIGHT-1, LAST_COL = GRID_WIDTH-1;
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            int n = 0;
            if (y != 0) {
                if (x != 0) 
                    n += GRID[y-1][x-1];
                
                n += GRID[y-1][x];
                
                if (x != LAST_COL) 
                    n += GRID[y-1][x+1];
            }
            if (x != 0) 
                n += GRID[y][x-1];
            if (x != LAST_COL)
                n += GRID[y][x+1];
            
            if (y != LAST_ROW) {
                if (x != 0) 
                    n += GRID[y+1][x-1];
                
                n += GRID[y+1][x];
                
                if (x != LAST_COL) 
                    n += GRID[y+1][x+1];
            }
            if (n < 2 || n > 3) 
                NEXT_GRID[y][x] = 0;
            else if (n == 3)
                NEXT_GRID[y][x] = 1;
            else 
                NEXT_GRID[y][x] = GRID[y][x];
        }
    }
}*/