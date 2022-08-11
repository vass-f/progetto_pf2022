#include "palline.hpp"

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
  auto a = rand() % 5;
  if(a == 1 && infection() != 3 && copysign(1, velocity().x) == 1){  // dead's velocity do not change, they are static
    velocity(velocity().x - (((rand() % 10) - 4) / 8.), velocity().y); // 
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
double Person::distance (Person& other_radius) { // calcola la distanza tra due palline
  double d_x = position().x - other_radius.position().x;
  double d_y = position().y - other_radius.position().y;
  double d = sqrt(d_x * d_x + d_y * d_y);
  return d;
}

int main(){
    int infection_distance{60};  // distanza contagio
    int radius{10}; // grandezza palline
    int first_infected{}; // n_infected iniziali
    int n_tot{}; // n_people_alive totali
    int pace{30}; // velocita palline
    int const fps{60}; // frame per second
    int n_text{6};  // numero di frasi/dati che possono apparire sullo schermo
    std::vector<sf::Text> text(n_text);
    sf::Font data_font;
    sf::Font comment_font;
    if (!data_font.loadFromFile("data_font.ttf")){
    throw std::runtime_error{"Could not load data font"};
    }
    if (!comment_font.loadFromFile("comment_font.ttf")){
    throw std::runtime_error{"Could not load comment font"};
    }
    for(int i{0}; i != 4; ++i){   
    text[i].setFont(comment_font);
    }
    for(int i{4}; i != 6; ++i){   
    text[i].setFont(data_font);
    }

    text[0].setFillColor(sf::Color::Blue);
    text[1].setFillColor(sf::Color::Red);
    text[2].setFillColor(sf::Color::Green);
    text[3].setFillColor(sf::Color::Black);
    text[4].setFillColor(sf::Color::Black);
    text[5].setFillColor(sf::Color::Red);
    for(int i{0}; i != n_text; ++i){
        text[i].setPosition(20,10+30*i);
    }

    sf::Clock clock;
    srand(time(0));  // volendo di seguito ho messo come commento il codice per far settare tutto all'utente 
                     // se non si mette questa opzione tutti i dati dovrebbero essere const per sicurezza
    
    std::cout<< "Set the initial number of people" << '\n';
    std::cin >> n_tot;
    std::cout<< "Set the initial number of infected people" << '\n';
    std::cin >> first_infected; 
        
    int n_people_alive{n_tot};
    int n_infectable{n_tot-first_infected};
    int n_infected{first_infected};
    int n_recovered{};
    int n_dead{};
    
    unsigned const display_width = sf::VideoMode::getDesktopMode().width;  // questa è la roba di sfml 
    unsigned const display_height = sf::VideoMode::getDesktopMode().height;

    sf::RenderWindow window(sf::VideoMode(display_width, display_height), "Covid epidemic");
    window.setFramerateLimit(fps);

    std::vector<Person> people;    // vettore di people
    for(int i{0}; i != n_tot; ++i){      // metti dentro n_tot people
        Person p{};
        p.position((rand() % (display_width - 20)), (rand() % (display_height - 70)));
        p.velocity(((rand() % (pace + 1)) - pace/2) / 20., ((rand() % (pace + 1)) - pace/2) / 20.);
        p.setRadius(radius);
        people.push_back(p);
    }
    for(int i{0}; i != first_infected; ++i) {   // metti dentro first_infected
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

    if(n_infected != 0){for(auto it = people.begin(); it != people.end(); ++it){
            auto n{rand() % 2000};                                    // genero il numero casuale da cui dipende tutto
            if((*it).position().x < 0.1 && copysign(1, (*it).velocity().x) == -1)   // fai rimbalzare le palline ai bordi
                (*it).velocity((*it).velocity().x * (-1), (*it).velocity().y);      // il copysign serve a capire la direzione delle palline, cioè il segno della
            if((*it).position().y < 0.1 && copysign(1, (*it).velocity().y) == -1)   // velocità, positiva se verso basso o destra e negativa se verso alto o 
                (*it).velocity((*it).velocity().x, (*it).velocity().y * (-1));      // sinistra, così che le palline rimbalzano solo se stanno andando contro il
                // muro, che a volte si buggava e la pallina si incastrava continuando a rimbalzare addosso alla parete perché, essendo oltre il 
                // limite dello schermo, ad ogni frame cambiava direzine e non riusciva ad uscire
            if((*it).position().x > (display_width - 20) && copysign(1, (*it).velocity().x) == 1) // fai rimbalzare le palline ai bordi
                (*it).velocity((*it).velocity().x * (-1), (*it).velocity().y); 
            if((*it).position().y > (display_height - 70) && copysign(1, (*it).velocity().y) == 1)
                (*it).velocity((*it).velocity().x, (*it).velocity().y * (-1));
            (*it).evolve_p();                    // fai muovere la pallina
            (*it).evolve_v();                    // varia un po' la velocita della pallina
            if ((*it).infection() == 1){         // guarda la distanza con le altre palline solo se la persona è infetta
              for(long unsigned int i{0}; i != people.size() - 1; i++){
                if ((*it).distance(people[i]) <=  infection_distance && people[i].infection() == 0) { // se un suscettibile è vicino a   
                    if(n < 25){                                                                      // n_infected PROBABILMENTE si infetta
                       if(people[i].infection() != 1){
                        ++n_infected;
                        --n_infectable;
                        people[i].infection(1);
                       }}}}}

            if((*it).infection() == 1){ // a volte gli infected 
                if(n == 25 || n == 26){
                  (*it).infection(3); // muoiono e si fermano
                  (*it).velocity(0.,0.);
                  --n_infected;
                  ++n_dead;
                  --n_people_alive;
                }   
                if(n ==  27 || n == 28 || n == 29){ // a volte guariscono
                  (*it).infection(2);
                  --n_infected;
                  ++n_recovered;
                }
            }
            if((*it).infection() == 2){ // a volte i recovered tornano sucettibili
                auto n{rand() % 800};
                if(n < 5){
                    (*it).infection(0);
                    ++n_infectable;
                    --n_recovered;
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

             if((*it).infection() == 3){ // i n_dead dopo un po' scompaiono
                if(n < 40){
                    people.erase(it);
                }
            } 
            }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
           Person p{};
           auto mouse_position = sf::Mouse::getPosition();
           p.position(mouse_position.x - 12, mouse_position.y - 60);
           p.velocity(((rand() % (pace + 1)) - pace/2) / 20., ((rand() % (pace + 1)) - pace/2) / 20.);
           p.setRadius(radius);
           ++n_infectable; 
           ++n_people_alive;
           people.reserve(people.size()+10);  // metto dello spazio in piu per sicurezza che senno si bugga
           people.push_back(p);
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right)){
           Person p{};
           auto mouse_position = sf::Mouse::getPosition();
           p.position(mouse_position.x - 12, mouse_position.y - 60);
           p.velocity(((rand() % (pace + 1)) - pace/2) / 20., ((rand() % (pace + 1)) - pace/2) / 20.);
           p.setRadius(radius);
           p.infection(1);
           ++n_infected; 
           ++n_people_alive;
           people.reserve(people.size()+10);
           people.push_back(p);
        }  

        assert(n_infectable+n_infected+n_recovered == n_people_alive);

        auto infectable = "Infectable:  " + std::to_string(n_infectable);
        auto infected = "Infected:  " + std::to_string(n_infected);
        auto recovered = "Recovered:  " + std::to_string(n_recovered);
        auto dead = "Dead:  " + std::to_string(n_dead);
        auto info_1 = "More than fifty dead";
        auto info_2 = "More than one hundred dead";
        auto info_3 = "More than two hundred dead";
        auto info_4 = "More than five hundred dead";
        auto info_5 = "More than one thousand dead";
        auto info_6 = "More than an half of the people are now infected,\nsurvivance of humanity's at risk";
        text[0].setString(infectable);
        text[1].setString(infected);
        text[2].setString(recovered);
        text[3].setString(dead);
        
        window.clear(sf::Color::White);
        if(n_dead <= 100 && n_dead > 50){
            text[4].setString(info_1);
            window.draw(text[4]);}
        if(n_dead <= 200 && n_dead > 100){
            text[4].setString(info_2);
            window.draw(text[4]);}
        if(n_dead <= 500 && n_dead > 200){
            text[4].setString(info_3);
            window.draw(text[4]);}
        if(n_dead <= 1000 && n_dead > 500){
            text[4].setString(info_4);
            window.draw(text[4]);}
        if(n_dead > 1000){
            text[4].setString(info_5);
            window.draw(text[4]);}
        if(n_infected > n_people_alive/2){
            text[5].setString(info_6);
            window.draw(text[5]);} 
        for(auto it = people.begin(); it != people.end(); ++it) window.draw((*it).circle()); // disegna tutte le palline
        for(int i{0}; i != 4; ++i){
            window.draw(text[i]);}
        window.display();

    }  
    if(n_infected == 0 && n_dead != n_tot) {  
        people.erase(people.begin(), people.end());
        text.erase(text.begin(), text.end());
        sf::Text the_end;
        the_end.setPosition(300,300);
        the_end.setCharacterSize(100);
        the_end.setFont(data_font);
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
    if(n_people_alive == 0){
        people.erase(people.begin(), people.end());
        text.erase(text.begin(), text.end());
        sf::Text the_end;
        the_end.setPosition(300,300);
        the_end.setCharacterSize(100);
        the_end.setFont(data_font);
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