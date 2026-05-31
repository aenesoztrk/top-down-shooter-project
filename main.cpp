#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

using namespace sf;

const float PI = 3.1415f;

// mermi classı
class Bullet {
private:
  CircleShape shape;
  Vector2f direction;
  float speed;

public:
  // mermi constructor
  Bullet(Vector2f position, float angle) {
    speed = 8.0f;

    direction.x = std::cos(angle * PI / 180.0f);
    direction.y = std::sin(angle * PI / 180.0f);

    shape.setRadius(5.0f);
    shape.setFillColor(Color::Yellow);
    shape.setOrigin({5.0f, 5.0f});
    shape.setPosition(position);
  }

  void update() { shape.move(direction * speed); }

  void draw(RenderWindow &window) { window.draw(shape); }

  Vector2f getPosition() const { return shape.getPosition(); }
};

// oyuncu classı
class Player {
private:
  CircleShape shape;
  float speed;
  float currentAngle;

public:
  Player() {
    speed = 4.0f;
    currentAngle = 0.0f;

    shape.setRadius(20.0f);
    shape.setFillColor(Color::Green);
    shape.setOrigin({20.0f, 20.0f});
    shape.setPosition({400.0f, 300.0f});
  }

  void update(RenderWindow &window) {
    Vector2f movement(0.0f, 0.0f);
    if (Keyboard::isKeyPressed(Keyboard::Key::W))
      movement.y -= 1.0f;
    if (Keyboard::isKeyPressed(Keyboard::Key::S))
      movement.y += 1.0f;
    if (Keyboard::isKeyPressed(Keyboard::Key::A))
      movement.x -= 1.0f;
    if (Keyboard::isKeyPressed(Keyboard::Key::D))
      movement.x += 1.0f;

    if (movement.x != 0.0f && movement.y != 0.0f) {
      movement.x /= std::sqrt(2.0f);
      movement.y /= std::sqrt(2.0f);
    }

    shape.move(movement * speed);

    Vector2i mousePos = Mouse::getPosition(window);
    Vector2f playerPos = shape.getPosition();

    float dx = mousePos.x - playerPos.x;
    float dy = mousePos.y - playerPos.y;
    currentAngle = std::atan2(dy, dx) * 180.0f / PI;

    shape.setRotation(degrees(currentAngle));
  }

  void draw(RenderWindow &window) { window.draw(shape); }

  Vector2f getPosition() const { return shape.getPosition(); }
  float getAngle() const { return currentAngle; }
};

// düşman classı
class Enemy {
private:
  sf::CircleShape shape;
  float speed;

public:
  Enemy(sf::Vector2f position) {
    speed = 2.0f;

    shape.setRadius(15.0f);
    shape.setFillColor(sf::Color::Red);
    shape.setOrigin({15.0f, 15.0f});
    shape.setPosition(position);
  }

  void update(sf::Vector2f playerPosition) {
    sf::Vector2f direction = playerPosition - shape.getPosition();

    float distance =
        std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance > 0.0f) {
      direction.x /= distance;
      direction.y /= distance;
      shape.move(direction * speed);
    }
  }

  void draw(sf::RenderWindow &window) { window.draw(shape); }

  sf::Vector2f getPosition() const { return shape.getPosition(); }
};
int main() {
  sf::RenderWindow window(sf::VideoMode({800, 600}), "Top-Down Shooter");
  window.setFramerateLimit(60);

  Player player;
  std::vector<Bullet> bullets;

  Enemy testEnemy({700.0f, 300.0f});

  while (window.isOpen()) {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>())
        window.close();

      if (event->is<sf::Event::MouseButtonPressed>()) {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
          bullets.push_back(Bullet(player.getPosition(), player.getAngle()));
        }
      }
    }

    player.update(window);

    testEnemy.update(player.getPosition());

    for (auto &bullet : bullets)
      bullet.update();

    window.clear(sf::Color(20, 20, 20));

    player.draw(window);
    testEnemy.draw(window);
    for (auto &bullet : bullets)
      bullet.draw(window);

    window.display();
  }
  return 0;
}