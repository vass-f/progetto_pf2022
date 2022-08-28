#include "isto_sfml.hpp"
/*Importante: ogni volta in cui ho dovuto decidere una posizione, l'ho fatto in funzione di percentuali rispetto alla
larghezza e lunghezza della finestra, in questo modo tutto dovrebbe essere al proprio posto qualsiasi sia la grandezza della finestra,
tutto in proporzione costante qualsiasi modifica si faccia alla finestra*/

bool Finestra::isOpen() { return window_.isOpen(); }
void Finestra::close() { window_.close(); }

void Finestra::add(double x){
    data_.add(x);
}

void Finestra::add(std::vector<double> vector){
    data_.add(vector);
}

sf::Text Finestra::create_text(std::string string, sf::Vector2f position, int grandezza){
    sf::Text text;
    text.setFont(font);
    text.setString(string);
    text.setCharacterSize(grandezza); 
    text.setPosition(position.x, position.y);
    return text;
}

//In create_rectangle, val paremeter indicate the height, but it's actually a number between 0 and 1. If is 1, it's the maximum 
//value, and its height is "h". If it tends to 0, the value is very low, and the height have to be near the x axis.
sf::VertexArray Finestra::create_rectangle(double val, int i){ 
    sf::VertexArray r(sf::Quads, 4);
    r[0].position = sf::Vector2f(origin.x + i * delta_x, origin.y);
    r[1].position = sf::Vector2f(origin.x + i * delta_x, h + (1-val)*(origin.y - h));
    r[2].position = sf::Vector2f(origin.x + (i + 1) * delta_x, h + (1-val)*(origin.y - h));
    r[3].position = sf::Vector2f(origin.x + (i + 1) * delta_x, origin.y);

    return r;
}

void Finestra::draw_rectangle(){
    std::vector<double> data = data_.get();
    auto it = data.begin();
    auto end = data.end();
    int i = 0;
    double max = data_.max();
    //If rectangles don't enter in the x axis, reduce their width
    while((x_extreme.x - origin.x) / delta_x < data.size()) { delta_x -= 0.1; };

    for(; it != end; ++it){
        auto altezza = (*it) / max;
        auto r = create_rectangle(altezza, i);
        window_.draw(r);
        ++i;
    }
}

sf::VertexArray Finestra::create_y_axis_bar(double heigh){
    sf::VertexArray b(sf::LineStrip, 2);
    b[0].position = sf::Vector2f(origin.x + (0.06)*origin.x, heigh);
    b[1].position = sf::Vector2f(origin.x - (0.06)*origin.x, heigh);

    return b;
}

sf::VertexArray Finestra::create_x_axis_bar(double distance){
    sf::VertexArray b(sf::LineStrip, 2);
    b[0].position = sf::Vector2f(distance, origin.y + (0.01)*origin.y);
    b[1].position = sf::Vector2f(distance, origin.y - (0.01)*origin.y);
    
    return b;
}

//This function create and draw all the bars
void Finestra::draw_bars(){
    double max = data_.max();
    for(int i = 1; i != n_int_y + 1; ++i){
        double val = (double)(i) / (double)(n_int_y); //It works with fraction because the bars are equidistant
        auto posizione = h + (1 - val)*(origin.y - h); 
        auto barra = create_y_axis_bar(posizione); 
        window_.draw(barra);
        //Draw the information related to the bar
        window_.draw(create_text(std::to_string(int(max) - int((max / n_int_y) * (n_int_y - i))),
         sf::Vector2f(barra[0].position.x - (0.5)* barra[0].position.x, barra[0].position.y - (0.06)*barra[0].position.y)));
        
    }

    for(int i = 1; i != (int)data_.get().size() + 1; ++i){
        auto barra = create_x_axis_bar(origin.x + i*delta_x);
        window_.draw(barra);
        if(i % 5 == 0) //Every five bars, draw the information related to the bar
            window_.draw(create_text(std::to_string(i), sf::Vector2f(barra[0].position.x - (0.01)*barra[0].position.x,
             barra[0].position.y + (0.02)*barra[0].position.y)));
    }
}

void Finestra::draw(){
    sf::Event event;
    while (window_.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window_.close();
    }

    window_.clear();
    
    //Draw all
    window_.draw(label_origin_); //Axis origin (symbol)
    window_.draw(x_axis);
    window_.draw(y_axis);
    window_.draw(tip_x);
    window_.draw(tip_y);
    draw_rectangle();
    window_.draw(text_x_);
    window_.draw(text_y_);
    draw_bars();

    //Draw a small legend
    window_.draw(create_text((label_y_ + " " + std::to_string(int(data_.get().back()))), sf::Vector2f((0.80)*display_width_, (0.05)*display_height_)));
    window_.draw(create_text(("Day " + std::to_string(data_.get().size())), sf::Vector2f((0.80)*display_width_, (0.1)*display_height_)));
    window_.display();
}