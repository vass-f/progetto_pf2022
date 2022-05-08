#include "epidemia.hpp"

void epidemia::evolve(){      //Per chiamarlo: covid.evolve();
    popolazione prima{};     //creo una variabile aleatoria in cui salvarmi i valori iniziali (i valori i-1 per intenderci)

    prima.S = p_.S;          //Mi salvo i valori iniziali di p1 in prima
    prima.I = p_.I;
    prima.R = p_.R;
    int N = prima.S + prima.I + prima.R; //In futuro creare una funzione membro di epidemia che ritorni N
    p_.S = prima.S - (beta * prima.S * prima.I) / N;        //Calcolo le variazioni a partire da quelli iniziali
    p_.I = prima.I + (beta * prima.S * prima.I) / N - (gamma * prima.I);
    p_.R = prima.R + gamma * prima.I;
}

void epidemia::evolve_t(int t){         //chiama evolve t volte banalmente
    for(int i = 0; i != t; ++i) evolve();
}

void epidemia::stampa_p(){
    popolazione p = approssima();               //Salva la popolazione approssimata in una variabile aleatoria che serve 
    std::cout<<p.S<<" "<<p.I<<" "<<p.R<<'\n';   //solo qua per stamparla
}

/*popolazione epidemia::p(){
    return {p_.S, p_.I, p_.R};
}*/

int epidemia::N(){
    return p_.S + p_.I + p_.R;
}

popolazione epidemia::approssima(){
    int tot = N();
    popolazione p_intero{};
    p_intero.I = (int)p_.I;
    p_intero.R = (int)p_.R;
    p_intero.S = tot - p_intero.I - p_intero.R;

    return p_intero;
}

void evolve(epidemia x){    //Per chiamarlo: evolve(covid);
    x.evolve();
}