#include "particel.hpp"

int main()
{
    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 500), "Particles");

    // create the particle system
    ParticleSystem particles(7000);

    // create a clock to track the elapsed time
    sf::Clock clock;

    // run the main loop
    while (window.isOpen())
    {
        // handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
              {  window.close();}

            else if (event.type == sf::Event::MouseButtonPressed)
            {
                sf::Vector2i posizione = sf::Mouse::getPosition(window);
                particles.setEmitter(window.mapPixelToCoords(posizione));
            }
            
        }

        // make the particle system emitter follow the mouse
        //sf::Vector2i mouse = sf::Mouse::getPosition(window);
        //particles.setEmitter(window.mapPixelToCoords(mouse));

        // update it
        sf::Time elapsed = clock.restart();
        particles.update(elapsed);

        // draw it
        window.clear();
        window.draw(particles);
        window.display();
    }

    return 0;
}