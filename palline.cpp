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
void evolve_v(){   // cambia un po' la velocita ai vivi, i dead_t restano fermi
  auto a = rand() % 5;
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

int main(){
    int r{60};  // distanza contagio
    int r_p{10}; // grandezza palline
    int n_infected_t{20}; // infected_t iniziali
    int n_tot{200}; // people_t totali
    int pace{30}; // velocita palline
    int const fps{60}; // frame per second
    int people_t{n_tot};
    int infectable_t{n_tot-n_infected_t};
    int infected_t{n_infected_t};
    int recovered_t{};
    int dead_t{};
    int text_t{5};
    std::vector<sf::Text> text(text_t);
    sf::Font font;
    if (!font.loadFromFile("example_font.ttf")){
    throw std::runtime_error{"Could not load font"};
    }
    for(int i{0}; i != text_t; ++i){   
    text[i].setFont(font);
    }

    text[0].setFillColor(sf::Color::Blue);
    text[1].setFillColor(sf::Color::Red);
    text[2].setFillColor(sf::Color::Green);
    text[3].setFillColor(sf::Color::Black);
    text[4].setFillColor(sf::Color::Cyan);
    text[0].setPosition(20,10);
    text[1].setPosition(20,40);
    text[2].setPosition(20,70);
    text[3].setPosition(20,100);
    text[4].setPosition(20,130);
    sf::Clock clock;
    srand(time(0));  // volendo di seguito ho messo come commento il codice per far settare tutto all'utente
    /* std::cout<< "Set the radius of the circles (10)" << '\n';
    std::cin >> r_p;
    std::cout<< "Set the distance where the peole can infectate (35)" << '\n';
    std::cin >> r;
    std::cout<< "Set the initial number of people (200)" << '\n';
    std::cin >> n_tot;
    std::cout<< "Set the initial number of infected people (20)" << '\n';
    std::cin >> n_infected_t;
    std::cout<< "Set the medium pace of people (100)" << '\n';
    std::cin >> pace; */
    
    unsigned const display_width = sf::VideoMode::getDesktopMode().width;  // questa è la roba di sfml 
    unsigned const display_height = sf::VideoMode::getDesktopMode().height;

    sf::RenderWindow window(sf::VideoMode(display_width, display_height), "Covid epidemic");
    window.setFramerateLimit(fps);

    std::vector<Person> people;    // vettore di people_t
    for(int i{0}; i != n_tot; ++i){      // metti dentro n_tot people_t
        Person p{};
        p.position((rand() % (display_width - 20)), (rand() % (display_height - 70)));
        p.velocity(((rand() % (pace + 1)) - pace/2) / 20., ((rand() % (pace + 1)) - pace/2) / 20.);
        p.setRadius(r_p);
        people.push_back(p);
    }
    for(int i{0}; i != n_infected_t; ++i) {   // metti dentro n_infected_t
        people[i].infection(1);
    }

    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {   
            if(event.type == sf::Event::Closed)
                window.close();
        }

    if(infected_t != 0){for(auto it = people.begin(); it != people.end(); ++it){
            if((*it).position().x < 0.1 && copysign(1, (*it).velocity().x) == -1)                 // fai rimbalzare le palline ai bordi
                (*it).velocity((*it).velocity().x * (-1), (*it).velocity().y); 
            if((*it).position().y < 0.1 && copysign(1, (*it).velocity().y) == -1)
                (*it).velocity((*it).velocity().x, (*it).velocity().y * (-1)); 
            if((*it).position().x > (display_width - 20) && copysign(1, (*it).velocity().x) == 1) // fai rimbalzare le palline ai bordi
                (*it).velocity((*it).velocity().x * (-1), (*it).velocity().y); 
            if((*it).position().y > (display_height - 70) && copysign(1, (*it).velocity().y) == 1)
                (*it).velocity((*it).velocity().x, (*it).velocity().y * (-1));
            (*it).evolve_p();                    // fai muovere la pallina
            (*it).evolve_v();                    // varia un po' la velocita della pallina
            if ((*it).infection() == 1){         // guarda la distanza con le altre palline solo se la persona è infetta
              for(long unsigned int i{0}; i != people.size() - 2; i++){
                if ((*it).distance(people[i]) <=  r && people[i].infection() == 0) { // se un suscettibile è vicino a infected_t probabilmente si infetta
                    int a = 30; // = 5 + 20 / (sqrt((people[i].velocity().x * people[i].velocity().x) + (people[i].velocity().y * people[i].velocity().y)));
                                // sennò avevo messo che la probabilità del contagio è proporzionale alla velocità della pallina, per bilanciare il 
                                // fatto che sennò essendo veloce resta meno tempo vicino all'altra ed è più difficile che la infected_t, però poi sono 
                                // troppi calcoli, quindi scarto questa opzione per alleggerire il programma, poi ditemi voi
                    auto n{rand() % a};
                    if(n ==1){
                       if(people[i].infection() != 1){
                        ++infected_t;
                        --infectable_t;
                        people[i].infection(1);
                       }}}}}

            if((*it).infection() == 1){ // a volte gli infected_t 
                auto n{rand() % 2000};
                if(n == 1 || n == 2){
                  (*it).infection(3); // muoiono e si fermano
                  (*it).velocity(0.,0.);
                  --infected_t;
                  ++dead_t;
                  --people_t;
                }
                if(n ==  3 || n == 4 || n == 5){ // a volte guariscono
                  (*it).infection(2);
                  --infected_t;
                  ++recovered_t;
                }
            }
            if((*it).infection() == 2){ // a volte i recovered_t tornano sucettibili
                auto n{rand() % 1000};
                if(n == 1){
                    (*it).infection(0);
                    ++infectable_t;
                    --recovered_t;
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

             if((*it).infection() == 3){ // i dead_t dopo un po' scompaiono
                auto n{rand() % 50};
                if(n == 1){
                    people.erase(it);
                }
            } 
            }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
           Person p{};
           auto mouse_position = sf::Mouse::getPosition();
           p.position(mouse_position.x - 12, mouse_position.y - 60);
           p.velocity(((rand() % (pace + 1)) - pace/2) / 20., ((rand() % (pace + 1)) - pace/2) / 20.);
           p.setRadius(r_p);
           ++infectable_t; 
           ++people_t;
           people.push_back(p);
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right)){
           Person p{};
           auto mouse_position = sf::Mouse::getPosition();
           p.position(mouse_position.x - 12, mouse_position.y - 60);
           p.velocity(((rand() % (pace + 1)) - pace/2) / 20., ((rand() % (pace + 1)) - pace/2) / 20.);
           p.setRadius(r_p);
           p.infection(1);
           ++infected_t; 
           ++people_t;
           people.push_back(p);
        }


        if(people.capacity() < people.size()){
            throw std::runtime_error{"capacity overreached"};
        }
        assert(infectable_t+infected_t+recovered_t == people_t);

        auto infectable = "Infectable:  " + std::to_string(infectable_t);
        auto infected = "Infected:  " + std::to_string(infected_t);
        auto recovered = "Recovered:  " + std::to_string(recovered_t);
        auto dead = "Dead:  " + std::to_string(dead_t);
        auto tot = "Totale:  " + std::to_string(infectable_t+infected_t+recovered_t+dead_t);
        text[0].setString(infectable);
        text[1].setString(infected);
        text[2].setString(recovered);
        text[3].setString(dead);
        text[4].setString(tot);

        window.clear(sf::Color::White);
        for(auto it = people.begin(); it != people.end(); ++it) window.draw((*it).circle()); // disegna tutte le palline
        for(int i{0}; i != text_t; ++i){
            window.draw(text[i]);
        }
        window.display();

    }  
    if(infected_t == 0 && dead_t != n_tot) {  
        people.erase(people.begin(), people.end());
        text.erase(text.begin(), text.end());
        sf::Text the_end;
        the_end.setPosition(300,300);
        the_end.setCharacterSize(100);
        the_end.setFont(font);
        the_end.setString("Humanity defeated Coronavirus!!!");
        int t = clock.getElapsedTime().asSeconds();
        if(t % 2 == 0){
            window.clear(sf::Color::Cyan);
            the_end.setFillColor(sf::Color::Black);
        } else {
            window.clear(sf::Color::Blue);
            the_end.setFillColor(sf::Color::Green);
        }
        window.draw(the_end);
        window.display();
        }
    if(people_t == 0){
        people.erase(people.begin(), people.end());
        text.erase(text.begin(), text.end());
        sf::Text the_end;
        the_end.setPosition(300,300);
        the_end.setCharacterSize(100);
        the_end.setFont(font);
        the_end.setString("Coronavirus defeated Humanity \n                           Try again");
        int t = clock.getElapsedTime().asSeconds();
        if(t % 2 == 0){
            window.clear(sf::Color::Red);
            the_end.setFillColor(sf::Color::Black);
        } else {
            window.clear(sf::Color::Blue);
            the_end.setFillColor(sf::Color::Yellow);
        }
        window.draw(the_end);
        window.display();
    }
    }
    return 0;
}