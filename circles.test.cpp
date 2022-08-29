// To compile
// g++ -Wall -Wextra -fsanitize=address -lsfml-system -lsfml-window
// -lsfml-graphics circles.cpp circles.test.cpp
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "circles.hpp"
#include "doctest.h"

TEST_CASE("Testing circles") {  //
  Person a;
  Person b;
  a.position(100, 100);
  b.position(100, 100);
  CHECK(a.distance(b) == 0);

  SUBCASE("Testing distance") {
    Person a;
    Person b;
    a.position(100, 200);
    b.position(400, 600);
    CHECK(b.distance(a) == 500);
  }
  SUBCASE("Testing distance") {
    Person a;
    Person b;
    a.position(300, 400);
    b.position(0, 100);
    CHECK(b.distance(a) == a.distance(b));
  }
  SUBCASE("Testing evolve_p") {
    Person a;
    a.position(100, 200);
    a.velocity(0, 300);
    a.evolve_p();
    CHECK(a.position().x == 100);
    CHECK(a.position().y == 500);
  }
  SUBCASE("Testing evolve_v (velocity maximum increase)") {
    Person a;
    a.velocity(10, 20);
    a.evolve_v();
    CHECK(a.velocity().x <= 10.5);
    CHECK(a.velocity().y <= 20.5);
  }

  SUBCASE("Testing evolve_v (velocity maximum decrease)") {
    Person a;
    a.velocity(30, 40);
    a.evolve_v();
    CHECK(a.velocity().x >= 29.4);
    CHECK(a.velocity().y >= 39.4);
  }
}