#include <SFML/Graphics.hpp>
#include "Particle.h"
#include <vector>
#include <random>

// Function to generate random particles
void generateParticles(std::vector<Particle>& particles, int count, const sf::RenderWindow& window) {
    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> xDist(0, static_cast<float>(window.getSize().x));
    std::uniform_real_distribution<float> yDist(0, static_cast<float>(window.getSize().y) / 4.0f); // Top area

    for (int i = 0; i < count; ++i) {
        // Create particle at random position near the top
        sf::Vector2f position(xDist(rng), yDist(rng));
        Particle particle(position);

        // Apply random direction force
        if (i % 3 == 0) {
            particle.applyForce(sf::Vector2f(0.0f, 300.0f)); // Downward
        } else if (i % 3 == 1) {
            particle.applyForce(sf::Vector2f(-200.0f, 300.0f)); // Bottom-left
        } else {
            particle.applyForce(sf::Vector2f(200.0f, 300.0f)); // Bottom-right
        }

        particles.push_back(particle);
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(700, 700), "Particle System Simulation");
    window.setFramerateLimit(60);

    std::vector<Particle> particles; 
    sf::Clock clock; 

    bool isPaused = false;

    generateParticles(particles, 300, window);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left && !isPaused) {
                    // Add a new particle at the mouse position
                    particles.emplace_back(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
                }
            }

            if (event.mouseButton.button == sf::Mouse::Right) {
                    for (auto& particle : particles) {
                        particle.applyForce(sf::Vector2f(500.0f, -500.0f)); // Adjust force magnitude
                    }
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    isPaused = !isPaused; // Toggle the pause state
                }
            }
        }

        if (!isPaused) {
            float deltaTime = clock.restart().asSeconds();

            // Update particles
            for (auto& particle : particles) {
                particle.update(deltaTime, window);
            }

            for (size_t i = 0; i < particles.size(); ++i) {
                for (size_t j = 0; j < particles.size(); ++j) {
                    particles[i].resolveCollision(particles[j]);
                }
            }
        } else {
            clock.restart();
        }

        // Rendering here: 
        window.clear(sf::Color(0, 0, 0));
        for (auto& particle : particles) {
            particle.draw(window);
        }
        window.display();
    }

    return 0;
}
