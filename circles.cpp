#include "circles.hpp"

void Person::position(double a, double b){ 
    position_.x = a;
    position_.y = b;
}
void Person::velocity(double a, double b){ 
    velocity_.x = a;
    velocity_.y = b;
}
Point Person::position(){
    return {position_.x, position_.y};
}
Point Person::velocity(){
    return {velocity_.x, velocity_.y};
}
int Person::infection(){ 
    return infection_;
}
void Person::infection(int a){ 
    infection_ = a;
}
sf::CircleShape Person::circle(){
    return circle_;
}
void Person::setRadius(int a){
    circle_.setRadius(a);
}
void Person::setPosition(double a, double b){
    circle_.setPosition(a,b);
}
void Person::setFillColor(sf::Color a){
    circle_.setFillColor(a);
}
void Person::evolve_p(){
    position(position().x + velocity().x, position().y + velocity().y);
}
void Person::evolve_v(){   
  auto a = rand() % 8;
  if(a == 1 && infection() != 3 && velocity().x > 0){  // dead's velocity do not change, they are static
    velocity(velocity().x - (((rand() % 10) - 4) / 8.), velocity().y);} // the information about the sign is used beacause people should more 
  if(a == 2 && infection() != 3 && velocity().x < 0){  // probably decrease their velocity to get a more realistic motion effect
    velocity(velocity().x + (((rand() % 10) - 4) / 8.), velocity().y);}
  if(a == 3 && infection() != 3 && velocity().y > 0){
    velocity(velocity().x, velocity().y - (((rand() % 10) - 4) / 8.));}
  if(a == 4 && infection() != 3 && velocity().y < 0){
    velocity(velocity().x, velocity().y + (((rand() % 10) - 4) / 8.));}
} 

double Person::distance (Person& other_radius) { // calculates the distance between two people
  double d_x = position().x - other_radius.position().x;
  double d_y = position().y - other_radius.position().y;
  double d = sqrt(d_x * d_x + d_y * d_y);
  return d;
}