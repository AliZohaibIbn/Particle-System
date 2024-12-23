#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

class Particle {
public:
    sf::Vector2f position;
    sf::Vector2f previousPosition;
    sf::Vector2f acceleration;
    sf::CircleShape shape;

    static constexpr float RADIUS = 15.0f;
    static constexpr float GRAVITY = 59.8f;

    Particle(sf::Vector2f startPosition)
        : position(startPosition), previousPosition(startPosition), acceleration(0.0f, 0.0f) {
        shape.setRadius(RADIUS);
        shape.setFillColor(sf::Color::White);
        shape.setOrigin(RADIUS, RADIUS);
        shape.setPosition(position);
    }

    void update(float deltaTime, const sf::RenderWindow& window) {
        
        acceleration.y += GRAVITY;

        // Verlet integration
        sf::Vector2f tempPosition = position;
        position += (position - previousPosition) + acceleration * (deltaTime * deltaTime);
        previousPosition = tempPosition;

        // Reset acceleration for the next frame
        acceleration = sf::Vector2f(0.0f, 0.0f);

        // Collision with walls
        resolveWallCollision(window);

        
        shape.setPosition(position);
    }

    void applyForce(const sf::Vector2f& force) {
        acceleration += force;
    }

    void resolveWallCollision(const sf::RenderWindow& window) {
        // Bottom wall
        if (position.y + RADIUS >= window.getSize().y) {
            position.y = window.getSize().y - RADIUS;
            previousPosition.y = position.y + (position.y - previousPosition.y) * -0.9f;
        }
        // Top wall
        if (position.y - RADIUS <= 0) {
            position.y = RADIUS;
            previousPosition.y = position.y + (position.y - previousPosition.y) * -0.9f;
        }
        // Left wall
        if (position.x - RADIUS <= 0) {
            position.x = RADIUS;
            previousPosition.x = position.x + (position.x - previousPosition.x) * -0.9f;
        }
        // Right wall
        if (position.x + RADIUS >= window.getSize().x) {
            position.x = window.getSize().x - RADIUS;
            previousPosition.x = position.x + (position.x - previousPosition.x) * -0.9f;
        }
    }

    void resolveCollision(Particle& other) {
        sf::Vector2f delta = position - other.position;
        float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);

        if (distance < 2 * RADIUS && distance > 0) {
            float overlap = 2 * RADIUS - distance;
            sf::Vector2f collisionNormal = delta / distance;

            position += collisionNormal * (overlap / 2.0f);
            other.position -= collisionNormal * (overlap / 2.0f);
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
    }
};
