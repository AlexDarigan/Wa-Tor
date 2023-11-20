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

const int ROWS = 10;
const int COLUMNS = 10;
const int NUM_FISH = -1;
const int NUM_SHARK = -1;
const int FISH_BREED = 3;
const int SHARK_BREED = 3;
const int SHARK_STARVE = 3;
const int NUM_THREADS = 1;
const int WindowXSize = 800;
const int WindowYSize = 600;
const int cellXSize = WindowXSize / ROWS;
const int cellYSize = WindowYSize / COLUMNS;
enum Direction { North = 0, East = 1, South = 2, West = 3 };
enum CellType { Ocean, Fish, Shark };

struct Cell {
  CellType celltype = CellType::Ocean;
  bool hasMoved = false;
  int energy = SHARK_STARVE;
  int turn = 0;

  bool isOcean() { return celltype == CellType::Ocean; };
  bool isFish() { return celltype == CellType::Fish; };
  bool isShark() { return celltype == CellType::Shark; };

  void toFish() {
    celltype = CellType::Fish;
    turn = 0;
    hasMoved = false;
  }

  void toShark() {
    celltype = CellType::Shark;
    energy = 0;
    turn = 0;
    hasMoved = false;
  }
};

sf::RenderWindow window(sf::VideoMode(WindowXSize,WindowYSize), "SFML Wa-Tor world");
sf::RectangleShape display[ROWS][COLUMNS];
Cell cells[ROWS][COLUMNS];


void countFish() {
    // Not working properly, too many fishes
    int fishes = 0;
    int shark = 0;
    for(int y = 0; y < ROWS; ++y) {
      for(int x = 0; x < COLUMNS; ++x) {
        if(cells[x][y].celltype == CellType::Fish) {
          fishes++;
        } else if(cells[x][y].celltype == CellType::Shark) {
          shark++;
        }
      }
    }
    printf("Fishes: %d\nSharks: %d\n", fishes, shark);
  }


void draw() {
   window.clear(sf::Color::Black);
    for(int i=0;i<ROWS;++i){
      for(int k=0;k<COLUMNS;++k){
        window.draw(display[i][k]);
      }
    }
      sf::sleep(sf::seconds(2));
      window.display();
}

void setColors() {
  for(int y = 0; y < ROWS; ++y) {
    for(int x = 0; x < COLUMNS; ++x) {
      switch(cells[x][y].celltype) {
        case CellType::Ocean:
          display[x][y].setFillColor(sf::Color::Blue);
          break;
        case CellType::Fish:
          display[x][y].setFillColor(sf::Color::Green);
          break;
        case CellType::Shark:
          display[x][y].setFillColor(sf::Color::Red);
          break;
        default:
          break;
        
      }
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
      //if(false)
      if(id%6==0) {
     // if(i == 7 && k == 7) {
        //display[i][k].setFillColor(sf::Color::Red);
        cells[i][k].celltype = CellType::Shark;
      }
      else if (id%2==0) { 
      //else if(i == 5 && k == 6) {  
        //display[i][k].setFillColor(sf::Color::Green);
        cells[i][k].celltype = CellType::Fish;
      }
      else {
        //display[i][k].setFillColor(sf::Color::Green);
        cells[i][k].celltype = CellType::Ocean;
      }
    }
  }
}

bool moveFish(int x, int y, int x2, int y2) {
  if(cells[x2][y2].celltype == CellType::Ocean) {
    cells[x][y].turn++;
    cells[x2][y2] = cells[x][y];
    cells[x2][y2].hasMoved = true;
    if(cells[x2][y2].turn == FISH_BREED) {
      cells[x][y].toFish();
      cells[x2][y2].turn = 0;
    } else {
      cells[x][y].celltype = CellType::Ocean;
    }
    return true;
  }
  return false;
}

bool moveShark(int x, int y, int x2, int y2, bool justMove = false) {
  if(cells[x2][y2].isFish()) {
    cells[x2][y2] = cells[x][y];
    cells[x2][y2].hasMoved = true;
    cells[x2][y2].energy--;
    cells[x][y].celltype = CellType::Ocean;
    if(cells[x2][y2].turn == SHARK_BREED) {
      cells[x][y].toShark();
      cells[x2][y2].turn = 0;
    } else {
      cells[x][y].celltype = CellType::Ocean;
    }
    return true;
  }
  return false;
}


void count() {
  for(int i=0;i<ROWS;++i) {
    for(int k=0;k<COLUMNS;++k){
      printf("%d\n", cells[i][k].celltype);
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

    
    // Move Fish
    for(int y = 0; y < ROWS; ++y) {
      for(int x = 0; x < COLUMNS; ++x) {
        if(cells[x][y].isFish() && !cells[x][y].hasMoved) {
        bool hasMoved = false;
        int direction = rand() % 4;
        for(int i = 0; i < 4; ++i) {
          switch (direction)
          {
            case North: {
              int north = (y - 1 + ROWS) % ROWS;
              hasMoved = moveFish(x, y, x, north);
            }
              break;
            case East: {
              int east = (x + 1) % COLUMNS;
              hasMoved = moveFish(x, y, east, y);
            }
              break;
            case South: {
              int south = (y + 1) % ROWS;
              hasMoved = moveFish(x, y, x, south);
            }
              break;
            case West: {
              int west = (x - 1 + COLUMNS) % COLUMNS;
              hasMoved = moveFish(x, y, west, y);
            }
              break;
            default:
              break;
          }
        }
        direction = (direction + 1) % 4;
        if(hasMoved) { break; }
      }
    }
  }

    // Move Shark
    for(int y = 0; y < ROWS; ++y) {
      for(int x = 0; x < COLUMNS; ++x) {
        if(cells[x][y].isShark() && !cells[x][y].hasMoved) {
          bool hasMoved = false;
          int lookDirection = rand() % 4;
          cells[x][y].energy++;
          for(int i = 0; i < 4; ++i) {
            switch (lookDirection) {
              case North: {
                  int north = (y - 1 + ROWS) % ROWS;
                  hasMoved = moveShark(x, y, x, north);
                }
                break;
              case East: {
                  int east = (x + 1) % COLUMNS;
                  hasMoved = moveShark(x, y, east, y);
                }
                break;
              case South: {
                  int south = (y + 1) % ROWS;
                  hasMoved = moveShark(x, y, x, south);                
                }
                break;
              case West: {
                int west = (x - 1 + COLUMNS) % COLUMNS;
                hasMoved = moveShark(x, y, west, y);
                }
                break;
              default:
                break;
            }
            lookDirection = (lookDirection + 1) % 4;
            if(hasMoved) { 
              break; 
            }
          }
          if(cells[x][y].isShark() && cells[x][y].energy == SHARK_STARVE) {
            cells[x][y].celltype = Ocean;
          }
      }
    }
  }

    for(int y = 0; y < ROWS; ++y) {
      for(int x = 0; x < COLUMNS; ++x) {
        cells[x][y].hasMoved = false;
      }
    }

    setColors();
    draw();
    countFish();
  }
    
    return 0;
}