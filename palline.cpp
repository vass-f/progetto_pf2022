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
void infection(int a){ // cambia stato della malattia
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
void evolve_p(){ // fai muovere la pallina
    position(position().x + velocity().x, position().y + velocity().y);
}
void evolve_v(){   // cambia un po' la velocita ai vivi, i morti restano fermi
  auto a = rand() % 10;
  if(a == 1 && infection() != 3 && copysign(1, velocity().x) == 1){
    velocity(velocity().x - (((rand() % 10) - 4) / 8.), velocity().y);
  }
  if(a == 2 && infection() != 3 && copysign(1, velocity().x) == -1){
    velocity(velocity().x + (((rand() % 10) - 4) / 8.), velocity().y);
  }
  if(a == 3 && infection() != 3 && copysign(1, velocity().y) == 1){
    velocity(velocity().x, velocity().y - (((rand() % 10) - 4) / 8.));
  }
  if(a == 4 && infection() != 3 && copysign(1, velocity().y) == -1){
    velocity(velocity().x, velocity().y + (((rand() % 10) - 4) / 8.));
  }
}
double distance (Person& other_p) { // calcola la distanza tra due palline
double d_x = position().x - other_p.position().x;
double d_y = position().y - other_p.position().y;
double d = sqrt(d_x * d_x + d_y * d_y);
return d;
}
};

unsigned const display_width = sf::VideoMode::getDesktopMode().width;  // questa è la roba di sfml 
unsigned const display_height = sf::VideoMode::getDesktopMode().height;

sf::RenderWindow window(sf::VideoMode(display_width, display_height), "Covid epidemic");

  /* class Text
{
private:
    sf::Text s_;
    sf::Text i_;
    sf::Text g_;
    sf::Text m_;
public:
    Text() = default;
    sf::Text s(){
        return s_;
    }
    sf::Text i(){
        return i_;
    }
    sf::Text g(){
        return g_;
    }
    sf::Text m(){
        return m_;
    }
    void setString(){
        s_.setString("Infectable");
        i_.setString("Infected");
        g_.setString("Recovered");
        m_.setString("Dead");
    }
    void setFillColor(){
        s_.setFillColor(sf::Color::Blue);
        i_.setFillColor(sf::Color::Red);
        g_.setFillColor(sf::Color::Green);
        m_.setFillColor(sf::Color::Black);
    }
    void setPosition(){
        s_.setPosition(200,200);
        i_.setPosition(200,300);
        g_.setPosition(200,400);
        m_.setPosition(200,500);
    }
    void draw(){
        window.draw(s_);
        window.draw(i_);
        window.draw(g_);
        window.draw(m_);
    } 
};  */

