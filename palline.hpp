#ifndef PALLINE_HPP
#define PALLINE_HPP

#include <SFML/Window.hpp>
#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <cmath>
#include <iostream>
#include <cassert>
#include <stdexcept>
#include <string>

struct Point{    
    double x;
    double y;
};

class Person{ 
  private:
    Point position_{0.,0.};
    Point velocity_{0.,0.};
    int infection_{0}; //integer number with the information of the infection status, 0 means infectable, 1 means infected, 2 means recovered, 3 means dead
    sf::CircleShape circle_;
  public:
    Person(Point position, Point velocity, int infection, sf::CircleShape circle) : position_{position}, velocity_{velocity}, infection_{infection}, circle_{circle}{
      if(infection_ < 0 || infection_ > 3){
        throw std::runtime_error{"non valid infection parameter"};
      }
      if(position_.x < 0 || position_.y < 0){
        throw std::runtime_error{"non valid position"};
      }
    }
    Person() {}  // default constructor
    void position(double a, double b); // modifies position
    void velocity(double a, double b); // modifies velocity
    void infection(int a);  // modifies the infection status
    Point position();  // returns position
    Point velocity();  // returns velocity
    int infection();   // returns the infection status
    sf::CircleShape circle(); 
    void setRadius(int a);
    void setPosition(double a, double b);
    void setFillColor(sf::Color a);
    void evolve_p();  // changes position 
    void evolve_v();  // changes velocity randomly
    double distance(Person& other_radius); // calculates the distance between two people
};

#endif