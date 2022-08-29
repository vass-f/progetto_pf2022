// To compile
// g++ -Wall -Wextra -fsanitize=address -lsfml-system -lsfml-window
// -lsfml-graphics isto_sfml.cpp isto_sfml.test.cpp
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "isto_sfml.hpp"

TEST_CASE("Testing costructor") {
  Finestra f0;
  Finestra f1("x", "y");
  Finestra f2("aaa", "bbb", 500, 700);
  CHECK(f1.isOpen() == true);

  SUBCASE("Test get_data") {
    f1.add(20);
    f1.add(10);
    auto vet = f1.get_data().get();
    CHECK(vet[0] == 20);
    ++vet[0];
    CHECK(vet[0] == 21);

    auto it = vet.begin();
    CHECK(*it == 21);
    ++it;
    CHECK(*it == 10);
  }

  SUBCASE("Test create_rectangle") {
    Finestra f3("x", "y");
    Finestra f4("x", "y");

    f3.add({20, 10, 30, 50, 10});
    f4.add({10, 20, 30, 10, 50});
    auto max = 50;

    auto vet3 = f3.get_data().get();
    auto vet4 = f4.get_data().get();

    sf::VertexArray r1 = f3.create_rectangle(vet3[0] / max, 1);
    sf::VertexArray r2 = f4.create_rectangle(vet4[1] / max, 2);

    CHECK(r1[1].position.y == r2[1].position.y);
    CHECK(r2[0].position.x == r1[3].position.x);
  }

  SUBCASE("Test text") {
    std::string t = "test_test";
    sf::Vector2f position = {10., 10.};
    auto text = f2.create_text(t, position);
    CHECK(text.getString() == t);
    CHECK(text.getPosition() == position);
    CHECK(text.getCharacterSize() == 10);

    auto text2 = f2.create_text(t, position, 20);
    CHECK(text2.getString() == t);
    CHECK(text2.getPosition() == position);
    CHECK(text2.getCharacterSize() == 20);
  }
}