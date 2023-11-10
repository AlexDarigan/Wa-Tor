// wator.cpp --- 
// 
// Filename: wator.cpp
// Description: 
// Author: David Darigan
// Maintainer: David Darigan
// Created: Fri Nov  9
// Last-Updated: Fri Nov  9
//           By: David
//     Update #: 1
// 
// 

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

#include <SFML/Graphics.hpp>
#include <cstdlib> 

// Defining structs because we need more information (like "moved this round")
struct Cell {
  bool isFish = false;
  bool isOcean = false;
  bool hasMoved = false;
};

// make

// Constants required by following arrays
const int xdim = 100;
const int ydim= 100;

// Allocating arrays on the heap by moving them to the global scope
sf::RectangleShape recArray[xdim][ydim];
sf::RectangleShape recArray2[xdim][ydim];
Cell cells[xdim][ydim];

int main()
{
  srand(0);
  int WindowXSize=800;
  int WindowYSize=600;
  int cellXSize=WindowXSize/xdim;
  int cellYSize=WindowYSize/ydim;
  //each shape will represent either a fish, shark or empty space
  //e.g. blue for empty, red for shark and green for fish

  // Allocating on the heap rather than the stack (because large x/y can cause stack)
  
  for(int i=0;i<xdim;++i){
    recArray[xdim][ydim].setFillColor(sf::Color::Blue);
    for(int k=0;k<ydim;++k){//give each one a size, position and color
      recArray2[i][k].setFillColor(sf::Color::Blue);
      recArray[i][k].setSize(sf::Vector2f(80.f,80.f));
      recArray[i][k].setPosition(i*cellXSize,k*cellYSize);//position is top left corner!
      int id=i*1-+k;
      if (id%2==0) { 
        recArray[i][k].setFillColor(sf::Color::Green);
        cells[i][k].isFish = true;
      }
      else {
        recArray[i][k].setFillColor(sf::Color::Blue);
        cells[i][k].isOcean = true;        
      }
    }
  }
    sf::RenderWindow window(sf::VideoMode(WindowXSize,WindowYSize), "SFML Wa-Tor world");
   

    while (window.isOpen())
    {
 
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

      // MoveFishLeft + New Fish
      for(int y = 0; y < ydim; ++y) {
        for(int x = 0; x < xdim; ++x) {
          int destination = (x + 1) % xdim;
          //bool isFish = recArray[x][y].getFillColor() == Fish;
          //bool isNotBlocked = recArray[destination][y].getFillColor() == Ocean;
          bool isFish = cells[x][y].isFish;
          bool isNotBlocked = cells[destination][y].isOcean;
          bool willMoveNow = (rand() % 2) == 1; // 1 - Yes / 0 - No
          bool hasMoved = cells[x][y].hasMoved;
          if(isFish && willMoveNow && isNotBlocked && !hasMoved) {
            cells[destination][y].isFish = true;
            cells[destination][y].isOcean = false;
            cells[destination][y].hasMoved = true;
            cells[x][y].isFish = false;
            cells[x][y].isOcean = true;
          //  recArray2[destination][y].setFillColor(Fish);
          //  recArray2[x][y].setFillColor(Ocean);
          } else {
            // If we're not moving, we still need to state that in rect2
            if(isFish) {
              //recArray2[x][y].setFillColor(Fish); 
              cells[x][y].isFish = true;
              cells[x][y].isOcean = false;
            }
            else {
              //recArray[x][y].setFillColor(Ocean);
              cells[x][y].isOcean = true; // do we care?
              cells[x][y].isFish = false;
            }
            if(isNotBlocked) {
              // Turn water into wine but not fish into the Ocean, that's for the sharks
              //recArray2[destination][y].setFillColor(Ocean);
              cells[destination][y].isOcean = true;
              cells[destination][y].isFish = false;
            }
            printf("IsFish: %d, IsNotBlocked: %d, willMoveNow: %d\n", isFish, isNotBlocked, willMoveNow);
          }
        }
      }

      // TODO: Fix Direction Names
      // // MoveFishRight + New Fish
      for(int y = 0; y < ydim; ++y) {
        for(int x = xdim - 1; x > 0; --x) {
          int destination = (x - 1) % xdim;
          // We need to add additional checks now that rec2 has been populated
          //bool isNotBlocked = (recArray[destination][y].getFillColor() == Ocean) && (recArray2[destination][y].getFillColor() == Ocean);
          bool isFish = cells[x][y].isFish;
          bool isNotBlocked = cells[destination][y].isOcean;
          // Fish are now able to be in two places at once
          bool willMoveNow = (rand() % 2) == 1; // 1 - Yes / 0 - No
          bool hasMoved = cells[x][y].hasMoved;
          if(isFish && willMoveNow && isNotBlocked && !hasMoved) {
            cells[destination][y].isFish = true;
            cells[destination][y].isOcean = false;
            cells[destination][y].hasMoved = true;
            cells[x][y].isFish = false;
            cells[x][y].isOcean = true;
          } else {
            // If we're not moving, we still need to state that in rect2
            if(isFish) {
              cells[x][y].isFish = true;
              cells[x][y].isOcean = false;
            }
            else {
              cells[x][y].isFish = false;
              cells[x][y].isOcean = true;
            }
            if(isNotBlocked) {
              // Turn water into wine but not fish into the Ocean, that's for the sharks
              cells[destination][y].isFish = false;
              cells[destination][y].isOcean = true;
            }
            printf("IsFish: %d, IsNotBlocked: %d, willMoveNow: %d\n", isFish, isNotBlocked, willMoveNow);
          }
        }
      }
      // MoveFishUp + New Fish
      // MoveFishDown + New Fish
      // MoveSharksLeft + EatFish
      // MoveSharksRight + EatFish
      // MoveSharksUp + EatFish
      // MoveSharksDown + EatFish

    // Pass buffer and clear
    for(int y = 0; y < ydim; ++y) {
      for(int x = 0; x < xdim; ++x) {
        cells[x][y].hasMoved = false;
        if(cells[x][y].isFish) {
          recArray[x][y].setFillColor(sf::Color::Green);
        } else {
          recArray[x][y].setFillColor(sf::Color::Blue);
        }
        //recArray[x][y].setFillColor(recArray2[x][y].getFillColor());
        //recArray2[x][y].setFillColor(Ocean);
      }
    }

    

    

	
	
	//loop these three lines to draw frames
  window.clear(sf::Color::Black);
	for(int i=0;i<xdim;++i){
	  for(int k=0;k<ydim;++k){
	    window.draw(recArray[i][k]);
	  }
	}
        window.display();
    }

    


    return 0;
}

// 
// wator.cpp ends here
