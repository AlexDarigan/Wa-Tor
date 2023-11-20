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



auto start = std::chrono::high_resolution_clock::now();
auto end = std::chrono::high_resolution_clock::now();
auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
// Defining structs because we need more information (like "moved this round")

enum Direction { North = 0, East = 1, South = 2, West = 3 };
enum CellType { Ocean, Fish, Shark };

struct Cell {
  CellType celltype = Ocean;
};

// Constants required by following arrays
const int DIMENSIONS = 600; // Shape is a square so x and y arrays are the same dimensions
const int UP = -1;
const int DOWN = 1;
const int RIGHT = 1;
const int LEFT = -1;
const int STANDSTILL = 0;
const bool OVERRIDING_FREEWILL = true;

// Allocating arrays on the heap by moving them to the global scope
sf::RectangleShape recArray[DIMENSIONS][DIMENSIONS];
Cell cells[DIMENSIONS][DIMENSIONS];
Cell future[DIMENSIONS][DIMENSIONS];

bool willMove(int x, int y, int destX, int destY, bool overrideFreeWill) {
  return (
    (cells[x][y].celltype == CellType::Fish) 
    && cells[destX][destY].celltype == Ocean 
    && (overrideFreeWill || (rand() % 2) == 1) // (Yes / No) OR Divine Intervention
  );
}

int getNextMove(int location, int direction) {
  // If direction is STANDSTILL (0), then we perform a roundabout to the original location
  return (location + (1 * direction) + DIMENSIONS) % DIMENSIONS;
}


void moveFish(int xDirection, int yDirection, bool overrideFreeWill = false) {
  for(int y = 0; y < DIMENSIONS; ++y) {
    for(int x = 0; x < DIMENSIONS; ++x) {
      int xDestination = getNextMove(x, xDirection);
      int yDestination = getNextMove(y, yDirection);
      if(willMove(x, y, xDestination, yDestination, overrideFreeWill)) {
        //cells[xDestination][yDestination].celltype = CellType::FutureFish;
        //cells[x][y].celltype = CellType::PastFish;
      }
    }
  }
}

void moveLazyFish() {
  // Handle all the fish who have not yet moved
  for(int y = 0; y < DIMENSIONS; ++y) {
    for(int x = 0; x < DIMENSIONS; ++x) {
      int direction = rand() % 4;
      int options = 4;
      while((cells[x][y].celltype == CellType::Fish) && (options > 0)) {
        switch(direction) {
          case Direction::North: {
              moveFish(STANDSTILL, UP, OVERRIDING_FREEWILL);
              break;
            }  
            case Direction::East: {
              moveFish(RIGHT, STANDSTILL, OVERRIDING_FREEWILL);
              break;
            } 
            case Direction::South: {
              moveFish(STANDSTILL, DOWN, OVERRIDING_FREEWILL);
              break;
            }
            case Direction::West: {
              moveFish(LEFT, STANDSTILL, OVERRIDING_FREEWILL);
              break;
            }
            default:
              break;
        }
          --options;
          // Move to the next direction, wrap if near boundaries with options left
          direction = (direction + 1) % 4;
        }
        if(cells[x][y].celltype == CellType::Fish) {
          // If we're still a fish (ie have not moved, then set as future fish)
          // ..this is unnecessary but useful for debugging new fish
         // cells[x][y].celltype = CellType::FutureFish;
        }
      }
    }
}


void countFish() {
    // Not working properly, too many fishes
    int fishes = 0;
    for(int x = 0; x < DIMENSIONS; ++x) {
      for(int y = 0; y < DIMENSIONS; ++y) {
        if(cells[x][y].celltype == CellType::Fish) {
          fishes++;
        }
      }
    }
    printf("Fishes: %d\n", fishes);
  }

void copyFuture() {
// Applying future to current
  for(int y = 0; y < DIMENSIONS; ++y) {
    for(int x = 0; x < DIMENSIONS; ++x) {
      cells[x][y] = future[x][y];
      future[x][y].celltype = CellType::Ocean;
    }
  }
}


const int WindowXSize=800;
const int WindowYSize=600;
const int cellXSize=WindowXSize/DIMENSIONS;
const int cellYSize=WindowYSize/DIMENSIONS;
sf::RenderWindow window(sf::VideoMode(WindowXSize,WindowYSize), "SFML Wa-Tor world");

void draw() {
   window.clear(sf::Color::Black);
    for(int i=0;i<DIMENSIONS;++i){
      for(int k=0;k<DIMENSIONS;++k){
        window.draw(recArray[i][k]);
      }
    }
      sf::sleep(sf::seconds(2));
      window.display();
}

void setColors() {
  for(int y = 0; y < DIMENSIONS; ++y) {
    for(int x = 0; x < DIMENSIONS; ++x) {
      switch(cells[x][y].celltype) {
        case CellType::Ocean:
          recArray[x][y].setFillColor(sf::Color::Blue);
          break;
        case CellType::Fish:
          recArray[x][y].setFillColor(sf::Color::Green);
          break;
        case CellType::Shark:
          recArray[x][y].setFillColor(sf::Color::Red);
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
  for(int i=0;i<DIMENSIONS;++i){
    for(int k=0;k<DIMENSIONS;++k){
      recArray[i][k].setSize(sf::Vector2f(cellXSize,cellYSize));
      recArray[i][k].setPosition(i*cellXSize,k*cellYSize);
      int id=i*1-+k;
      //if(false)
      // if(id%10==0) {
      //   recArray[i][k].setFillColor(sf::Color::Red);
      //   cells[i][k].celltype = CellType::Shark;
      // }
      //else 
      if (id%2==0) { 
      //if(i == 8 && k == 7) {  
        recArray[i][k].setFillColor(sf::Color::Green);
        cells[i][k].celltype = CellType::Fish;
      }
      else {
        recArray[i][k].setFillColor(sf::Color::Blue);
        cells[i][k].celltype = CellType::Ocean;
      }
    }
  }
}


int main()
{
  srand(0);
 
  initialize();
  omp_set_num_threads(6);
  while (window.isOpen())
  {
      poll();
      auto start = std::chrono::steady_clock::now();

      #pragma omp parallel
      {
      // move fish left
        #pragma omp for collapse(2)
        for(int y = 0; y < DIMENSIONS; ++y) {
          for(int x = 0; x < DIMENSIONS; ++x) {
            int xDestination = getNextMove(x, RIGHT);
            int yDestination =  getNextMove(y, STANDSTILL);
            if(willMove(x, y, xDestination, yDestination, false)) {
              future[xDestination][yDestination] = cells[x][y];
            } else {
              if(cells[x][y].celltype == CellType::Fish) {
                future[x][y] = cells[x][y];
              }
            }
          }
        }
      }

    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    printf("Duration: %ld\n", duration);

    copyFuture();
    setColors();
    countFish();
    draw();
  }
    
    return 0;
}
// wator.cpp ends here
