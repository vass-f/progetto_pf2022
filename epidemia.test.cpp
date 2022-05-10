#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "epidemia.hpp"

TEST_CASE("Testing epidemia"){
    popolazione p1{90,10,0};
    epidemia x(0.75, 0.25, p1);
    int tot = x.N();
    x.evolve();
    CHECK(x.N() == tot);
    CHECK(x.approssima() == (popolazione){83, 14, 3});
    x.stampa_p();

    SUBCASE("0 infetti") {
        popolazione p2{100, 0, 0};
        epidemia x1(0.75, 0.25, p2);
        int tot = x.N();
        x1.evolve();
        CHECK(x.N() == tot);
        CHECK(x1.approssima() == (popolazione){100, 0, 0});
    }

    SUBCASE("beta su gamma minore di 1"){
        epidemia x2{0.25, 0.75, p1};
        evolve(x2);
        CHECK(x2.N() == tot);
        CHECK(x2.approssima() == (popolazione){90, 10, 0});
    }

    SUBCASE("beta e gamma invalidi"){
        CHECK_THROWS(epidemia{-1, 0, p1});
        CHECK_THROWS(epidemia{1, 1, p1});
        CHECK_THROWS(epidemia{0.5, -1.5, p1});
        CHECK_THROWS(epidemia{-1, 0.4, p1});
    }
}
