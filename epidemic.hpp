#ifndef EPIDEMIC_HPP
#define EPIDEMIC_HPP

#include <algorithm>
#include <iostream>
#include <cassert>
#include <cmath>

/*
The following class implement a model that describe a epidemic's behavior.
The model divide people between three state: Susceptible, Infected and Removed (dead or healed).
Through 3 differential equation we can predict the behavior of the epidemic in a population, knowing the two parameters
that characterize an epidemic
*/

struct population{
    double S{}, I{}, R{};
};

inline bool operator==(population p1, population p2){
    return p1.S == p2.S && p1.I == p2.I && p1.R == p2.R;
}

class epidemic{
    private:
        double beta_{};     //Probability of being infected
        double gamma_{};    //Probability to die or to heal
        population p_{};
        int N_{};           //Number of people, which must be costant through time, even if we work with double
    public:
        epidemic(double b, double g, population p): beta_{b}, gamma_{g}, p_{p}{     //Constructor
            if(beta_ >= 1 || beta_ <= 0 || gamma_ >= 1 || gamma_ <= 0){  //If beta and gamma are out of range throw an error
                throw std::runtime_error{"non valid parameters"};
            }
            p_.S = round(p_.S); //Rounded input population for a better control on N_
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

        void evolve();              //evolve population through 3 equation
        void evolve_t(int t);       //call evolve t-time
        void print_p();             //print rounded population
        population state();         //return population (not rounded)
        int N();                    //return N_
        population approx();        //return population (rounded)
        double tot();               //return the sum of population's elements, created for testing purpose on N conservation
        
        bool IsOnGoing();           //True if the epidemic is on going, false on the opposite case
                                    //With it's on going we mean the infected aren't zero
};

#endif 