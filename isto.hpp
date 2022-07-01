#ifndef ISTO_HPP
#define ISTO_HPP

template<typename T>
class isto{
    private:
        std::vector<T> data_{};
        T max_{}; //Il massimo dipende dall'esistenza dell'operatore <, quindi devo trovare un modo per controllare questa cosa 
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
            
        } 

};


#endif