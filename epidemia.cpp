#include "epidemia.hpp"
#include <cmath>

void epidemia::evolve(){      //Per chiamarlo: covid.evolve();
    popolazione prima{};     //creo una variabile aleatoria in cui salvarmi i valori iniziali (i valori i-1 per intenderci)

    prima.S = p_.S;          //Mi salvo i valori iniziali di p1 in prima
    prima.I = p_.I;
    prima.R = p_.R;
    double tot = prima.S + prima.I + prima.R; //In futuro creare una funzione membro di epidemia che ritorni N
    p_.S = prima.S - (beta * prima.S * prima.I) / tot;        //Calcolo le variazioni a partire da quelli iniziali
    p_.I = prima.I + (beta * prima.S * prima.I) / tot - (gamma * prima.I);
    p_.R = prima.R + gamma * prima.I;
    //assert((double)N() == tot); 
}

void epidemia::evolve_t(int t){         //chiama evolve t volte banalmente
    for(int i = 0; i != t; ++i) evolve();
}

void epidemia::stampa_p(){
    popolazione p = approssima();               //Salva la popolazione approssimata in una variabile aleatoria che serve 
    std::cout<<p.S<<" "<<p.I<<" "<<p.R<<'\n';   //solo qua per stamparla
}

popolazione epidemia::p(){
    return {p_.S, p_.I, p_.R};
}

int epidemia::N(){
    return N_;
}

popolazione epidemia::approssima(){    //funzione superflua, alla fine possiamo pure levarla completamente e mettere round 
                                       //direttamente sul cout in "stampa_p", se tutto funziona ovviamente
    int tot = N();
    popolazione p_intero{};
    p_intero.I = round(p_.I);
    p_intero.R = round(p_.R);
    p_intero.S = tot - p_intero.I - p_intero.R;

    assert(p_intero.S + p_intero.I + p_intero.R == tot); //controllo N
    return p_intero;
}

void evolve(epidemia x){    //Per chiamarlo: evolve(covid); 
    x.evolve();             //Non funziona! non so il perché
}  