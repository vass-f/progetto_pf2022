#ifndef ISTO_SFML_HPP
#define ISTO_SFML_HPP

#include <SFML/Graphics.hpp>

#include "isto.hpp"

/*
This class has the purpose of represent data with an histogram in a graphic
window. It can store data, in an object of the class "isto", and draw them on
window. It use a graphic library called "SFML".
*/

class Finestra {
 private:
  isto<double> data_{};
  std::string label_x_{};
  std::string label_y_{};
  sf::Font font;
  int const n_int_y = 7;  // Bar's number on y axis

  sf::RenderWindow window_;
  unsigned display_width_ = sf::VideoMode::getDesktopMode().width / 2;
  unsigned display_height_ = sf::VideoMode::getDesktopMode().height / 2;
  sf::Vector2f const origin = {
      (float)(0.13) * display_width_,
      (float)(0.90) * display_height_};  // Position of particulars points
  sf::Vector2f const x_extreme = {(float)(0.90) * display_width_,
                                  (float)(0.90) * display_height_};
  sf::Vector2f const y_extreme = {(float)(0.13) * display_width_,
                                  (float)(0.13) * display_height_};
  double const h = (1.15) * y_extreme.y;  // upper extreme of rectangles
  double delta_x = 15.f;                  // thickness of every rectangle

  // Declarations and definitions of axis, they are private and costant
  sf::VertexArray const x_axis = [&] {
    sf::VertexArray linea(sf::LineStrip, 2);
    linea[0].position = origin;
    linea[1].position = x_extreme;
    return linea;
  }();

  sf::VertexArray const y_axis = [&] {
    sf::VertexArray linea(sf::LineStrip, 2);
    linea[0].position = origin;
    linea[1].position = y_extreme;
    return linea;
  }();

  sf::VertexArray const tip_x = [&] {
    sf::VertexArray triangolo(sf::Triangles, 3);
    triangolo[0].position = x_extreme;
    triangolo[1].position =
        sf::Vector2f((float)(x_extreme.x - (0.01) * x_extreme.x),
                     (float)(origin.y + (0.01) * origin.y));
    triangolo[2].position =
        sf::Vector2f((float)(x_extreme.x - (0.01) * x_extreme.x),
                     (float)(origin.y - (0.01) * origin.y));
    return triangolo;
  }();

  sf::VertexArray const tip_y = [&] {
    sf::VertexArray triangolo(sf::Triangles, 3);
    triangolo[0].position = y_extreme;
    triangolo[1].position =
        sf::Vector2f((float)(origin.x + (0.06) * origin.x),
                     (float)(y_extreme.y + (0.13) * y_extreme.y));
    triangolo[2].position =
        sf::Vector2f((float)(origin.x - (0.06) * origin.x),
                     (float)(y_extreme.y + (0.13) * y_extreme.y));
    return triangolo;
  }();

  sf::Text text_x_ = create_text(
      label_x_, sf::Vector2f(x_extreme.x + (0.02) * x_extreme.x, origin.y));
  sf::Text text_y_ = create_text(
      label_y_, sf::Vector2f(origin.x, y_extreme.y - (0.2) * y_extreme.y));
  sf::Text label_origin_ = create_text(
      "o",
      sf::Vector2f(origin.x - (0.1) * origin.x, origin.y + (0.01) * origin.y));

 public:
  Finestra() {
    if (!font.loadFromFile("arial.ttf")) {
      throw std::runtime_error("Could not load font");
    }
    window_.create(sf::VideoMode(display_width_, display_height_), "Epidemic");
  };

  Finestra(std::string label_x, std::string label_y,
           unsigned width = sf::VideoMode::getDesktopMode().width / 2,
           unsigned height = sf::VideoMode::getDesktopMode().height / 2)
      : label_x_{label_x},
        label_y_{label_y},
        display_width_{width},
        display_height_{height} {
    if (!font.loadFromFile("arial.ttf")) {
      throw std::runtime_error("Could not load font");
    }
    window_.create(sf::VideoMode(display_width_, display_height_), "Epidemic");
  }

  bool isOpen();  // same as window.isOpen(), needed to keep while loop up
  void close();   // close the window

  void add(double x);
  void add(std::vector<double> vector);
  auto get_data() { return data_; }

  // Create a generic test in a generic position
  sf::Text create_text(std::string string, sf::Vector2f position,
                       int grandezza = 10);

  // Create a vertical rectangle, i-times distant from origin. The height is
  // calculated based on his value and the data's maximum
  sf::VertexArray create_rectangle(double val, int i);

  // Loop that draw on window all rectangle. Calls create_rectangle
  void draw_rectangle();

  // Create an horizontal bar on y axis, from an heigh
  sf::VertexArray create_y_axis_bar(double heigh);

  // Create a vertical bar on x axis, from the distance
  sf::VertexArray create_x_axis_bar(double distance);

  // Draw all the bars, calls the two function above
  void draw_bars();

  // Draw all, including axis and label.
  void draw();
};

#endif