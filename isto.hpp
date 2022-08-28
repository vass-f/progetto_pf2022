#ifndef ISTO_HPP
#define ISTO_HPP

#include <vector>
#include <algorithm>
#include <cmath>

/*
This class was designed as a container to manage data. It was implemented as a template
for the purpose to make it as open and general as possible. It can store data in the only private variable it has, a vector.
It has also functions for input/output purpose and statistic-oriented function, like "max" and "mean"
*/

template<typename T>
class isto{
    private:
        std::vector<T> data_{};
    public:
        isto() = default;
        isto(T input){
            data_.push_back(input);
        }

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

        auto const size(){
            return data_.size();
        }

        auto const begin(){
            return data_.begin();
        }

        auto const end(){
            return data_.end();
        }

        T max(){
            if(data_.size() == 0) return T{};
            else{
                auto it = data_.begin();
                T max = *it;
                ++it;
                for(auto end = data_.end(); it != end; ++it){
                    if(*it > max)
                        max = *it;
                }
                return max;
            }
        } 

        auto mean(){
            T mean{};
            for(auto it = data_.begin(); it != data_.end(); ++it)
                mean = mean + *(it);
            mean = mean / data_.size();
            return mean;
        }

        double SD(){    //Standard Deviation, chosen a double value because non-numeric variable have little sense
            if(data_.size() == 0 || data_.size() == 1) return -1;
            else{
                double SD;
                for(auto it = data_.begin(); it != data_.end(); ++it){
                    SD = SD + std::pow(*it - mean(), 2);
                }
                SD = SD / (data_.size() - 1);
                return std::sqrt(SD);
            }
        } 

        double SDOM(){  //Standard Deviation Of Mean
            auto N = std::sqrt(data_.size());
            return SD() / N;
        }
};


#endif