#include "isto_sfml.hpp"

bool Finestra::isOpen() { return window_.isOpen(); }

void Finestra::add(double x){
    data_.add(x);
}

void Finestra::add(std::vector<double> vettore){
    data_.add(vettore);
}

sf::Text Finestra::crea_text(std::string string, sf::Vector2f position){
    sf::Text text;
    text.setFont(font);
    text.setString(string);
    text.setCharacterSize(10); //Da capire se è possibile modificarlo in funzione della dimensione della finestra o non importa
    text.setPosition(position.x, position.y);
    return text;
}

sf::VertexArray Finestra::crea_rettangolo(double val, int i){ //L'altezza di un rettangolo non deve essere calcolata con val * h 
                                                    //Perché i pixel sulle y vanno dall'alto verso l'alto, più è piccolo più è verso l'alto
    sf::VertexArray r(sf::Quads, 4);
    r[0].position = sf::Vector2f(origine.x + i * delta_x, origine.y);
    r[1].position = sf::Vector2f(origine.x + i * delta_x, h + (1-val)*(origine.y - h));
    r[2].position = sf::Vector2f(origine.x + (i + 1) * delta_x, h + (1-val)*(origine.y - h));
    r[3].position = sf::Vector2f(origine.x + (i + 1) * delta_x, origine.y);

    return r;
}

void Finestra::draw_rectangle(){
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

sf::VertexArray Finestra::crea_barra_asse_y(double altezza){
    sf::VertexArray b(sf::LineStrip, 2);
    b[0].position = sf::Vector2f(origine.x + (0.06)*origine.x, altezza);
    b[1].position = sf::Vector2f(origine.x - (0.06)*origine.x, altezza);

    return b;
}

sf::VertexArray Finestra::crea_barra_asse_x(double distanza){
    sf::VertexArray b(sf::LineStrip, 2);
    b[0].position = sf::Vector2f(distanza, origine.y + (0.01)*origine.y);
    b[1].position = sf::Vector2f(distanza, origine.y - (0.01)*origine.y);
    
    return b;
}

void Finestra::draw_barre(){
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

void Finestra::draw(){
    sf::Event event;
    while (window_.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window_.close();
    }

    window_.clear();

    window_.draw(crea_text("o", sf::Vector2f(origine.x - (0.1)*origine.x, origine.y + (0.01)*origine.y)));
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