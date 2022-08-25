//Compilare con
//g++ -Wall -Wextra -fsanitize=address -lsfml-system -lsfml-graphics -lsfml-window epidemia.cpp isto_sfml.cpp main_epidemia.cpp
#include "epidemia.hpp"
#include "isto.hpp"
#include "isto_sfml.hpp"

int main(){
    double beta, gamma;
    popolazione p{};
    std::cout<<"Benvenuto/a\n";
    char scelta = ' ';
    while(scelta != 'x' && (scelta == 'a' || scelta == 'b' || scelta == 'c' || scelta == ' ')){
        scelta = ' ';
        std::cout<<"Inserisci il numero totale della popolazione: ";
        std::cin>>p.S;
        std::cout<<"Inserisci il numero di infetti: ";
        std::cin>>p.I;
        p.S -= p.I;
        std::cout<<"Inserisci un valore per beta e un valore per gamma\nBeta: ";
        std::cin>>beta;
        std::cout<<"Gamma: ";
        std::cin>>gamma;
        epidemia virus(beta, gamma, p);
        while(scelta != 'x' && scelta != 'c'){
          std::cout<<"Cosa vuoi visualizzare?\nDigita 'a' per l'evoluzione numerica della pandemia";
          std::cout<<"\nDigita 'b' per la visualizzazione grafica della pandemia\nDigita 'c' per inserire altri valori\n";
          std::cout<<"Digita 'x' per terminare il programma\n";
          std::cin>>scelta;
          if(scelta == 'a'){
            int giorni;
            std::cout<<"Quanti giorni di evoluzione vuoi visualizzare? (Inserisci qualsiasi numero non positivo per farla andare finché non finisce) ";
            std::cin>>giorni;
            epidemia clone = virus;
            std::cout<<"S I R\n";
            clone.stampa_p();
            if(giorni <= 0){
              while(clone.IsOnGoing()){
                  clone.evolve();
                  clone.stampa_p();
                  sf::sleep(sf::milliseconds(200));

                  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) break; //Non funziona perché credo non ci sia nessuna finestra aperta
                                                                              //e sul terminale non ha effetto sfml
                  //Aggiungere una cosa che permette di uscire dalla visualizzazione dei dati schiacciando esc
                  //e fare in modo di uscire se l'epidemia finisce, probabilmente meglio mettere una funzione all'interno
                  //della stessa classe epidemia (bool?)
              }
            }
            else{
              for(int i = 0; i != giorni; ++i){
                clone.evolve();
                clone.stampa_p();
                sf::sleep(sf::milliseconds(200));
              }
            }
          }

          if(scelta == 'b'){
            char s{};
            std::cout<<"Che grafico ti inseressa?\nDigita 'a' per i suscettibili\nDigita 'b' per gli infetti\nDigita 'c' per i rimossi\n";
            std::cin>>s;
            epidemia clone = virus;
            std::string mod{};
            if(s == 'a') mod = "Suscettibili";
            if(s == 'b') mod = "Infetti";
            if(s == 'c') mod = "Rimossi";
            Finestra finestra("Giorni", mod);
            if(s == 'a') finestra.add(clone.approssima().S);
            if(s == 'b') finestra.add(clone.approssima().I);
            if(s == 'c') finestra.add(clone.approssima().R);
            while(finestra.isOpen() && clone.IsOnGoing()){
                clone.evolve();
                if(s == 'a') finestra.add(clone.approssima().S);
                if(s == 'b') finestra.add(clone.approssima().I);
                if(s == 'c') finestra.add(clone.approssima().R);

                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                    finestra.close();
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)){
                  clone.evolve();
                  if(s == 'a') finestra.add(clone.approssima().S);
                  if(s == 'b') finestra.add(clone.approssima().I);
                  if(s == 'c') finestra.add(clone.approssima().R);
                }

                finestra.draw();
                sf::sleep(sf::milliseconds(400));
            }
          }
        };
    };
}