int main(){
    int r{60};  // distanza contagio
    int r_p{10}; // grandezza palline
    int n_infetti{20}; // infetti iniziali
    int n_tot{200}; // persone totali
    int pace{30}; // velocita palline
    int const fps{60}; // frame per second
    int persone{n_tot};
    int suscettibili{n_tot-n_infetti};
    int infetti{n_infetti};
    int guariti{};
    int morti{};
  //  int nati{};
    std::vector<sf::Text> text(4);
    sf::Font font;
    if (!font.loadFromFile("example_font.ttf")){
    throw std::runtime_error{"Could not load font"};
    }
    for(int i{0}; i != 4; ++i){   
    text[i].setFont(font);
    }
    text[0].setFillColor(sf::Color::Blue);
    text[1].setFillColor(sf::Color::Red);
    text[2].setFillColor(sf::Color::Green);
    text[3].setFillColor(sf::Color::Black);
    text[0].setPosition(20,10);
    text[1].setPosition(20,40);
    text[2].setPosition(20,70);
    text[3].setPosition(20,100);
    srand(time(0));  // volendo di seguito ho messo come commento il codice per far settare tutto all'utente
    /* std::cout<< "Set the radius of the circles (10)" << '\n';
    std::cin >> r_p;
    std::cout<< "Set the distance where the peole can infectate (35)" << '\n';
    std::cin >> r;
    std::cout<< "Set the initial number of people (200)" << '\n';
    std::cin >> n_tot;
    std::cout<< "Set the initial number of infected people (20)" << '\n';
    std::cin >> n_infetti;
    std::cout<< "Set the medium pace of people (100)" << '\n';
    std::cin >> pace; */
    

    window.setFramerateLimit(fps);

    std::vector<Person> p_tot;    // vettore di persone
    for(int i{0}; i != n_tot + 1; ++i){      // metti dentro n_tot persone
        Person p{};
        p.position((rand() % (display_width - 20)), (rand() % (display_height - 70)));
        p.velocity(((rand() % (pace + 1)) - pace/2) / 20., ((rand() % (pace + 1)) - pace/2) / 20.);
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

    if(infetti != 0){    for(auto it = p_tot.begin(); it != p_tot.end() - 1; ++it){
            if((*it).position().x < 0.1 && copysign(1, (*it).velocity().x) == -1) // fai rimbalzare le palline ai bordi
                (*it).velocity((*it).velocity().x * (-1), (*it).velocity().y); 
            if((*it).position().y < 0.1 && copysign(1, (*it).velocity().y) == -1)
                (*it).velocity((*it).velocity().x, (*it).velocity().y * (-1)); 
            if((*it).position().x > (display_width - 20) && copysign(1, (*it).velocity().x) == 1) // fai rimbalzare le palline ai bordi
                (*it).velocity((*it).velocity().x * (-1), (*it).velocity().y); 
            if((*it).position().y > (display_height - 70) && copysign(1, (*it).velocity().y) == 1)
                (*it).velocity((*it).velocity().x, (*it).velocity().y * (-1));
            (*it).evolve_p();                           // fai muovere la pallina
            (*it).evolve_v();                    // varia un po' la velocita della pallina
            if ((*it).infection() == 1){    // guarda la distanza con le altre palline solo se la persona è infetta
              for(long unsigned int i{0}; i != p_tot.size() - 2; i++){
                if ((*it).distance(p_tot[i]) <=  r && p_tot[i].infection() == 0) { // se un suscettibile è vicino a infetti probabilmente si infetta
                    int a = 5 + 20 / (sqrt((p_tot[i].velocity().x * p_tot[i].velocity().x) + (p_tot[i].velocity().y * p_tot[i].velocity().y)));
                    auto n{rand() % a};
                    if(n ==1){
                       if(p_tot[i].infection() != 1){
                        ++infetti;
                        --suscettibili;
                        p_tot[i].infection(1);
                       }}}}}

            if((*it).infection() == 1){ // a volte gli infetti 
                auto n{rand() % 2000};
                if(n == 1 || n == 2){
                  (*it).infection(3); // muoiono e si fermano
                  (*it).velocity(0.,0.);
                  --infetti;
                  ++morti;
                  --persone;
                }
                if(n ==  3 || n == 4 || n == 5){ // a volte guariscono
                  (*it).infection(2);
                  --infetti;
                  ++guariti;
                }
            }
            if((*it).infection() == 2){ // a volte i guariti tornano sucettibili
                auto n{rand() % 1000};
                if(n == 1){
                    (*it).infection(0);
                    ++suscettibili;
                    --guariti;
                }
            }

            (*it).setPosition((*it).position().x, (*it).position().y); // setta la posizione
            if ((*it).infection() == 0){
             (*it).setFillColor(sf::Color::Blue);} // colora tutte le palline
            if ((*it).infection() == 1){
             (*it).setFillColor(sf::Color::Red);}
            if ((*it).infection() == 2){
             (*it).setFillColor(sf::Color::Green);}
            if ((*it).infection() == 3) {
             (*it).setFillColor(sf::Color::Black);}

             if((*it).infection() == 3){ // i morti dopo un po' scompaiono
                auto n{rand() % 50};
                if(n == 1){
                    p_tot.erase(it);
                }
            } 
            }

 /*       auto n{rand() % 100}; // ogni tanto nasce qualche suscettibile o spunta qualche infetto 
        if (n == 1){
            ++nati;
            ++persone;
            Person p{};
            p.position(rand() % (display_width - 20), rand() % (display_height - 70));
            p.velocity(((rand() % pace) - pace/2) / 20., ((rand() % pace) - pace/2) / 20.);
            p.setRadius(r_p);
            auto n_{rand() % 4};
            if(n_ == 1){
                p.setFillColor(sf::Color::Red);
                p.infection(1);
                ++infetti;
                } else {
                    p.setFillColor(sf::Color::Blue);
                    ++suscettibili;
                }
            p_tot.push_back(p);
       //     p_tot.resize(p_tot.size() + 1);
        }   */
        assert(suscettibili+infetti+guariti == persone);
        auto infectable = "Infectable:  " + std::to_string(suscettibili);
        auto infected = "Infected:  " + std::to_string(infetti);
        auto recovered = "Recovered:  " + std::to_string(guariti);
        auto dead = "Dead:  " + std::to_string(morti);
        text[0].setString(infectable);
        text[1].setString(infected);
        text[2].setString(recovered);
        text[3].setString(dead);

        window.clear(sf::Color::White);
        for(auto it = p_tot.begin(); it != p_tot.end() - 1; ++it) window.draw((*it).circle()); // disegna tutte le palline
        for(int i{0}; i != 4; ++i){
            window.draw(text[i]);
        }
        window.display();
    }   else{
            sf::Clock clock;
            auto t_ = clock.getElapsedTime();
            auto t = t_.asSeconds();
            p_tot.erase(p_tot.begin(), p_tot.end());
            text.erase(text.begin(), text.end());
            sf::Text the_end;
            the_end.setFillColor(sf::Color::Green);
            the_end.setPosition(350,300);
            the_end.setCharacterSize(100);
            the_end.setFont(font);
            the_end.setString("People defeated Coronavirus!!!");
            window.clear(sf::Color::White);
            window.draw(the_end);
            window.display();
            std::cout<<t << '\n';
            if(trunc(t) == 5.0){
                abort();
            }
        }
    }

    return 0;
}