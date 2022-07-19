#ifndef ISTO_HPP
#define ISTO_HPP

#include <vector>
#include <algorithm>
//Solo hpp, senza cpp causa template
//Creare altri metodi? A cosa altro può servire questa classe?

template<typename T>
class isto{
    private:
        std::vector<T> data_{};
        //T max_{}; //Il massimo dipende dall'esistenza dell'operatore <, quindi devo trovare un modo per controllare questa cosa 
    public:
        isto() = default;
        isto(std::vector<T> input) {
            for(auto it = input.begin(), end = input.end(); it != end; ++it)
                data_.push_back(*it);
        }

        void add(T var){
            data_.push_back(var);
        }

        void add(std::vector<T> var){
            for(auto it = var.begin(), end = var.end(); it != end; ++it){
                data_.push_back(*it);
            }
        }

        auto const get(){
            return data_;
        }

        auto max(){ //Dipendenza dall'esistenza dell'operatore <
            T max{};
            for(auto it = data_.begin(), end = data_.end(); it != end; ++it){
                if(*it > max)
                    max = *it;
            }
            return max;
        } 

};


#endif