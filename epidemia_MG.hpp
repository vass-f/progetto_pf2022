#ifndef prima_prova_MG.hpp
#define prima_prova_MG.hpp

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

// class persone : public sf::Transformable, public sf::Drawable
// {
// public:
// //qui si mettono le funzioni per giocare con posizione e colori delle persone
// private:
//   virtual void draw(sf::RenderTarget& persona, sf::RenderStates stato);  const {
//     stato.transform *= getTransform();
//     stato.texture = &miatexture;//applico le texture
//     persona.draw(m_vertici, stato);
//   }
// };
// //persone persona;
// sf::VertexArray m_vertici;
// sf::Texture miatexture;


struct popolazione {
  double S{};
  double I{};    
  double R{};    //suscettibili, infetti, rimossi
};

class epidemia {
 private:
  double beta{};
  double gamma{};  // entrambi tra 0 e 1

  //int popol_tot = prima.S + prima.I + prima.R;    
  // check ad ogni delta_t se popol_tot = S + I + M

 public:
 epidemia(double b, double g): beta{b}, gamma{g} {
   if (beta > 1 || beta < 0 || gamma > 1 ||
        gamma < 0)  // check per i parametri gamma e beta
    {
      throw std::runtime_error{"non valid parameters"};
    }}
    bool const check() {} //check per i parametri beta e gamma, dobbiamo toglierlo dalla funzione

  // std::vector<persone> prima{};     //creo due vettori che mi salvino lo
  // stato della mia popolazione std::vector<persone> dopo;        //risultato
  // della funzione evolve
  //popolazione evoluzione_mono(popolazione inizio) : prima{inizio} {};  // costruttore, 
  // popolazione operator()(popolazione const& p1) {
     popolazione evoluzione_mono(popolazione p1) {
    popolazione prima{};
    
    prima.S = p1.S;
    prima.R = p1.R;
    prima.I = p1.I;
    int popol_tot = prima.S + prima.I + prima.R;
    p1.S = prima.S - (beta * prima.S * prima.I) / popol_tot;
    p1.I = prima.I + (beta * prima.S * prima.I) / popol_tot - (gamma * prima.I);
    p1.R = prima.R + gamma * prima.I;
    return p1;
  }
/*
std::vector<int> const& state() const { 
    std::vector<int> stato;
    stato[0] = prima.S;
    stato[1] = prima.R;
    stato[2] = prima.I;
    return stato; }

    popolazione evoluzione_ciclo(popolazione inizio, double tempo_tot) {
        double intervallo = 1;   //tempo di un intervallo
        int intervalli = tempo_tot/intervallo;
        for (int i = 0; i < intervalli; i++)
        {
            evoluzione_mono(inizio);  //serve solo lo stato iniziale per fare quel singolo step
        }
        return prima;    }
*/
};

#endif