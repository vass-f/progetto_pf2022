#ifndef ISTO_SFML_HPP
#define ISTO_SFML_HPP

#include "isto.hpp"
#include <SFML/Graphics.hpp>
#include <iostream> //Solo per debugging

class Finestra{
    private:
        isto<double> data_{}; //O deve essere int?
        std::string label_x_{};
        std::string label_y_{};
        sf::Font font;
        int const n_int_y = 7; //Numero di tacche sull'asse y

        sf::RenderWindow window_;
        unsigned display_width_ = sf::VideoMode::getDesktopMode().width / 2;
        unsigned display_height_ = sf::VideoMode::getDesktopMode().height / 2;
        sf::Vector2f const origin = {(float)(0.13)*display_width_, (float)(0.90)*display_height_};  //Posizione di punti particolari
        sf::Vector2f const x_extreme = {(float)(0.90)*display_width_, (float)(0.90)*display_height_};
        sf::Vector2f const y_extreme = {(float)(0.13)*display_width_,(float)(0.13)*display_height_};
        double const h = (1.15)*y_extreme.y;
        double delta_x = 15.f; //Spessore di ogni rettangolo

        //Definizione e dichiarazione degli assi, scritto qua perché sono const
        sf::VertexArray const asse_x = [&]{
            sf::VertexArray linea(sf::LineStrip, 2);
            linea[0].position = origin;
            linea[1].position = x_extreme;
            return linea;
        }();

        sf::VertexArray const asse_y = [&]{
            sf::VertexArray linea(sf::LineStrip, 2);
            linea[0].position = origin;
            linea[1].position = y_extreme;
            return linea;
        }();

        sf::VertexArray const punta_x = [&]{
            sf::VertexArray triangolo(sf::Triangles, 3);
            triangolo[0].position = x_extreme;
            triangolo[1].position = sf::Vector2f((float)(x_extreme.x - (0.01)*x_extreme.x), (float)(origin.y + (0.01) * origin.y));
            triangolo[2].position = sf::Vector2f((float)(x_extreme.x - (0.01)*x_extreme.x), (float)(origin.y - (0.01) * origin.y));
            return triangolo;
        }();

        sf::VertexArray const punta_y = [&]{
            sf::VertexArray triangolo(sf::Triangles, 3);
            triangolo[0].position = y_extreme;
            triangolo[1].position = sf::Vector2f((float)(origin.x + (0.06)*origin.x), (float)(y_extreme.y + (0.13)*y_extreme.y));
            triangolo[2].position = sf::Vector2f((float)(origin.x - (0.06)*origin.x), (float)(y_extreme.y + (0.13)*y_extreme.y));
            return triangolo;
        }();

        sf::Text label_x = create_text(label_x_, sf::Vector2f(x_extreme.x + (0.02)*x_extreme.x, origin.y));
        sf::Text label_y = create_text(label_y_, sf::Vector2f(origin.x, y_extreme.y - (0.2)*y_extreme.y));
        sf::Text label_origin = create_text("o", sf::Vector2f(origin.x - (0.1)*origin.x, origin.y + (0.01)*origin.y));
    public:
        Finestra() {
            if(!font.loadFromFile("arial.ttf")){
                throw std::runtime_error("Could not load font");
            }
            window_.create(sf::VideoMode(display_width_, display_height_), "Epidemic");
        };
        
        Finestra(std::string label_x, std::string label_y, unsigned width = sf::VideoMode::getDesktopMode().width / 2, unsigned height = sf::VideoMode::getDesktopMode().height / 2) 
            : label_x_{label_x}, label_y_{label_y}, display_width_{width}, display_height_{height} {
            if(!font.loadFromFile("arial.ttf")){
                throw std::runtime_error("Could not load font");
            }
            window_.create(sf::VideoMode(display_width_, display_height_), "Epidemic");
        }

        bool isOpen();  //Uguale a window.isOpen() per far andare il ciclo while
        void close();

        void add(double x); //aggiungo elementi al mio isto

        void add(std::vector<double> vector);

        sf::Text create_text(std::string string, sf::Vector2f position); //Crea un testo generico in una posizione generica
                                                                       //Capire perché se la dimensione del carattere gliela passo come
                                                                       //parametro fissato da problemi (int grandezza = 10)

        sf::VertexArray create_rettangolo(double val, int i); //Crea un rettangolo alla posizione di i, l'altezza è calcolata in base al suo valore
                                                            //e al massimo dei dati

        void draw_rectangle(); //Ciclo for che disegna tutti i rettangoli a partire dai dati in data_, chiama crea_rettangolo

        sf::VertexArray create_y_axis_bar(double heigh); //Crea una barra orizzontale sull'asse y a partire dall'altezza

        sf::VertexArray create_x_axis_bar(double distance); //Crea una barra verticale sull'asse x a partire dalla distanza

        void draw_barre(); //Disegna le barre chiamando le due funzioni sopra, l'altezza dipende dal numero di intervalli, la distanza
                           //dipende da delta_x e i

        void draw(); //Disegna tutto, chiama le funzioni per disegnare le barre e i rettangoli
};

#endif
//Fare la finestra delle informazioni in alto a destra