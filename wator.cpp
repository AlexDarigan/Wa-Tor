// wator.cpp --- 
// 
// Filename: wator.cpp
// Description: 
// Author: David Darigan
// Maintainer: David Darigan
// Created: Fri Nov  9

// Commentary: 
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or (at
// your option) any later version.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
// 
// Code:

#include <chrono>
#include <SFML/Graphics.hpp>
#include <cstdlib> 
#include <iostream>
#include <omp.h>

// 85 * 85 (7225) is total size
const int ROWS = 85;
const int COLUMNS = 85;
const int NUM_FISH = -1;
const int NUM_SHARK = -1;
const int FISH_BREED = 2;
const int SHARK_BREED = 6;
const int SHARK_STARVE = 2;
const int NUM_THREADS = 1;
const int SHARK_ENERGY_GAIN = 1;
const int WindowXSize = 800;
const int WindowYSize = 600;
const int cellXSize = WindowXSize / ROWS;
const int cellYSize = WindowYSize / COLUMNS;
enum Direction { North = 0, East = 1, South = 2, West = 3 };
enum CellType { Ocean, Fish, Shark };
struct Cell;
Cell getCell(int x, int y);
void setCell(int x, int y, Cell cell);

struct Cell {

    CellType celltype = CellType::Ocean;
    int energy = 0;
    int turn = 0;
    sf::Color color = sf::Color::Blue;
    int x;
    int y;
    bool hasMoved = false;
    bool isOcean() { return celltype == CellType::Ocean; };
    bool isFish() { return celltype == CellType::Fish; };
    bool isShark() { return celltype == CellType::Shark; };
    void setMove(bool _hasMoved) { hasMoved = _hasMoved; }
    int getX() { return x; }
    int getY() { return y; }

    
    // void moveFish(int x, int y) {  
    //   int north = (y - 1 + ROWS) % ROWS;
    //   int south = y + 1 % ROWS;
    //   int east = x + 1 % COLUMNS;
    //   int west = (x - 1 + COLUMNS) % COLUMNS;
    //   Cell neighbours[4] = {getCell(x, north), getCell(east, y), getCell(x, south), getCell(west, y)};
    //   int location = rand() % 4;
    //   for(int i = 0; i < 4; i++) {
    //     int x2 = neighbours[location].getX();
    //     int y2 = neighbours[location].getY();
    //     if(getCell(x2, y2).isOcean()) {
    //       turn = turn + 1;
    //       hasMoved = true;
    //       Cell prev;
    //       prev = prev.toOcean();
    //       if(turn == FISH_BREED) {
    //         turn = 0;
    //         prev = prev.toFish();
    //       }
    //       setCell(x2, y2, *this);
    //       setCell(x, y, prev);
    //       break;
    //     }
    //    if(!hasMoved) { location = (location + 1) % 4; }
    //  }
    // }

    // void moveShark(int x, int y) {
    //   int north = (y - 1 + ROWS) % ROWS;
    //   int south = y + 1 % ROWS;
    //   int east = x + 1 % COLUMNS;
    //   int west = (x - 1 + COLUMNS) % COLUMNS;
    //   Cell neighbours[4] = {getCell(x, north), getCell(east, y), getCell(x, south), getCell(west, y)};
    //   int location = rand() % 4;
    
    //   int x2;
    //   int y2;
    //   energy = energy - 1;
    //   for(int i = 0; i < 4; i++) {
    //     int x2 = neighbours[location].getX();
    //     int y2 = neighbours[location].getY();
    //     if(getCell(x2, y2).isFish()) {
    //       turn = turn + 1;
    //       hasMoved = true;
    //       energy += SHARK_ENERGY_GAIN;
    //       Cell prev;
    //       prev = prev.toOcean();
    //       if(turn == SHARK_BREED) {
    //         turn = 0;
    //         prev = prev.toShark();
    //       }
    //       if(this->energy <= 0) {
    //         setCell(x2, y2, this->toOcean());
    //         setCell(x, y, prev);
    //       } else {
    //         setCell(x2, y2, *this);
    //         setCell(x, y, prev);
    //       }
    //       break;
    //     }
    //     if(!hasMoved) { location = (location + 1) % 4;}
    //   }
    //   if(!hasMoved) {
    //     for(int i = 0; i < 4; ++i) {
    //       x2 = neighbours[location].getX();
    //       y2 = neighbours[location].getY();
    //       if(getCell(x2, y2).isOcean()) {
    //         turn++;
    //         hasMoved = true;
    //         Cell prev;
    //         prev = prev.toOcean();
    //         if(turn == SHARK_BREED) {
    //           turn = 0;
    //           prev = prev.toShark();
    //         }
    //         if(this->energy <= 0) {
    //           setCell(x2, y2, this->toOcean());
    //           setCell(x, y, prev);
    //         } else {
    //           setCell(x2, y2, *this);
    //           setCell(x, y, prev);
    //         }
    //         break;
    //       }
    //     if(!hasMoved) { location = (location + 1) % 4;}
    //     }
    //   }
    // }

    Cell toOcean() {
      Cell copy;
      copy.celltype = CellType::Ocean;
      copy.color = sf::Color::Blue;
      return copy;
    }

    Cell toFish() {
      Cell copy;
      copy.celltype = CellType::Fish;
      copy.color = sf::Color::Green;
      copy.turn = 0;
      copy.hasMoved = false;
      return copy;
    }

    Cell toShark() {
      Cell copy;
      copy.celltype = CellType::Shark;
      copy.color = sf::Color::Red;
      copy.energy = SHARK_STARVE;
      copy.turn = 0;
      copy.hasMoved = false;
      return copy;
    }

    void setPosition(int _x, int _y) {
      x = _x;
      y = _y;
    }

    sf::Color getFillColor() { return this->color; }
};

sf::RenderWindow window(sf::VideoMode(WindowXSize,WindowYSize), "SFML Wa-Tor world");
sf::RectangleShape display[ROWS][COLUMNS];
Cell cells[ROWS][COLUMNS];

Cell getCell(int x, int y) {
  return cells[x][y];
}

void setCell(int x, int y, Cell cell) {
  cells[x][y] = cell;
  cells[x][y].setPosition(x, y);
}

  void moveFish(int x, int y) { 
      Cell fish = cells[x][y];
      int north = (y - 1 + ROWS) % ROWS;
      int south = y + 1 % ROWS;
      int east = x + 1 % COLUMNS;
      int west = (x - 1 + COLUMNS) % COLUMNS;
      Cell neighbours[4] = {getCell(x, north), getCell(east, y), getCell(x, south), getCell(west, y)};
      int location = rand() % 4;
      for(int i = 0; i < 4; i++) {
        int x2 = neighbours[location].getX();
        int y2 = neighbours[location].getY();
        if(getCell(x2, y2).isOcean()) {
          fish.turn = fish.turn + 1;
          fish.hasMoved = true;
          Cell prev;
          prev = prev.toOcean();
          if(fish.turn == FISH_BREED) {
            fish.turn = 0;
            prev = prev.toFish();
          }
          setCell(x2, y2, fish);
          setCell(x, y, prev);
          break;
        }
       if(!fish.hasMoved) { location = (location + 1) % 4; }
     }
    }

void moveShark(int x, int y) {
      Cell shark = cells[x][y];
      int north = (y - 1 + ROWS) % ROWS;
      int south = y + 1 % ROWS;
      int east = x + 1 % COLUMNS;
      int west = (x - 1 + COLUMNS) % COLUMNS;
      Cell neighbours[4] = {getCell(x, north), getCell(east, y), getCell(x, south), getCell(west, y)};
      int location = rand() % 4;
    
      int x2;
      int y2;
      shark.energy = shark.energy - 1;
      for(int i = 0; i < 4; i++) {
        int x2 = neighbours[location].getX();
        int y2 = neighbours[location].getY();
        if(getCell(x2, y2).isFish()) {
          shark.turn = shark.turn + 1;
          shark.hasMoved = true;
          shark.energy += SHARK_ENERGY_GAIN;
          Cell prev;
          prev = prev.toOcean();
          if(shark.turn == SHARK_BREED) {
            shark.turn = 0;
            prev = prev.toShark();
          }
          if(shark.energy <= 0) {
            setCell(x2, y2, shark.toOcean());
            setCell(x, y, prev);
          } else {
            setCell(x2, y2, shark);
            setCell(x, y, prev);
          }
          break;
        }
        if(!shark.hasMoved) { location = (location + 1) % 4;}
      }
      if(!shark.hasMoved) {
        for(int i = 0; i < 4; ++i) {
          x2 = neighbours[location].getX();
          y2 = neighbours[location].getY();
          if(getCell(x2, y2).isOcean()) {
            shark.turn++;
            shark.hasMoved = true;
            Cell prev;
            prev = prev.toOcean();
            if(shark.turn == SHARK_BREED) {
              shark.turn = 0;
              prev = prev.toShark();
            }
            if(shark.energy <= 0) {
              setCell(x2, y2, shark.toOcean());
              setCell(x, y, prev);
            } else {
              setCell(x2, y2, shark);
              setCell(x, y, prev);
            }
            break;
          }
        if(!shark.hasMoved) { location = (location + 1) % 4;}
        }
      }
    }

void countFish() {
    int fishes = 0;
    int shark = 0;
    for(int y = 0; y < ROWS; ++y) {
      for(int x = 0; x < COLUMNS; ++x) {
        if(cells[x][y].isFish()) {
          fishes++;
        } else if(cells[x][y].isShark()) {
          shark++;
        }
      }
    }
    printf("Fishes: %d\nSharks: %d\n", fishes, shark);
  }


void draw() {
  window.clear(sf::Color::Black);
  for(int i = 0; i < ROWS; ++i){
    for(int k = 0; k < COLUMNS; ++k){
      window.draw(display[i][k]);
    }
  }
  sf::sleep(sf::seconds(1));
  window.display();
}

void clearMoves() {
  for(int y = 0; y < ROWS; ++y) {
    for(int x = 0; x < COLUMNS; ++x) {
      cells[x][y].hasMoved = false;
    }
  }
}

void setColors() {
  for(int y = 0; y < ROWS; ++y) {
    for(int x = 0; x < COLUMNS; ++x) {
      display[x][y].setFillColor(cells[x][y].getFillColor());
    }
  }
}

void poll() {
  sf::Event event;
  while (window.pollEvent(event))
  {
      if (event.type == sf::Event::Closed)
          window.close();
  }
}

void initialize() {
  for(int i=0;i<ROWS;++i){
    for(int k=0;k<COLUMNS;++k){
      display[i][k].setSize(sf::Vector2f(cellXSize,cellYSize));
      display[i][k].setPosition(i*cellXSize,k*cellYSize);
      int id=i*1-+k;
      if(id%9==0) {
        Cell c;
        setCell(i, k, c.toShark());
      }
      else if (id%6==0) { 
        Cell c;
        setCell(i,k, c.toFish());
      }
      else {
        Cell c;
        setCell(i, k, c.toOcean());
      }
    }
  }
}

int main()
{
  srand(0);
  omp_set_num_threads(NUM_THREADS);
  initialize();
  setColors();
  draw();
  while (window.isOpen())
  {
    poll();
    
    
    auto start = std::chrono::steady_clock::now();
    //Move Fish
    #pragma omp parallel for
    for(int y = 0; y < ROWS; ++y) {
      for(int x = 0; x < COLUMNS; ++x) {
        if(cells[x][y].isFish() && !cells[x][y].hasMoved) {
          //cells[x][y].moveFish(x, y);
          moveFish(x, y);
      }
    }
    
    // Move Shark
    #pragma omp parallel for
    for(int y = 0; y < ROWS; ++y) {
      for(int x = 0; x < COLUMNS; ++x) {
        if(cells[x][y].isShark() && !cells[x][y].hasMoved) {
          moveShark(x, y);
        }
      }
    }
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    printf("Duration: %ld\n", duration);
  }


   
    clearMoves();
    setColors();
    draw();
    countFish();
  }
    
    return 0;
}