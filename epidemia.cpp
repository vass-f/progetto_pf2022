#include "epidemia.hpp"
#include <cmath>

void epidemia::evolve(){      //Per chiamarlo: covid.evolve();
    popolazione prima{};     //creo una variabile aleatoria in cui salvarmi i valori iniziali (i valori i-1 per intenderci)

    prima.S = p_.S;          //Mi salvo i valori iniziali di p1 in prima
    prima.I = p_.I;
    prima.R = p_.R;
    
    p_.S = prima.S - (beta_ * prima.S * prima.I) / N();        //Calcolo le variazioni a partire da quelli iniziali
    p_.I = prima.I + (beta_ * prima.S * prima.I) / N() - (gamma_ * prima.I);
    p_.R = prima.R + gamma_ * prima.I;
    assert(round(p_.S + p_.I + p_.R) == N());     //round per evitare che (per esempio) 99.99999999 approssimi a 99
}

void epidemia::evolve_t(int t){         //chiama evolve t volte banalmente
    for(int i = 0; i != t; ++i) evolve();
}

void epidemia::stampa_p(){
    popolazione p = approssima();               //Salva la popolazione approssimata in una variabile aleatoria che serve 
    std::cout<<p.S<<" "<<p.I<<" "<<p.R<<'\n';   //solo qua per stamparla
}

popolazione epidemia::state(){
    return {p_.S, p_.I, p_.R};
}

int epidemia::N(){
    return N_;
}

popolazione epidemia::approssima(){
    popolazione p_intero{};
    p_intero.I = round(p_.I);
    p_intero.R = round(p_.R);
    p_intero.S = N() - p_intero.I - p_intero.R;

    assert(p_intero.S + p_intero.I + p_intero.R == N()); //controllo N
    return p_intero;
}

void evolve(epidemia x){    //Per chiamarlo: evolve(covid); 
    x.evolve();             //Non funziona! non so il perché
}  

double epidemia::tot(){
    return round(p_.S + p_.I + p_.R);
}