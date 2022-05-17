#include "epidemia.hpp"

int main(){
    popolazione p{90, 10, 0};
    epidemia x{0.60, 0.40, p};
    x.stampa_p();
    std::cout<<" "<<x.N()<<'\n';
    for(int i = 0; i != 10; ++i){
        x.evolve();
        x.stampa_p();
        std::cout<<" "<<x.N()<<'\n';
    }
}