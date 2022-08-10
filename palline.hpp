#include <SFML/Window.hpp>
#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <cmath>
#include <iostream>
#include <cassert>
#include <stdexcept>
#include <string>

struct Point{    // coordinata xy
    double x;
    double y;
};

class Person{ // persona che sarebbe un cerchio che si muove
  private:
    Point position_{0.,0.};
    Point velocity_{0.,0.};
    int infection_{0}; //0 suscettibile 1 infetto 2 guarito 3 morto
    sf::CircleShape circle_;
  public:
    Person() = default;
    void position(double a, double b);
    void velocity(double a, double b);
    Point position();
    Point velocity();
    int infection();
    void infection(int a);
    sf::CircleShape circle();
    void setRadius(int a);
    void setPosition(double a, double b);
    void setFillColor(sf::Color a);
    void evolve_p();
    void evolve_v();
    double distance (Person& other_radius);
};