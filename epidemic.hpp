#ifndef EPIDEMIC_HPP
#define EPIDEMIC_HPP

#include <algorithm>
#include <iostream>
#include <cassert>
#include <cmath>

struct population{
    double S{}, I{}, R{};
};

inline bool operator==(population p1, population p2){
    return p1.S == p2.S && p1.I == p2.I && p1.R == p2.R;
}

class epidemic{
    private:
        double beta_{};
        double gamma_{};
        population p_{};
        int N_{};
    public:
        epidemic(double b, double g, population p): beta_{b}, gamma_{g}, p_{p}{     //Costruttore
            if(beta_ >= 1 || beta_ <= 0 || gamma_ >= 1 || gamma_ <= 0){  //se beta e gamma non sono nell'intervallo stampa l'errore
                throw std::runtime_error{"non valid parameters"};
            }
            p_.S = round(p_.S);
            p_.I = round(p_.I);
            p_.R = round(p_.R);

            N_ = p_.S + p_.I + p_.R;
            if(N_ <= 0){
                if (N_ == 0) throw std::runtime_error{"cannot be zero population"};
                else throw std::runtime_error{"cannot be negative population"};
            }

            if(p_.S < 0 || p_.I < 0 || p_.R < 0){
                throw std::runtime_error{"cannot be negative population"};
            }
        }

        void evolve();         //covid.evolve();
        void evolve_t(int t);  //chiama evolve t volte
        void print_p();         //stampa la population
        population state();        //ritorna la population (l'ho fatta visto che è privata, per ora serve solo per i test)
        int N();                    //ritorna la population totale, che deve essere costante
        population approx();  //approx la population a interi ma la salva in un'altra variabile di tipo population
        double tot();                 //Ritorna la somma degli elementi della variabile population, creata per fare il check sulla
                                      //conservazione di N
        bool IsOnGoing();
};

void evolve(epidemic x);

#endif 