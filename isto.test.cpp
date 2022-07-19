#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "isto.hpp"

TEST_CASE("Test isto"){
    std::vector<double> data{3.4, 6.6, 0.4, -6.4};
    isto<double> prova(data);
    SUBCASE("costruttore e add monodimensionale"){
        isto<double> a;
        a.add(1.1);
        a.add(2.2);
        std::vector<double> data = a.get();
        auto it = data.begin();
        CHECK(*it == 1.1);
        ++it;
        CHECK(*it == 2.2);
    }

    SUBCASE("add vector"){
        isto<int> a;
        std::vector<int> vet{1,2,3};
        a.add(vet);
        std::vector<int> data = a.get();
        auto it = data.begin();
        auto itt = vet.begin();
        CHECK(*it == *itt );
        ++it;
        ++itt;
        CHECK(*it == *itt );
        ++it;
        ++itt;
        CHECK(*it == *itt );
        ++it;
        ++itt;
    }
}