#include "epidemic.hpp"


void epidemic::evolve(){      //Per chiamarlo: covid.evolve();
    population before{};     //creo una variabile aleatoria in cui salvarmi i valori iniziali (i valori i-1 per intenderci)

    before.S = p_.S;          //Mi salvo i valori iniziali di p1 in before
    before.I = p_.I;
    before.R = p_.R;
    
    p_.S = before.S - (beta_ * before.S * before.I) / N();        //Calcolo le variazioni a partire da quelli iniziali
    p_.I = before.I + (beta_ * before.S * before.I) / N() - (gamma_ * before.I);
    p_.R = before.R + gamma_ * before.I;
    assert(round(p_.S + p_.I + p_.R) == N());     //round per evitare che (per esempio) 99.99999999 approssimi a 99
}

void epidemic::evolve_t(int t){         //chiama evolve t volte banalmente
    for(int i = 0; i != t; ++i) evolve();
}

void epidemic::print_p(){
    population p = approx();               //Salva la population approx  in una variabile aleatoria che serve 
    std::cout<<p.S<<" "<<p.I<<" "<<p.R<<'\n';   //solo qua per stamparla
}

population epidemic::state(){
    return {p_.S, p_.I, p_.R};
}

int epidemic::N(){
    return N_;
}

population epidemic::approx(){
    population p_integer{};
    p_integer.I = round(p_.I);
    p_integer.R = round(p_.R);
    p_integer.S = N() - p_integer.I - p_integer.R;

    assert(p_integer.S + p_integer.I + p_integer.R == N()); //controllo N
    return p_integer;
}

void evolve(epidemic x){    //Per chiamarlo: evolve(covid); 
    x.evolve();             //Non funziona! non so il perché  // forse per l'overload? boh non so
}  

double epidemic::tot(){
    return round(p_.S + p_.I + p_.R);
}
bool epidemic::IsOnGoing(){
    if(approx().I == 0) return false;
    else return true;
}