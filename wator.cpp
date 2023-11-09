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

sf::Color Ocean = sf::Color::Blue;
sf::Color Fish = sf::Color::Green;



int main()
{
  int xdim = 10;
  int ydim= 10;
  int WindowXSize=800;
  int WindowYSize=600;
  int cellXSize=WindowXSize/xdim;
  int cellYSize=WindowYSize/ydim;
  //each shape will represent either a fish, shark or empty space
  //e.g. blue for empty, red for shark and green for fish
  sf::RectangleShape recArray[xdim][ydim];
  sf::RectangleShape recArray2[xdim][ydim];
  for(int i=0;i<xdim;++i){
    for(int k=0;k<ydim;++k){//give each one a size, position and color
      recArray[i][k].setSize(sf::Vector2f(80.f,80.f));
      recArray[i][k].setPosition(i*cellXSize,k*cellYSize);//position is top left corner!
      int id=i*1-+k;
      if (id%2==0) recArray[i][k].setFillColor(Fish);
      else recArray[i][k].setFillColor(Ocean);
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
	
      // current = future
      // future = new Array()

      // MoveFishLeft + New Fish
      for(int y = 0; y < ydim; ++y) {
        for(int x = 0; x < xdim; ++x) {
          bool isFish = recArray[x][y].getFillColor() == Fish;
          bool isBlocked = recArray[(x + 1) % xdim][y].getFillColor() == Fish;
          if(isFish && !isBlocked) {
            printf("%s","Move Fish\n");
          } else {
            printf("%s", "No Move Fish\n");
          }
          // if(recArray[(x + 1) % xdim][y].color == sf::Color)
          // recArray[x][y].setFillColor(sf::Color::Yellow);
        }
        break;
      }
      // MoveFishRight + New Fish
      // MoveFishUp + New Fish
      // MoveFishDown + New Fish
      // MoveSharksLeft + EatFish
      // MoveSharksRight + EatFish
      // MoveSharksUp + EatFish
      // MoveSharksDown + EatFish
	
	
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
