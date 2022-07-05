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
        int const n_int_y = 7;

        sf::RenderWindow window_;
        unsigned display_width_ = sf::VideoMode::getDesktopMode().width / 2;
        unsigned display_height_ = sf::VideoMode::getDesktopMode().height / 2;
        sf::Vector2f const origine = {(float)(0.13)*display_width_, (float)(0.90)*display_height_};
        sf::Vector2f const estremo_x = {(float)(0.90)*display_width_, (float)(0.90)*display_height_};
        sf::Vector2f const estremo_y = {(float)(0.13)*display_width_,(float)(0.13)*display_height_};
        double const h = estremo_y.y + (0.15)*estremo_y.y;
        double delta_x = 15.f;

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
    public:
        Finestra() {
            if(!font.loadFromFile("arial.ttf")){
                throw std::runtime_error("Il font non è stato caricato");
            }
            window_.create(sf::VideoMode(display_width_, display_height_), "Epidemia");
        };
        
        Finestra(std::string a, std::string b) : label_x_{a}, label_y_{b} {
            if(!font.loadFromFile("arial.ttf")){
                throw std::runtime_error("Il font non è stato caricato");
            }
            window_.create(sf::VideoMode(display_width_, display_height_), "Epidemia");
        }

        bool isOpen(){ return window_.isOpen(); }
        void add(double x){
            data_.add(x);
        }

        void add(std::vector<double> vettore){
            data_.add(vettore);
        }

        sf::Text crea_text(std::string string, sf::Vector2f position, int grandezza = 10){
            sf::Text text;
            text.setFont(font);
            text.setString(string);
            text.setCharacterSize(grandezza); //Da capire se è possibile modificarlo in funzione della dimensione della finestra o non importa
            text.setPosition(position.x, position.y);
            return text;
        }

        sf::VertexArray crea_rettangolo(double val, int i){ //L'altezza di un rettangolo non deve essere calcolata con val * h 
                                                            //Perché i pixel sulle y vanno dall'alto verso l'alto, più è piccolo più è verso l'alto
            sf::VertexArray r(sf::Quads, 4);
            r[0].position = sf::Vector2f(origine.x + i * delta_x, origine.y);
            r[1].position = sf::Vector2f(origine.x + i * delta_x, h + (1-val)*(origine.y - h));
            r[2].position = sf::Vector2f(origine.x + (i + 1) * delta_x, h + (1-val)*(origine.y - h));
            r[3].position = sf::Vector2f(origine.x + (i + 1) * delta_x, origine.y);

            return r;
        }

        void draw_rectangle(){
            std::vector<double> data = data_.get();
            auto it = data.begin();
            auto end = data.end();
            int i = 0;
            double max = data_.max();
            while((estremo_x.x - origine.x) / delta_x < data.size()) { delta_x -= 0.1; };
            for(; it != end; ++it){
                auto altezza = (*it) / max;
                auto r = crea_rettangolo(altezza, i);
                window_.draw(r);
                ++i;
            }
        }

        sf::VertexArray crea_barra_asse_y(double altezza){
            sf::VertexArray b(sf::LineStrip, 2);
            b[0].position = sf::Vector2f(origine.x + (0.06)*origine.x, altezza);
            b[1].position = sf::Vector2f(origine.x - (0.06)*origine.x, altezza);

            return b;
        }

        sf::VertexArray crea_barra_asse_x(double distanza){
            sf::VertexArray b(sf::LineStrip, 2);
            b[0].position = sf::Vector2f(distanza, origine.y + (0.01)*origine.y);
            b[1].position = sf::Vector2f(distanza, origine.y - (0.01)*origine.y);
            
            return b;
        }

        void draw_barre(){
            double max = data_.max();
            for(int i = 1; i != n_int_y + 1; ++i){
                double val = (double)(i) / (double)(n_int_y);
                auto posizione = h + (1 - val)*(origine.y - h);
                auto barra = crea_barra_asse_y(posizione);
                window_.draw(barra);
                window_.draw(crea_text(std::to_string(max * val), sf::Vector2f(barra[0].position.x - (0.5)* barra[0].position.x, barra[0].position.y - (0.06)*barra[0].position.y)));
                
            }

            for(int i = 1; i != (int)data_.get().size() + 1; ++i){
                auto barra = crea_barra_asse_x(origine.x + i*delta_x);
                window_.draw(barra);
                if(i % 5 == 0) window_.draw(crea_text(std::to_string(i), sf::Vector2f(barra[0].position.x - (0.01)*barra[0].position.x, barra[0].position.y + (0.02)*barra[0].position.y)));
            }
            std::cout<<'\n';
        }

        void draw(){
            sf::Event event;
            while (window_.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window_.close();
            }

            window_.clear();

            window_.draw(crea_text("o", sf::Vector2f(origine.x - (0.1)*origine.x, origine.y + (0.01)*origine.y), 15));
            window_.draw(asse_x);
            window_.draw(asse_y);
            window_.draw(punta_x);
            window_.draw(punta_y);
            draw_rectangle();
            window_.draw(label_x);
            window_.draw(label_y);
            draw_barre();

            window_.display();
        }
};


#endif

/*Deve avere come variabili private: un oggetto della classe isto dove verranno salvati i dati dell'epidemia
                                     Non so se la parte grafica deve rientrare come variabili private della classe
Deve riuscire a creare una finestra, a creare degli assi, le cui unità di misura riescano a scalare all'evenienza

Una funzione membro per stampare che richiami l'aggiornamento (add) dei dati nella variabile isto, che a sua volta richiami
evolve di epidemia, me lo immagino come isto.add(evolve(...))
Capire bene come disegnare rettangoli
Capire se posso scrivere TUTTA la parte grafica nella classe, in modo che nel main non ci andrà niente se non la definizione di un oggetto
della classe definita qui
*/

//Forse creare una struct/classe per le barre sugli assi per gestirle meglio
