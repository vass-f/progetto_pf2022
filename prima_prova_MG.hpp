#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

struct popolazione {
  int S{}, I{}, R{};
};

class epidemia {
 private:
  int beta{};
  int gamma{};  // entrambi tra 0 e 1

  int popol_tot = prima.S + prima.I + prima.R;
  // check ad ogni delta_t se popol_tot = S + I + M

 public:
 
 bool check() { //check per i parametri beta e gamma, dobbiamo toglierlo dalla funzione
  if (beta > 1 && beta < 0 && gamma > 1 &&
      gamma < 0)  // check per i parametri gamma e beta
  {
    throw std::runtime_error{"non valid parameters"};
  };}
  // std::vector<persone> prima{};     //creo due vettori che mi salvino lo
  // stato della mia popolazione std::vector<persone> dopo;        //risultato
  // della funzione evolve
  popolazione prima;  // creo la mia popolazione iniziale (come oggetto della struc)
    double delta_t{};
  popolazione evoluzione_mono(popolazione inizio, double t) : prima{inizio}, delta_t{t} {};  // costruttore
    popolazione operator()(popolazione const& p1, double const& t) {
    popolazione dopo{};
    dopo.S = prima.S - (beta * prima.S * prima.I) / popol_tot;
    dopo.I = prima.I + (beta * prima.S * prima.I) / popol_tot - (gamma * prima.I);
    dopo.R = prima.R + gamma * prima.I;
    prima = dopo;  // poi bisogna rendere uguali i due oggetti come "aggiornamento"
    return prima;
  }

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
            evoluzione_mono(inizio, intervallo);
        }
        return prima;    }

};