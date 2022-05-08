#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <iostream>

class ParticleSystem : public sf::Drawable, public sf::Transformable {
 public:
  ParticleSystem(unsigned int count)
      : m_particles(count),
        m_vertices(sf::Points, count),
        m_lifetime(sf::seconds(9.f)),
        m_emitter(0.f, 0.f) {}

  void setEmitter(
      sf::Vector2f position)  // funzione per cambiare la posizione dell'emitter
  {
    m_emitter = position;
  }

  void update(sf::Time elapsed) {
    for (std::size_t i = 0; i < m_particles.size(); ++i) {
      // for (size_t i = 0; i < 1; i++)
      // {
           
       
      // update the particle lifetime
      Particle& p = m_particles[i];
      p.lifetime -= elapsed;

      //if the particle is dead, respawn it
      if (p.lifetime <= sf::Time::Zero) {
        resetParticle(i);
      };

      // update the position of the corresponding vertex
      m_vertices[i].position += p.velocity * elapsed.asSeconds();

      // update the alpha (transparency) of the particle according to its
      // lifetime
      float ratio = p.lifetime.asSeconds() / m_lifetime.asSeconds();
      m_vertices[i].color.a = static_cast<sf::Uint8>(ratio * 700);

      if (m_vertices[i].position.x > 800) {     //le particelle rimbalzano sulla parete (dimensione predefinita)
        p.velocity.x = -p.velocity.x;
      }
      if (m_vertices[i].position.x < 0) {
        p.velocity.x = -p.velocity.x;
      }

      if (m_vertices[i].position.y < 0) {
        p.velocity.y = -p.velocity.y;
      }

      if (m_vertices[i].position.y > 500) {
        p.velocity.y = -p.velocity.y;
      }
    }
  }

 private:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
    // apply the transform
    states.transform *= getTransform();

    // our particles don't use a texture
    states.texture = NULL;

    // draw the vertex array
    target.draw(m_vertices, states);
  }

 private:
  struct Particle {
    sf::Vector2f velocity;
    sf::Time lifetime;
    sf::Color
        stato;  // suscettibili, malati e rimossi si distinguono per in colore
    int situazione;
  };

  void resetParticle(std::size_t index) {
    // give a random velocity and lifetime to the particle
    float angle = (std::rand() % 360) * 3.14f / 180.f;
    float speed = (std::rand() % 50) + 50.f;
    m_particles[index].velocity =
    sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);
    m_particles[index].lifetime = sf::milliseconds((std::rand() % 2000) + 1000);

    // void creaParticel(std::size_t quantita); {
    // float angle = (std::rand() % 360) * 3.14f / 180.f;
    // float speed = (std::rand() % 50) + 50.f;

    // sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);

    // }

    //reset the position of the corresponding vertex
    m_vertices[index].position = m_emitter;
  }

  std::vector<Particle> m_particles;
  sf::VertexArray m_vertices;
  sf::Time m_lifetime;
  sf::Vector2f m_emitter;
};