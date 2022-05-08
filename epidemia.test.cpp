#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "epidemia.hpp"

TEST_CASE("Testing epidemia"){
    popolazione p1{90,10,0};
    epidemia x(0.75, 0.25, p1);
    int tot = x.N();
    x.evolve();
    CHECK(x.approssima().S == 83);
    CHECK(x.approssima().I == 14); //I e R falliti, ricontrollare
    CHECK(x.approssima().R == 3);
}