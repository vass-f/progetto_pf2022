#ifndef EPIDEMIA_HPP
#define EPIDEMIA_HPP

#include <algorithm>
#include <iostream>
#include <cassert>

struct popolazione{
    double S{}, I{}, R{};
};

inline bool operator==(popolazione p1, popolazione p2){
    return p1.S == p2.S && p1.I == p2.I && p1.R == p2.R;
}

class epidemia{
    private:
        double beta_{};
        double gamma_{};
        popolazione p_{};
        int N_{};
        

    public:
        epidemia(double b, double g, popolazione p): beta_{b}, gamma_{g}, p_{p}{     //Costruttore
            if(beta_ >= 1 || beta_ <= 0 || gamma_ >= 1 || gamma_ <= 0){  //se beta e gamma non sono nell'intervallo stampa l'errore
                throw std::runtime_error{"non valid parameters"};
            }
            N_ = p_.S + p_.I + p_.R;
        }

        void evolve();         //covid.evolve();
        void evolve_t(int t);  //chiama evolve t volte
        void stampa_p();         //stampa la popolazione
        popolazione state();        //ritorna la popolazione (l'ho fatta visto che è privata, per ora serve solo per i test)
        int N();                    //ritorna la popolazione totale, che deve essere costante
        popolazione approssima();  //approssima la popolazione a interi ma la salva in un'altra variabile di tipo popolazione
        double tot();                 //Ritorna la somma degli elementi della variabile popolazione, creata per fare il check sulla
                                      //conservazione di N
        bool IsOnGoing();  //True se l'epidemia sta andando, false se è ferma (infetti = 0)
};

void evolve(epidemia x);

#endif 