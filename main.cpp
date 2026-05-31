#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

const float PI = 3.1415f;

// mermi sinifi olusturdum
class Bullet {
private:
    sf::CircleShape shape;
    sf::Vector2f direction;
    float speed;

public:
    // mermi constructor
    Bullet(sf::Vector2f position, float angle) {
        speed = 8.0f;
        
        direction.x = std::cos(angle * PI / 180.0f);
        direction.y = std::sin(angle * PI / 180.0f);

        shape.setRadius(5.0f);
        shape.setFillColor(sf::Color::Yellow);
        shape.setOrigin({5.0f, 5.0f});
        shape.setPosition(position);
    }

    void update() {
        shape.move(direction * speed);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
    }

    sf::Vector2f getPosition() const { return shape.getPosition(); }
};

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Top-Down Shooter");
    window.setFramerateLimit(60); // 60 fps

    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.clear(sf::Color(20, 20, 20));
        window.display();
    }
    return 0;
}