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
        sf::Vector2f const origine = {(float)(0.13)*display_width_, (float)(0.90)*display_height_};  //Posizione di punti particolari
        sf::Vector2f const estremo_x = {(float)(0.90)*display_width_, (float)(0.90)*display_height_};
        sf::Vector2f const estremo_y = {(float)(0.13)*display_width_,(float)(0.13)*display_height_};
        double const h = (1.15)*estremo_y.y;
        double delta_x = 15.f; //Spessore di ogni rettangolo

        //Definizione e dichiarazione degli assi, scritto qua perché sono const
        sf::VertexArray const asse_x = [&]{
            sf::VertexArray linea(sf::LineStrip, 2);
            linea[0].position = origine;
            linea[1].position = estremo_x;
            return linea;
        }();

        sf::VertexArray const asse_y = [&]{
            sf::VertexArray linea(sf::LineStrip, 2);
            linea[0].position = origine;
            linea[1].position = estremo_y;
            return linea;
        }();

        sf::VertexArray const punta_x = [&]{
            sf::VertexArray triangolo(sf::Triangles, 3);
            triangolo[0].position = estremo_x;
            triangolo[1].position = sf::Vector2f((float)(estremo_x.x - (0.01)*estremo_x.x), (float)(origine.y + (0.01) * origine.y));
            triangolo[2].position = sf::Vector2f((float)(estremo_x.x - (0.01)*estremo_x.x), (float)(origine.y - (0.01) * origine.y));
            return triangolo;
        }();

        sf::VertexArray const punta_y = [&]{
            sf::VertexArray triangolo(sf::Triangles, 3);
            triangolo[0].position = estremo_y;
            triangolo[1].position = sf::Vector2f((float)(origine.x + (0.06)*origine.x), (float)(estremo_y.y + (0.13)*estremo_y.y));
            triangolo[2].position = sf::Vector2f((float)(origine.x - (0.06)*origine.x), (float)(estremo_y.y + (0.13)*estremo_y.y));
            return triangolo;
        }();

        sf::Text label_x = crea_text(label_x_, sf::Vector2f(estremo_x.x + (0.02)*estremo_x.x, origine.y));
        sf::Text label_y = crea_text(label_y_, sf::Vector2f(origine.x, estremo_y.y - (0.2)*estremo_y.y));
        sf::Text label_origin = crea_text("o", sf::Vector2f(origine.x - (0.1)*origine.x, origine.y + (0.01)*origine.y));
    public:
        Finestra() {
            if(!font.loadFromFile("arial.ttf")){
                throw std::runtime_error("Could not load font");
            }
            window_.create(sf::VideoMode(display_width_, display_height_), "Epidemia");
        };
        
        Finestra(std::string label_x, std::string label_y, unsigned width = sf::VideoMode::getDesktopMode().width / 2, unsigned height = sf::VideoMode::getDesktopMode().height / 2) 
            : label_x_{label_x}, label_y_{label_y}, display_width_{width}, display_height_{height} {
            if(!font.loadFromFile("arial.ttf")){
                throw std::runtime_error("Could not load font");
            }
            window_.create(sf::VideoMode(display_width_, display_height_), "Epidemia");
        }



        bool isOpen();  //Uguale a window.isOpen() per far andare il ciclo while
        void close();

        void add(double x); //aggiungo elementi al mio isto

        void add(std::vector<double> vettore);

        sf::Text crea_text(std::string string, sf::Vector2f position); //Crea un testo generico in una posizione generica
                                                                       //Capire perché se la dimensione del carattere gliela passo come
                                                                       //parametro fissato da problemi (int grandezza = 10)

        sf::VertexArray crea_rettangolo(double val, int i); //Crea un rettangolo alla posizione di i, l'altezza è calcolata in base al suo valore
                                                            //e al massimo dei dati

        void draw_rectangle(); //Ciclo for che disegna tutti i rettangoli a partire dai dati in data_, chiama crea_rettangolo

        sf::VertexArray crea_barra_asse_y(double altezza); //Crea una barra orizzontale sull'asse y a partire dall'altezza

        sf::VertexArray crea_barra_asse_x(double distanza); //Crea una barra verticale sull'asse x a partire dalla distanza

        void draw_barre(); //Disegna le barre chiamando le due funzioni sopra, l'altezza dipende dal numero di intervalli, la distanza
                           //dipende da delta_x e i

        void draw(); //Disegna tutto, chiama le funzioni per disegnare le barre e i rettangoli
};

/*
template<typename T>
class legend{
    private:
        std::vector<sf::Text> text(3);
        sf::Font comment_font;

    public:
    legend() {
        if (!comment_font.loadFromFile("comment_font.ttf")){
        throw std::runtime_error{"Could not load comment font"};
        }
    }
    void setLegend(popolazione p){
        for(int i{0}; i != 3; ++i){   
        text[i].setFont(comment_font);
        }

        text[0].setFillColor(sf::Color::Blue);
        text[1].setFillColor(sf::Color::Red);
        text[2].setFillColor(sf::Color::Green);
        for(int i{0}; i != n_text; ++i){
        text[i].setPosition(0.8*sf::VideoMode::getDesktopMode().width / 2,10+30*i);
        }
        auto susceptible = "Susceptible:  " + std::to_string(p.S);  // some information about the epidemic
        auto infected = "Infected:  " + std::to_string(p.I);
        auto recovered = "Recovered:  " + std::to_string(p.R);
        text[0].setString(susceptible);
        text[1].setString(infected);
        text[2].setString(recovered);
        for(int i{0}; i != 4; ++i){  // draw the text
            window.draw(text[i]);
        }
        window.display();
    }

};
*/

#endif
//Fare la finestra delle informazioni in alto a destra