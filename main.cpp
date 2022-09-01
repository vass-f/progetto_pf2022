// To compile
// g++ -Wall -Wextra -fsanitize=address -lsfml-system -lsfml-window
// -lsfml-graphics epidemic.cpp isto_sfml.cpp circles.cpp main.cpp
#include "circles.hpp"
#include "epidemic.hpp"
#include "isto.hpp"
#include "isto_sfml.hpp"

/*
The program is divided into two parts. The first is the SIR model. It simulates
an epidemic, with a population and two parameters taken in input. The evolution
of the epidemic can be visualized as numbers on terminal, or as an histogram on
a graphic window. It uses epidemic and Finestra classes. The second one is
cellular automata. Using the class Person, it creates moving circles on screen,
some of team are infected (red), can sometimes infect other healthy circles
(blue), and sometimes die (black) or heal (green). The healed circles can go
back to be blue (susceptible). The user choose the number of circles and
infected.
*/

int main() {
  char scelta_ = ' ';
  std::cout << "Welcome\n";
  while (scelta_ != 'x') {
    scelta_ = ' ';
    std::cout << "Choose the program you're interested in.\na) SIR "
                 "model\nb) Cellular automata\nx) Exit\n";
    std::cin >> scelta_;
    std::cout << '\n';

    //////////////////////
    // PROGRAMMA EPIDEMIA//
    //////////////////////

    if (scelta_ == 'a') {
      double beta, gamma;
      population p{};
      char scelta = ' ';
      while (scelta != 'x' && (scelta == 'a' || scelta == 'b' ||
                               scelta == 'c' || scelta == ' ')) {
        scelta = ' ';
        std::cout << "Enter the number of people: ";
        std::cin >> p.S;
        std::cout << "Enter the number of infected people: ";
        std::cin >> p.I;
        p.S -= p.I;
        std::cout << "Enter the values of Beta and Gamma\nBeta "
                     "(Infection's probability): ";
        std::cin >> beta;
        std::cout
            << "Gamma (Disease's end probability): ";  // formally it's the
                                                       // probability of
                                                       // recovering or dying
        std::cin >> gamma;
        epidemic virus(beta, gamma, p);

        while (scelta != 'x' && scelta != 'c') {
          std::cout << "what do you want to display?\nPress 'a' for the "
                       "numerical evolution of the epidemic";
          std::cout << "\nPress 'b' for the graphics evolution of the "
                       "epidemic\nPress 'c' to enter another values\n";
          std::cout << "Press 'x' to go back to the menù\n";
          std::cin >> scelta;
          std::cout << '\n';
          if (scelta == 'a') {
            int giorni;
            std::cout << "How many days do you want to see? "
                         "(Enter any non-positive number to watch until "
                         "epidemic's end)";
            std::cin >> giorni;
            epidemic clone = virus;
            std::cout << "S I R\n";
            clone.print_p();
            if (giorni <= 0) {
              while (clone.IsOnGoing()) {
                clone.evolve();
                clone.print_p();
                sf::sleep(sf::milliseconds(250));
              }
            } else {
              for (int i = 0; i != giorni; ++i) {
                clone.evolve();
                clone.print_p();
                sf::sleep(sf::milliseconds(250));
              }
            }
          }

          if (scelta == 'b') {
            char s{};
            std::cout << "What graphic are you interested in?\nPress 'a' for "
                         "susceptible\nPress 'b' for infected\nPress 'c' "
                         "for removed\n";
            std::cin >> s;
            std::cout << '\n';
            epidemic clone = virus;
            std::string mod{};
            if (s == 'a') mod = "Susceptible";
            if (s == 'b') mod = "Infected";
            if (s == 'c') mod = "Removed";
            Finestra finestra("Days", mod);
            if (s == 'a') finestra.add(clone.approx().S);
            if (s == 'b') finestra.add(clone.approx().I);
            if (s == 'c') finestra.add(clone.approx().R);

            while (finestra.isOpen()) {
              if (sf::Keyboard::isKeyPressed(
                      sf::Keyboard::Escape)) {  // Close the window if esc has
                                                // been pressed
                finestra.close();
              }
              if (clone.IsOnGoing()) {  // With this condition the window can
                                        // stay open even though the epidemic has
                                        // ended
                if (sf::Keyboard::isKeyPressed(
                        sf::Keyboard::Space)) {  // Stop everything, to undone
                                                 // it press space again
                  bool space = true;
                  sf::sleep(sf::milliseconds(150));
                  while (space) {
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                      space = false;
                  }
                }
                clone.evolve();
                if (s == 'a') finestra.add(clone.approx().S);
                if (s == 'b') finestra.add(clone.approx().I);
                if (s == 'c') finestra.add(clone.approx().R);

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                  clone.evolve();
                  if (s == 'a') finestra.add(clone.approx().S);
                  if (s == 'b') finestra.add(clone.approx().I);
                  if (s == 'c') finestra.add(clone.approx().R);
                }
                sf::sleep(sf::milliseconds(400));
              }
              finestra.draw();
            }
          }
        };
      };
    }

    //////////////////////////////
    // PROGRAMMA AUTOMI CELLULARI//
    //////////////////////////////

    if (scelta_ == 'b') {
      int infection_distance{60};
      int radius{10};  // circle's radius
      int first_infected{};
      int n_tot{};  // total number of people
      int const pace{30};
      int const fps{60};    // frame per second
      int const n_text{6};  // number of texts in the legend
      std::vector<sf::Text> text(n_text);
      sf::Font data_font;
      sf::Font comment_font;
      if (!data_font.loadFromFile("data_font.ttf")) {
        throw std::runtime_error{"Could not load data font"};
      }
      if (!comment_font.loadFromFile("comment_font.ttf")) {
        throw std::runtime_error{"Could not load comment font"};
      }
      for (int i{0}; i != 4; ++i) {
        text[i].setFont(comment_font);
      }
      for (int i{4}; i != 6; ++i) {
        text[i].setFont(data_font);
      }

      text[0].setFillColor(sf::Color::Blue);
      text[1].setFillColor(sf::Color::Red);
      text[2].setFillColor(sf::Color::Green);
      text[3].setFillColor(sf::Color::Black);
      text[4].setFillColor(sf::Color::Black);
      text[5].setFillColor(sf::Color::Red);
      for (int i{0}; i != n_text; ++i) {
        text[i].setPosition(20, 10 + 30 * i);
      }

      sf::Clock clock;
      srand(time(0));

      std::cout << "Set the initial number of people (maximum 350)" << '\n';
      std::cin >> n_tot;
      if (n_tot <= 0 || n_tot > 350) {
        throw std::runtime_error{"Non valid parameters"};
      }
      std::cout << "Set the initial number of infected people" << '\n';
      std::cin >> first_infected;
      if (first_infected > n_tot) {
        throw std::runtime_error{
            "Non valid parameters, infected people can't be more than the "
            "total number of people"};
      }
      if (first_infected <= 0) {
        throw std::runtime_error{"Non valid parameters"};
      }

      int n_people_alive{n_tot};
      int n_infectable{n_tot - first_infected};
      int n_infected{first_infected};
      int n_recovered{};
      int n_dead{};

      unsigned const display_width = sf::VideoMode::getDesktopMode().width;
      unsigned const display_height = sf::VideoMode::getDesktopMode().height;

      sf::RenderWindow window(sf::VideoMode(display_width, display_height),
                              "Covid epidemic");
      window.setFramerateLimit(fps);

      std::vector<Person> people;        // people vector
      for (int i{0}; i != n_tot; ++i) {  // insert n_tot people
        Person p{};
        p.position((rand() % (display_width - 20)),
                   (rand() % (display_height - 70)));
        p.velocity(((rand() % (pace + 1)) - pace / 2) / 20.,
                   ((rand() % (pace + 1)) - pace / 2) / 20.);
        p.setRadius(radius);
        people.push_back(p);
      }
      for (int i{0}; i != first_infected; ++i) {  // insert infected people
        people[i].infection(1);
      }
      assert(!people.empty());

      while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
          if (event.type == sf::Event::Closed) window.close();
        }

        if (n_infected != 0) {
          for (auto it = people.begin(); it < people.end(); ++it) {
            auto n{rand() % 2000};  // random number that will be used for
                                    // recovering and death rate
            if ((*it).position().x < 0.1 &&
                (*it).velocity().x <
                    0)  // people "bounce" at the end of the screen
            {
              (*it).velocity((*it).velocity().x * (-1), (*it).velocity().y);
            }  // the information about the sign is used because only
            if ((*it).position().y < 0.1 &&
                (*it).velocity().y <
                    0)  // people that are going directed against the wall
                        // should "bounce", if not sometimes
            {
              (*it).velocity((*it).velocity().x, (*it).velocity().y * (-1));
            }  // could happen that some people get trapped at the end of the
               // screen
            if ((*it).position().x > (display_width - 20) &&
                (*it).velocity().x > 0) {
              (*it).velocity((*it).velocity().x * (-1), (*it).velocity().y);
            }
            if ((*it).position().y > (display_height - 70) &&
                (*it).velocity().y > 0) {
              (*it).velocity((*it).velocity().x, (*it).velocity().y * (-1));
            }

            assert((*it).position().x > -25 &&
                   (*it).position().x < (display_width) + 5 &&
                   (*it).position().y > -25 &&
                   (*it).position().y <
                       display_height -
                           45);  // assert people are inside borders

            (*it).evolve_p();
            (*it).evolve_v();
            (*it).setPosition((*it).position().x, (*it).position().y);
            if ((*it).infection() ==
                1) {  // if this person is infected, he can infectate close
                      // people with some probability
              for (unsigned int i{0}; i != people.size(); i++) {
                if ((*it).distance(people[i]) <= infection_distance &&
                    people[i].infection() == 0) {
                  auto a{rand() % 80};
                  if (a == 1) {
                    ++n_infected;
                    --n_infectable;
                    people[i].infection(1);
                  }
                }
              }
            }

            if ((*it).infection() == 1) {  // infected people
              if (n == 25 || n == 26) {
                (*it).infection(3);  // can die
                (*it).velocity(0., 0.);
                --n_infected;
                ++n_dead;
                --n_people_alive;
              }
              if (n == 27 || n == 28 || n == 29) {
                (*it).infection(2);  // or recover
                --n_infected;
                ++n_recovered;
              }
            }
            if ((*it).infection() ==
                2) {  // recovered can become infectable again
              if (n < 35 && n > 29) {
                (*it).infection(0);
                ++n_infectable;
                --n_recovered;
              }
            }

            if ((*it).infection() == 0) {
              (*it).setFillColor(sf::Color::Blue);
            }
            if ((*it).infection() == 1) {
              (*it).setFillColor(sf::Color::Red);
            }
            if ((*it).infection() == 2) {
              (*it).setFillColor(sf::Color::Green);
            }
            if ((*it).infection() == 3) {
              (*it).setFillColor(sf::Color::Black);
            }

            if ((*it).infection() ==
                3) {  // dead people can disappear with some probability
              if (n > 34 && n < 75) {
                people.erase(it);
              }
            }
          }

          if (sf::Mouse::isButtonPressed(
                  sf::Mouse::Left)) {  // with mouse left click infectable
                                       // people will spawn
            Person p{};
            auto mouse_position = sf::Mouse::getPosition();
            p.position(mouse_position.x - 12, mouse_position.y - 60);
            p.velocity(((rand() % (pace + 1)) - pace / 2) / 20.,
                       ((rand() % (pace + 1)) - pace / 2) / 20.);
            p.setRadius(radius);
            ++n_infectable;
            ++n_people_alive;
            people.push_back(p);
          }
          if (sf::Mouse::isButtonPressed(
                  sf::Mouse::Right)) {  // with mouse right click infected
                                        // people will spawn
            Person p{};
            auto mouse_position = sf::Mouse::getPosition();
            p.position(mouse_position.x - 12, mouse_position.y - 60);
            p.velocity(((rand() % (pace + 1)) - pace / 2) / 20.,
                       ((rand() % (pace + 1)) - pace / 2) / 20.);
            p.setRadius(radius);
            p.infection(1);
            ++n_infected;
            ++n_people_alive;
            people.push_back(p);
          }

          if (sf::Keyboard::isKeyPressed(
                  sf::Keyboard::Escape)) {  // Close the window if esc has been
                                            // pressed
            window.close();
          }

          assert(n_infectable + n_infected + n_recovered == n_people_alive);

          auto infectable =
              "Infectable:  " +
              std::to_string(
                  n_infectable);  // some information about the epidemic
          auto infected = "Infected:  " + std::to_string(n_infected);
          auto recovered = "Recovered:  " + std::to_string(n_recovered);
          auto dead = "Dead:  " + std::to_string(n_dead);
          auto info_1 = "More than fifty dead";
          auto info_2 = "More than hundred dead";
          auto info_3 = "More than two hundred dead";
          auto info_4 = "More than five hundred dead";
          auto info_5 = "More than one thousand dead";
          auto info_6 =
              "More than an half of the people are now infected,\nsurvivance "
              "of humanity's at risk";
          text[0].setString(infectable);
          text[1].setString(infected);
          text[2].setString(recovered);
          text[3].setString(dead);

          window.clear(sf::Color::White);
          if (n_dead <= 100 && n_dead > 50) {
            text[4].setString(info_1);
            window.draw(text[4]);
          }
          if (n_dead <= 200 && n_dead > 100) {
            text[4].setString(info_2);
            window.draw(text[4]);
          }
          if (n_dead <= 500 && n_dead > 200) {
            text[4].setString(info_3);
            window.draw(text[4]);
          }
          if (n_dead <= 1000 && n_dead > 500) {
            text[4].setString(info_4);
            window.draw(text[4]);
          }
          if (n_dead > 1000) {
            text[4].setString(info_5);
            window.draw(text[4]);
          }
          if (n_infected > n_people_alive / 2) {
            text[5].setString(info_6);
            window.draw(text[5]);
          }

          for (auto it = people.begin(); it != people.end(); ++it)
            window.draw((*it).circle());  // draw all the people
          for (int i{0}; i != 4; ++i) {   // draw the text
            window.draw(text[i]);
          }
          window.display();
        }

        if (n_infected == 0 &&
            n_people_alive !=
                0) {  // if there are no infected, virus is defeated
          sf::Text the_end;
          the_end.setPosition(display_width / 7, display_height / 3);
          the_end.setCharacterSize(display_height / 10);
          the_end.setFont(data_font);
          the_end.setString("Humanity defeated Coronavirus!!!");
          int t = clock.getElapsedTime().asSeconds();
          if (t % 2 == 0) {
            window.clear(sf::Color::Cyan);
            the_end.setFillColor(sf::Color::Black);
          } else {
            window.clear(sf::Color::Blue);
            the_end.setFillColor(sf::Color::Green);
          }
          window.draw(the_end);
          window.display();
        }
        if (n_people_alive ==
            0) {  // if all the people die, humanity no longer exists
          sf::Text the_end;
          the_end.setPosition(display_width / 7, display_height / 3);
          the_end.setCharacterSize(display_height / 10);
          the_end.setFont(data_font);
          the_end.setString(
              "Coronavirus defeated Humanity \n                           Try "
              "again");
          int t = clock.getElapsedTime().asSeconds();
          if (t % 2 == 0) {
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
    }
  }
}