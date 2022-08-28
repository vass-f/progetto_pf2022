//To compile
//g++ -Wall -Wextra -fsanitize=address epidemic.cpp
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "epidemic.hpp"

TEST_CASE("Testing epidemic"){
    population p1{90,10,0};
    epidemic x(0.75, 0.25, p1);
    int tot = x.N();
    x.evolve();
    CHECK(x.N() == tot);
    CHECK(x.approx() == (population){83, 14, 3});

    SUBCASE("0 infect") {
        population p2{100, 0, 0};
        epidemic x1(0.75, 0.25, p2);
        int tot = x.N();
        x1.evolve();
        CHECK(x.N() == tot);
        CHECK(x1.approx() == (population){100, 0, 0});
    }

    SUBCASE("beta on gamma less than 1"){
        epidemic x2{0.25, 0.75, p1};
        x2.evolve_t(3);
        CHECK(x.N() == tot);
        CHECK(x2.approx().I == 0);
    }

    SUBCASE("invalid beta and gamma"){
        CHECK_THROWS(epidemic{-1, 0, p1});
        CHECK_THROWS(epidemic{1, 1, p1});
        CHECK_THROWS(epidemic{0.5, -1.5, p1});
        CHECK_THROWS(epidemic{-1, 0.4, p1});
    }

    SUBCASE("test conservation of N"){
        population p{90, 10, 0};
        epidemic x3{0.60, 0.40, p};    
        CHECK(x3.N() == 100);
        
        x3.evolve();
        CHECK(x3.N() == x3.tot());
        x3.evolve();
        CHECK(x3.N() == (int)x3.tot());

        x3.evolve();
        CHECK(x3.N() == x3.tot());
        x3.evolve();
        CHECK(x3.N() == x3.tot());
        x3.evolve();
        CHECK(x3.N() == x3.tot());
        x3.evolve();
        CHECK(x3.N() == x3.tot());
        x3.evolve();
        CHECK(x3.N() == x3.tot());

        x3.evolve_t(10);
        CHECK(x3.N() == x3.tot());
    }

    SUBCASE("test long period of time"){
        population p1{99997, 47, 0};
        epidemic x4{0.61, 0.4, p1};
        int tot = x4.N();
        CHECK(tot == 100044);
        CHECK(x4.N() == 100044);

        x4.evolve();
        CHECK(x4.N() == x4.tot());
        x4.evolve_t(1000);
        CHECK(x4.N() == x4.tot());
        x4.evolve_t(10000);
        CHECK(x4.N() == x4.tot()); 
        
        x4.evolve_t(10000);
        CHECK(x4.N() == x4.tot());
    }

    SUBCASE("test IsOnGoing"){
        population p2{9999, 1, 0};
        epidemic x5{0.9, 0.1, p2};

        CHECK(x5.IsOnGoing() == true);
        bool test;
        double a;
        while(x5.IsOnGoing()){
            x5.evolve();
            if(test){
                CHECK(x5.approx().S == a);
            }
            if(x5.state().I > 0 && x5.state().I < 0.4999){
                test = true;
                a = x5.approx().S;
                CHECK(x5.IsOnGoing() == false);
            }
        }
    }

    SUBCASE("wrong population"){
        population p3{-1, -3, 1};
        CHECK_THROWS(epidemic{0.1,0.1,p3});

        population p4{0.4, 0.3, 0.4};           
        CHECK_THROWS(epidemic{0.1, 0.1, p4});

        population p5{0.6, 0.6, 0.3};
        epidemic x6{0.6, 0.3, p5};
        CHECK(x6.state().S == 1);
        CHECK(x6.state().I == 1);
        CHECK(x6.state().R == 0);

        population p6{2, 0, -3};
        CHECK_THROWS(epidemic{0.1,0.1,p6});

        population p7{0.6, 1, -1.5};
        CHECK_THROWS(epidemic{0.1, 0.1, p7});

        population p8{1, -0.4, 0};
        epidemic x7{0.6, 0.3, p8};
        CHECK(x7.state().S == 1);
        CHECK(x7.state().I == 0);
        CHECK(x7.state().R == 0);
    }
}
