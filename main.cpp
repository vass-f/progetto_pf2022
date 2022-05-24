#include "epidemia.hpp"

int main(){
    popolazione p{90, 10, 0};
    epidemia x{0.60, 0.40, p};
    std::cout<<"S (int)  I (int)  R(int)    S   I   R    tot    tot (int)\n";
    x.stampa_p();
    std::cout<<"\t"<<x.state().S<<" "<<x.state().I<<" "<<x.state().R<<"\t";
    std::cout<<" "<<x.tot()<<" "<<(int)x.tot()<<'\n';
    for(int i = 0; i != 20; ++i){
        x.evolve();
        x.stampa_p();
        std::cout<<"\t"<<x.state().S<<" "<<x.state().I<<" "<<x.state().R<<"\t";
        std::cout<<" "<<x.tot()<<" "<<(int)x.tot()<<'\n';
    }
}