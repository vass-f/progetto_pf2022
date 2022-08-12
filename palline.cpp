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
    velocity(velocity().x - (((rand() % 10) - 4) / 8.), velocity().y);} // copysign is used beacause people should more probably decrease their velocity
  if(a == 2 && infection() != 3 && copysign(1, velocity().x) == -1){  // to get a more realistic motion effect
    velocity(velocity().x + (((rand() % 10) - 4) / 8.), velocity().y);}
  if(a == 3 && infection() != 3 && copysign(1, velocity().y) == 1){
    velocity(velocity().x, velocity().y - (((rand() % 10) - 4) / 8.));}
  if(a == 4 && infection() != 3 && copysign(1, velocity().y) == -1){
    velocity(velocity().x, velocity().y + (((rand() % 10) - 4) / 8.));}
} 

double Person::distance (Person& other_radius) { // calculates the distance between two people
  double d_x = position().x - other_radius.position().x;
  double d_y = position().y - other_radius.position().y;
  double d = sqrt(d_x * d_x + d_y * d_y);
  return d;
}

int main(){
    int infection_distance{60};  
    int radius{10}; // circle's radius
    int first_infected{}; 
    int n_tot{}; // total number of people
    int const pace{30}; 
    int const fps{60}; // frame per second
    int const n_text{6};  // number of texts in the legend
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
    srand(time(0));  
    
    std::cout<< "Set the initial number of people" << '\n';
    std::cin >> n_tot;
    std::cout<< "Set the initial number of infected people" << '\n';
    std::cin >> first_infected; 
        
    int n_people_alive{n_tot};
    int n_infectable{n_tot-first_infected};
    int n_infected{first_infected};
    int n_recovered{};
    int n_dead{};
    
    unsigned const display_width = sf::VideoMode::getDesktopMode().width;   
    unsigned const display_height = sf::VideoMode::getDesktopMode().height;

    sf::RenderWindow window(sf::VideoMode(display_width, display_height), "Covid epidemic");
    window.setFramerateLimit(fps);

    std::vector<Person> people;    // people vector
    for(int i{0}; i != n_tot; ++i){   // insert n_tot people
        Person p{};
        p.position((rand() % (display_width - 20)), (rand() % (display_height - 70)));
        p.velocity(((rand() % (pace + 1)) - pace/2) / 20., ((rand() % (pace + 1)) - pace/2) / 20.);
        p.setRadius(radius);
        people.push_back(p);
    }
    for(int i{0}; i != first_infected; ++i) {   // insert infected people
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
            auto n{rand() % 2000};              // random number that will be used for infection, recovering and death rate
            if((*it).infection() != 3){if((*it).position().x < 0.1 && copysign(1, (*it).velocity().x) == -1)   // people "bounce" at the end of the screen 
                                      {(*it).velocity((*it).velocity().x * (-1), (*it).velocity().y);}     // copysign function is used because only the people that are going directed against
                                      if((*it).position().y < 0.1 && copysign(1, (*it).velocity().y) == -1)  //the wall should "bounce", if not sometimes could happen that
                                      {(*it).velocity((*it).velocity().x, (*it).velocity().y * (-1));}     // some people get trapped at the end of the screen
                                      if((*it).position().x > (display_width - 20) && copysign(1, (*it).velocity().x) == 1)
                                      {(*it).velocity((*it).velocity().x * (-1), (*it).velocity().y);}   
                                      if((*it).position().y > (display_height - 70) && copysign(1, (*it).velocity().y) == 1) 
                                      {(*it).velocity((*it).velocity().x, (*it).velocity().y * (-1));}}  
            (*it).evolve_p();                    
            (*it).evolve_v();                    
            (*it).setPosition((*it).position().x, (*it).position().y);
            if ((*it).infection() == 1){         // if this person is infected, he can infectate close people with some probability
              for(long unsigned int i{0}; i != people.size(); i++){  
                if ((*it).distance(people[i]) <=  infection_distance && people[i].infection() == 0) {    
                    if(n < 25){                                                                    
                       if(people[i].infection() != 1){
                        ++n_infected;
                        --n_infectable;
                        people[i].infection(1);
                       }}}}}

            if((*it).infection() == 1){ // infected people 
                if(n == 25 || n == 26){
                  (*it).infection(3); // can die
                  (*it).velocity(0.,0.);
                  --n_infected;
                  ++n_dead;
                  --n_people_alive;
                }   
                if(n ==  27 || n == 28 || n == 29){ // or recover
                  (*it).infection(2);
                  --n_infected;
                  ++n_recovered;
                }
            }
            if((*it).infection() == 2){ // recovered can become infectable again
                if(n < 35 && n > 29){
                    (*it).infection(0);
                    ++n_infectable;
                    --n_recovered;
                }
            }

            if ((*it).infection() == 0){
             (*it).setFillColor(sf::Color::Blue);} 
            if ((*it).infection() == 1){
             (*it).setFillColor(sf::Color::Red);}
            if ((*it).infection() == 2){
             (*it).setFillColor(sf::Color::Green);}
            if ((*it).infection() == 3) {
             (*it).setFillColor(sf::Color::Black);}

             if((*it).infection() == 3){ // dead people can disappear with some probability
                if(n > 34 && n < 75){
                    people.erase(it);
                }
            } 
            }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){  // with mouse left click infectable people will spawn
           Person p{};
           auto mouse_position = sf::Mouse::getPosition();
           p.position(mouse_position.x - 12, mouse_position.y - 60);
           p.velocity(((rand() % (pace + 1)) - pace/2) / 20., ((rand() % (pace + 1)) - pace/2) / 20.);
           p.setRadius(radius);
           ++n_infectable; 
           ++n_people_alive;
           people.reserve(people.size()+2);  
           people.push_back(p);
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right)){  // with mouse right click infected people will spawn
           Person p{};
           auto mouse_position = sf::Mouse::getPosition();
           p.position(mouse_position.x - 12, mouse_position.y - 60);
           p.velocity(((rand() % (pace + 1)) - pace/2) / 20., ((rand() % (pace + 1)) - pace/2) / 20.);
           p.setRadius(radius);
           p.infection(1);
           ++n_infected; 
           ++n_people_alive;
           people.reserve(people.size()+2);
           people.push_back(p);
        }  

        assert(n_infectable+n_infected+n_recovered == n_people_alive);

        auto infectable = "Infectable:  " + std::to_string(n_infectable);  // some information about the epidemic
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
        for(auto it = people.begin(); it != people.end(); ++it) window.draw((*it).circle()); // draw all the people
        for(int i{0}; i != 4; ++i){  // draw the text
            window.draw(text[i]);}
        window.display();

    }  
    if(n_infected == 0 && n_people_alive != 0) {  // if there are no infected, virus is defeated
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
    if(n_people_alive == 0){          // if all the people die, humanity no longer exists
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