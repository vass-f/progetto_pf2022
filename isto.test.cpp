// To compile
// g++ -Wall -Wextra -fsanitize=address isto.test.cpp
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "isto.hpp"

TEST_CASE("Test isto") {
  std::vector<double> data{3.4, 6.6, 0.4, -6.4};
  isto<double> prova(data);
  SUBCASE("constructor and monodimensional add") {
    isto<double> a;
    a.add(1.1);
    a.add(2.2);
    std::vector<double> data = a.get();
    auto it = data.begin();
    CHECK(*it == 1.1);
    ++it;
    CHECK(*it == 2.2);
  }

  SUBCASE("add vector") {
    isto<int> a;
    std::vector<int> vet{1, 2, 3};
    a.add(vet);
    std::vector<int> data = a.get();
    auto it = data.begin();
    auto itt = vet.begin();
    CHECK(*it == *itt);
    ++it;
    ++itt;
    CHECK(*it == *itt);
    ++it;
    ++itt;
    CHECK(*it == *itt);
    ++it;
    ++itt;
  }

  SUBCASE("test iterator") {
    std::vector<double> vet{1.1, 2.2, 3.3};
    isto<double> b;
    b.add(1.1);
    b.add(2.2);
    b.add(3.3);

    auto it = vet.begin();
    auto itt = b.begin();
    for (; itt != b.end(); ++itt) {
      CHECK(*it == *itt);
      ++it;
    }
  }

  SUBCASE("test mean") {
    std::vector<int> values{5, 4, 6, 7, 3};
    isto<int> h(values);
    CHECK(h.max() == 7);
    CHECK(h.mean() == 5);
    CHECK(h.size() == 5);
  }

  SUBCASE("test max") {
    isto<double> c{};
    CHECK(c.size() == 0);
    CHECK(c.max() == 0);

    c.add(3.);
    CHECK(c.size() == 1);
    CHECK(c.max() == 3.);

    CHECK(prova.max() == 6.6);

    isto<double> d{-1.};
    CHECK(d.max() == -1);
  }

  SUBCASE("test SD") {
    std::vector<double> values{10., 4.5, 9.3, 6.9};
    isto<double> f(values);
    CHECK(f.max() == 10.);
    CHECK(f.size() == 4);
    CHECK(f.mean() == doctest::Approx(7.675).epsilon(0.1));
    CHECK(f.SD() == doctest::Approx(2.5).epsilon(0.1));
    CHECK(f.SDOM() == doctest::Approx(1.25).epsilon(0.1));
  }
}