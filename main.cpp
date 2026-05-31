#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace sf;

const float PI = 3.1415f;

class Bullet {
private:
  sf::CircleShape shape;
  sf::Vector2f direction;
  float speed;

public:
  Bullet(sf::Vector2f position, float angle) {
    speed = 10.0f;
    direction.x = std::cos(angle * PI / 180.0f);
    direction.y = std::sin(angle * PI / 180.0f);
    shape.setRadius(4.0f);
    shape.setFillColor(sf::Color::Yellow);
    shape.setOrigin({4.0f, 4.0f});
    shape.setPosition(position);
  }
  void update() { shape.move(direction * speed); }
  void draw(sf::RenderWindow &window) { window.draw(shape); }
  sf::Vector2f getPosition() const { return shape.getPosition(); }
};

class PowerUp {
private:
  sf::CircleShape shape;
  int type;

public:
  PowerUp(sf::Vector2f position, int pType) : type(pType) {
    shape.setRadius(10.0f);
    shape.setOrigin({10.0f, 10.0f});
    shape.setPosition(position);
    if (type == 0)
      shape.setFillColor(sf::Color::Cyan);
    else
      shape.setFillColor(sf::Color::Magenta);
  }
  void draw(sf::RenderWindow &window) { window.draw(shape); }
  sf::Vector2f getPosition() const { return shape.getPosition(); }
  int getType() const { return type; }
};

class Player {
private:
  sf::CircleShape shape;
  float speed;
  float currentAngle;

public:
  int health;
  int maxHealth;
  float shootCooldown;

  Player() {
    speed = 4.0f;
    currentAngle = 0.0f;
    maxHealth = 100;
    health = maxHealth;
    shootCooldown = 0.25f;

    shape.setRadius(20.0f);
    shape.setFillColor(sf::Color::Green);
    shape.setOrigin({20.0f, 20.0f});
    shape.setPosition({400.0f, 300.0f});
  }

  void update(sf::RenderWindow &window) {
    sf::Vector2f movement(0.0f, 0.0f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
      movement.y -= 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
      movement.y += 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
      movement.x -= 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
      movement.x += 1.0f;

    if (movement.x != 0.0f && movement.y != 0.0f) {
      movement.x /= std::sqrt(2.0f);
      movement.y /= std::sqrt(2.0f);
    }
    shape.move(movement * speed);

    sf::Vector2f pos = shape.getPosition();
    if (pos.x < 20)
      shape.setPosition({20.0f, pos.y});
    if (pos.x > 780)
      shape.setPosition({780.0f, pos.y});
    if (pos.y < 20)
      shape.setPosition({pos.x, 20.0f});
    if (pos.y > 580)
      shape.setPosition({pos.x, 580.0f});

    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    float dx = mousePos.x - shape.getPosition().x;
    float dy = mousePos.y - shape.getPosition().y;
    currentAngle = std::atan2(dy, dx) * 180.0f / PI;
    shape.setRotation(sf::degrees(currentAngle));
  }

  void draw(sf::RenderWindow &window) { window.draw(shape); }
  sf::Vector2f getPosition() const { return shape.getPosition(); }
  float getAngle() const { return currentAngle; }
};

class Enemy {
private:
  sf::CircleShape shape;
  float speed;

public:
  Enemy(sf::Vector2f position, float speedMultiplier) {
    speed = 1.2f * speedMultiplier;
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
  sf::RenderWindow window(sf::VideoMode({800, 600}),
                          "Top-Down Shooter - Ahmet Enes Ozturk");
  window.setFramerateLimit(60);
  srand(static_cast<unsigned int>(time(0)));

  Player player;
  std::vector<Bullet> bullets;
  std::vector<Enemy> enemies;
  std::vector<PowerUp> powerups;

  int score = 0;
  int currentWave = 0;
  float waveSpeedMultiplier = 1.0f;
  sf::Clock shootClock;

  sf::RectangleShape healthBarBack({150.0f, 20.0f});
  healthBarBack.setFillColor(sf::Color(100, 0, 0));
  healthBarBack.setPosition({20.0f, 20.0f});

  sf::RectangleShape healthBarFront({150.0f, 20.0f});
  healthBarFront.setFillColor(sf::Color::Green);
  healthBarFront.setPosition({20.0f, 20.0f});

  while (window.isOpen()) {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>())
        window.close();
    }

    if (player.health > 0) {
      if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        if (shootClock.getElapsedTime().asSeconds() >= player.shootCooldown) {
          bullets.push_back(Bullet(player.getPosition(), player.getAngle()));
          shootClock.restart();
        }
      }

      if (enemies.empty()) {
        currentWave++;
        int enemiesToSpawn = currentWave * 3;
        waveSpeedMultiplier += 0.15f;
        for (int i = 0; i < enemiesToSpawn; i++) {
          float randomY = static_cast<float>(rand() % 540 + 30);
          enemies.push_back(
              Enemy({850.0f + (i * 50), randomY}, waveSpeedMultiplier));
        }
      }

      player.update(window);
      for (auto &enemy : enemies)
        enemy.update(player.getPosition());
      for (auto &bullet : bullets)
        bullet.update();

      for (size_t b = 0; b < bullets.size();) {
        bool bulletHit = false;
        for (size_t e = 0; e < enemies.size();) {
          sf::Vector2f diff =
              bullets[b].getPosition() - enemies[e].getPosition();
          if (std::sqrt(diff.x * diff.x + diff.y * diff.y) < 19.0f) {
            if (rand() % 100 < 30) {
              powerups.push_back(PowerUp(enemies[e].getPosition(), rand() % 2));
            }
            enemies.erase(enemies.begin() + e);
            score += 10;
            bulletHit = true;
            break;
          } else {
            e++;
          }
        }
        if (bulletHit)
          bullets.erase(bullets.begin() + b);
        else
          b++;
      }

      for (size_t i = 0; i < enemies.size(); i++) {
        sf::Vector2f diff = player.getPosition() - enemies[i].getPosition();
        if (std::sqrt(diff.x * diff.x + diff.y * diff.y) < 30.0f) {
          player.health -= 1;
          if (player.health < 0)
            player.health = 0;
        }
      }

      for (size_t i = 0; i < powerups.size();) {
        sf::Vector2f diff = player.getPosition() - powerups[i].getPosition();
        if (std::sqrt(diff.x * diff.x + diff.y * diff.y) < 30.0f) {
          if (powerups[i].getType() == 0) {
            player.health = std::min(player.maxHealth, player.health + 20);
          } else {
            player.shootCooldown =
                std::max(0.10f, player.shootCooldown - 0.03f);
          }
          powerups.erase(powerups.begin() + i);
        } else {
          i++;
        }
      }

      healthBarFront.setSize(
          {(static_cast<float>(player.health) / player.maxHealth) * 150.0f,
           20.0f});
    }

    window.clear(sf::Color(20, 20, 20));

    if (player.health > 0) {
      player.draw(window);
      for (auto &enemy : enemies)
        enemy.draw(window);
      for (auto &bullet : bullets)
        bullet.draw(window);
      for (auto &powerup : powerups)
        powerup.draw(window);

      window.draw(healthBarBack);
      window.draw(healthBarFront);
    } else {
      window.clear(sf::Color(100, 20, 20));
    }

    window.display();
  }
  return 0;
}