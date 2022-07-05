#include <SFML/Window.hpp>
#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <cmath>
#include <iostream>

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
void position(double a, double b){  // cambia posizione
    position_.x = a;
    position_.y = b;
}
void velocity(double a, double b){  // cambia velocita
    velocity_.x = a;
    velocity_.y = b;
}
Point position(){
    return {position_.x, position_.y};
}
Point velocity(){
    return {velocity_.x, velocity_.y};
}
int infection(){ 
    return infection_;
}
void infection(int a){ // cambia stao della malattia
    infection_ = a;
}
sf::CircleShape circle(){
    return circle_;
}
void setRadius(int a){
    circle_.setRadius(a);
}
void setPosition(double a, double b){
    circle_.setPosition(a,b);
}
void setFillColor(sf::Color a){
    circle_.setFillColor(a);
}
void evolve(){ // fai muovere la pallina
    position(position().x + velocity().x, position().y + velocity().y);

}
double distance (Person& other_p) { // calcola la distanza tra due palline
double d_x = position().x - other_p.position().x;
double d_y = position().y - other_p.position().y;
double d = sqrt(d_x * d_x + d_y * d_y);
return d;
}
};


int main(){
    int r{25};  // distanza contagio
    int r_p{10}; // grandezza palline
    int n_infetti{20}; // infetti iniziali
    int n_tot{200}; // persone totali
    int pace{100}; // velocita palline
    int const fps{60}; // frame per second
    srand(time(0));
    /* std::cout<< "Set the radius of the circles (5)" << '\n';
    std::cin >> r_p;
    std::cout<< "Set the distance where the peole can infectate (15)" << '\n';
    std::cin >> r;
    std::cout<< "Set the initial number of people (200)" << '\n';
    std::cin >> n_tot;
    std::cout<< "Set the initial number of infected people (30)" << '\n';
    std::cin >> n_infetti;
    std::cout<< "Set the medium pace of people (500)" << '\n';
    std::cin >> pace; */

    unsigned const display_width = sf::VideoMode::getDesktopMode().width;
    unsigned const display_height = sf::VideoMode::getDesktopMode().height;

    sf::RenderWindow window(sf::VideoMode(display_width, display_height), "Covid epidemic");
    window.setFramerateLimit(fps);

    std::vector<Person> p_tot;    // vettore di Persone
    for(int i{0}; i != n_tot; ++i){      // metti dentro n_tot Persone
        Person p{};
        p.position(rand() % display_width, rand() % display_height);
        p.velocity(((rand() % pace) - pace/2) / 20., ((rand() % pace) - pace/2) / 20.);
        p.setRadius(r_p);
        p_tot.push_back(p);
    }
    for(int i{0}; i != n_infetti; ++i) {   // metti dentro n_infetti
        p_tot[i].infection(1);
    }

    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {   
            if(event.type == sf::Event::Closed)
                window.close();
        }

        for(auto it = p_tot.begin(); it != p_tot.end() - 1; ++it){
         //   long unsigned int i = std::distance(p_tot.begin(), it);
            if((*it).position().x < 0.1 || (*it).position().x > display_width - 20) // fai rimbalzare le palline ai bordi
                (*it).velocity((*it).velocity().x * (-1), (*it).velocity().y); 
            if((*it).position().y < 0.1 || (*it).position().y > display_height - 70)
                (*it).velocity((*it).velocity().x, (*it).velocity().y * (-1)); 
       //     (*it).position((*it).position().x + (*it).velocity().x, (*it).position().y + (*it).velocity().y);
            (*it).evolve();
            if ((*it).infection() == 1){    // guarda la distanza con le altre palline solo se la persona è infetta
              for(long unsigned int i{0}; i != p_tot.size() - 1; i++){
                if ((*it).distance (p_tot[i]) <=  r && p_tot[i].infection() == 0) { // se un infetto è vicino a suscettibili probabilmente si infetta
                    auto n{rand() % 10};
                    if(n ==1){
                       p_tot[i].infection(1);
                    }}}}

            if((*it).infection() == 1){ // a volte gli infetti 
                auto n{rand() % 1000};
                if(n == 1){
                  (*it).infection(3); // muoiono
                }
                if(n == 2){ // a volte guariscono
                  (*it).infection(2);
                }
            }
            if((*it).infection() == 2){ // a volte i guariti tornano sucettibili
                auto n{rand() % 500};
                if(n == 1){
                    (*it).infection(0);
                }
            }
            if((*it).infection() == 3){ // i morti dopo un po' scompaiono
                auto n{rand() % 100};
                if(n == 1){
                    p_tot.erase(it);
                }
            } 
    /*        auto n{rand() % 10000}; // ogni tanto nasce qualche suscettibile o qualche infetto torna dall'estero
            if (n == 1){
                Person p{};
            //    p.position(rand() % display_width, rand() % display_height);
                p.velocity(((rand() % pace) - pace/2) / 20, ((rand() % pace) - pace/2) / 20);
                p.setRadius(r_p);
                auto n_{rand() % 4};
                if(n_ == 1){
                    p.infection(1);
                }
                p_tot.push_back(p);
            }  */
            if((*it).velocity().x == 0. && (*it).velocity().y == 0){    // se le palline si fermano a caso si fanno ripartire
                (*it).velocity(((rand() % pace) - pace/2) / 20, ((rand() % pace) - pace/2) / 20);
            }
      /*      if ((*it).velocity().x == 0 && (*it).velocity().y == 0){ // togli le palline ferme
            p_tot.erase(it);
            } */
            (*it).setPosition((*it).position().x, (*it).position().y); // setta la posizione
            if ((*it).infection() == 0)
             (*it).setFillColor(sf::Color::Blue); // colora tutte le palline
            if ((*it).infection() == 1)
             (*it).setFillColor(sf::Color::Red);
            if ((*it).infection() == 2)
             (*it).setFillColor(sf::Color::Green);
            if ((*it).infection() == 3)
             (*it).setFillColor(sf::Color::Black);
            }

        window.clear(sf::Color::White);
        for(auto it = p_tot.begin(); it != p_tot.end() - 1; ++it) window.draw((*it).circle()); // disegna tutte le palline
        window.display();
    }

    return 0;
}