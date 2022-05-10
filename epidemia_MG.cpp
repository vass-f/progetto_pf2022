#include "epidemia_MG.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

int main() {
  int tempo_tot{};
  int persone_tot{};
  int I_iniz{};
  std::cout << "scegliere la durata totale in giorni:  ";
  std::cin >> tempo_tot;
  std::cout << "scegliere numero totale di individui:  ";
  std::cin >> persone_tot;
  std::cout << "scegliere infetti iniziali: ";
  std::cin >> I_iniz;

  if (persone_tot < I_iniz)  // primo check
  {
    throw std::runtime_error(
        "numero di infetti maggiore della popolazione totale.");
  }
  epidemia covid(0.75, 0.25);   //BETA GAMMA
  if (covid.check() == 0)  // secondo check
  {
    throw std::runtime_error("valori di apha e/o beta non consentiti.");
  }

  double suscettibili = persone_tot - I_iniz;
  popolazione inizio{suscettibili, I_iniz,
                     0};  // creo la situazione iniziale di tipo "popolazione"

  for (int i = 0; tempo_tot > i; i++) {
    std::cout <<"SANI:  "<< inizio.S << "  INFETTI:  " << inizio.I << "  RIMOSSI:  " << inizio.R << "   "
              << inizio.S + inizio.I + inizio.R << std::endl;
    inizio = covid.evoluzione_mono(inizio);
  }

  //      ++PARTE GRAFICA++
  // sf::RenderWindow window(sf::VideoMode(500, 700), "simulazione pandemica");
  // window.setFramerateLimit(60);
  // while (window.isOpen()) {
  //   sf::Event event;
  //   while (window.pollEvent(event)) {
  //     switch (event.type) {
  //       case sf::Event::KeyPressed:
  //         /* code */  // cicli if per capire quale tasto è stato premuto
  //         break;

  //       case sf::Event::Closed:
  //         window.close();
  //         break;
  //       case sf::Event::Resized:
  //         std::cout << "nuova altezza " << event.size.height
  //                   << "     nuova larghezza " << event.size.width << std::endl;
  //         break;  // quando la finestra cambia di dimensioni scrive le
  //                 // dimensioni aggiornate
  //       default:
  //         break;
  //     }
  //   }
    
    //sf::Vertex persona(sf::Vector2f(10.f, 50.f), sf::Vector2f(100.f, 100.f));
    //usare i vertex non conviene visto che sono sempre e solo un pixel, scrivo la mia classe "persone"

    // switch (stato persona)
    // {
    // case /* malata */:
    //     persona.color = sf::Color::Red;
    //     break;

    // case /* sana */:
    //     persona.color = sf::Color::White;
    //     break;
    
    // case /* rimossa */:
    //     persona.color = sf::Color::Blue;
    //     break;
    
    // default:
    //     break;
    // }
    
    //window.clear(sf::Color::Black);
    //window.draw();
    //window.display();
 // }
}