#include "epidemic.hpp"

void epidemic::evolve() {  // In this function we implement the equation
  population before{};  // create a temporary variable with the initial values

  before.S = p_.S;
  before.I = p_.I;
  before.R = p_.R;

  p_.S = before.S -
         (beta_ * before.S * before.I) /
             N();  // calculate the variations starting from the initial values
  p_.I = before.I + (beta_ * before.S * before.I) / N() - (gamma_ * before.I);
  p_.R = before.R + gamma_ * before.I;
  assert(round(p_.S + p_.I + p_.R) ==
         N());  // we use "round" for a exact rounding
}

void epidemic::evolve_t(int t) {
  for (int i = 0; i != t; ++i) evolve();
}

void epidemic::print_p() {
  population p = approx();
  std::cout << p.S << " " << p.I << " " << p.R << '\n';
}

population epidemic::state() { return {p_.S, p_.I, p_.R}; }

int epidemic::N() { return N_; }

population
epidemic::approx() {  // To convert from double to "int" we use "round"
                      // algorithm on I and R, then we calcolate
  population p_integer{};  // S with a difference starting from the total. In
                           // this way we'll know that N will remain constant.
  p_integer.I = round(p_.I);
  p_integer.R = round(p_.R);
  p_integer.S = N() - p_integer.I - p_integer.R;

  assert(p_integer.S + p_integer.I + p_integer.R == N());
  return p_integer;
}

double epidemic::tot() { return round(p_.S + p_.I + p_.R); }
bool epidemic::IsOnGoing() {
  if (approx().I == 0)
    return false;
  else
    return true;
}