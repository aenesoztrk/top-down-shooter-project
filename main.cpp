#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include <vector>

const float PI = 3.1415f;
const sf::Color GRASS_COLOR(76, 153, 76);

int loadHighScore(const std::string &path) {
  std::ifstream file(path);
  int score = 0;
  if (file.is_open())
    file >> score;
  return score;
}

void saveHighScore(const std::string &path, int score) {
  std::ofstream file(path);
  if (file.is_open())
    file << score;
}

void centerText(sf::Text &text) {
  sf::FloatRect bounds = text.getLocalBounds();
  text.setOrigin(bounds.position + bounds.size / 2.0f);
}

class Bullet {
private:
  sf::Sprite sprite;
  sf::CircleShape fallback;
  bool hasTexture;
  sf::Vector2f direction;
  float speed;

public:
  Bullet(sf::Vector2f position, float angle, const sf::Texture &tex,
         bool useTex)
      : sprite(tex) {
    speed = 10.0f;
    hasTexture = useTex;
    direction.x = std::cos(angle * PI / 180.0f);
    direction.y = std::sin(angle * PI / 180.0f);
    if (hasTexture) {
      sprite.setTexture(tex);
      sprite.setOrigin({tex.getSize().x / 2.0f, tex.getSize().y / 2.0f});
      sprite.setPosition(position);
      sprite.setScale({0.04f, 0.04f});
      sprite.setRotation(sf::degrees(angle));
    } else {
      fallback.setRadius(4.0f);
      fallback.setFillColor(sf::Color::Yellow);
      fallback.setOrigin({4.0f, 4.0f});
      fallback.setPosition(position);
    }
  }
  void update() {
    if (hasTexture)
      sprite.move(direction * speed);
    else
      fallback.move(direction * speed);
  }
  void draw(sf::RenderWindow &window) {
    if (hasTexture)
      window.draw(sprite);
    else
      window.draw(fallback);
  }
  sf::Vector2f getPosition() const {
    return hasTexture ? sprite.getPosition() : fallback.getPosition();
  }
};

class PowerUp {
private:
  sf::Sprite sprite;
  sf::CircleShape fallback;
  bool hasTexture;
  int type;

public:
  PowerUp(sf::Vector2f position, int pType, const sf::Texture &tex, bool useTex)
      : sprite(tex), type(pType), hasTexture(useTex) {
    if (hasTexture) {
      sprite.setTexture(tex);
      sprite.setOrigin({tex.getSize().x / 2.0f, tex.getSize().y / 2.0f});
      sprite.setPosition(position);
      sprite.setScale({0.075f, 0.075f});
    } else {
      fallback.setRadius(10.0f);
      fallback.setOrigin({10.0f, 10.0f});
      fallback.setPosition(position);
      if (type == 0)
        fallback.setFillColor(sf::Color::Cyan);
      else
        fallback.setFillColor(sf::Color::Magenta);
    }
  }
  void draw(sf::RenderWindow &window) {
    if (hasTexture)
      window.draw(sprite);
    else
      window.draw(fallback);
  }
  sf::Vector2f getPosition() const {
    return hasTexture ? sprite.getPosition() : fallback.getPosition();
  }
  int getType() const { return type; }
};

class Coin {
private:
  sf::Sprite sprite;
  sf::CircleShape fallback;
  bool hasTexture;

public:
  Coin(sf::Vector2f position, const sf::Texture &tex, bool useTex)
      : sprite(tex), hasTexture(useTex) {
    if (hasTexture) {
      sprite.setTexture(tex);
      sprite.setOrigin({tex.getSize().x / 2.0f, tex.getSize().y / 2.0f});
      sprite.setPosition(position);
      sprite.setScale({0.13f, 0.13f});
    } else {
      fallback.setRadius(8.0f);
      fallback.setFillColor(sf::Color(255, 215, 0));
      fallback.setOrigin({8.0f, 8.0f});
      fallback.setPosition(position);
    }
  }
  void draw(sf::RenderWindow &window) {
    if (hasTexture)
      window.draw(sprite);
    else
      window.draw(fallback);
  }
  sf::Vector2f getPosition() const {
    return hasTexture ? sprite.getPosition() : fallback.getPosition();
  }
};

class Player {
private:
  sf::Sprite sprite;
  sf::CircleShape fallback;
  bool hasTexture;
  bool hasRight;
  bool hasLeft;
  const sf::Texture *texRight;
  const sf::Texture *texLeft;
  float speed;
  float currentAngle;

public:
  int health;
  int maxHealth;
  float shootCooldown;
  Player(const sf::Texture &texR, bool useR, const sf::Texture &texL, bool useL)
      : sprite(useR ? texR : texL) {
    speed = 4.0f;
    currentAngle = 0.0f;
    maxHealth = 100;
    health = maxHealth;
    shootCooldown = 0.25f;
    hasRight = useR;
    hasLeft = useL;
    hasTexture = useR || useL;
    texRight = &texR;
    texLeft = &texL;
    if (hasTexture) {
      const sf::Texture &init = hasRight ? texR : texL;
      sprite.setTexture(init, true);
      sprite.setOrigin({init.getSize().x / 2.0f, init.getSize().y / 2.0f});
      sprite.setPosition({400.0f, 300.0f});
      sprite.setScale({0.2f, 0.2f});
    } else {
      fallback.setRadius(20.0f);
      fallback.setFillColor(sf::Color::Green);
      fallback.setOrigin({20.0f, 20.0f});
      fallback.setPosition({400.0f, 300.0f});
    }
  }
  void reset() {
    health = maxHealth;
    shootCooldown = 0.25f;
    if (hasTexture)
      sprite.setPosition({400.0f, 300.0f});
    else
      fallback.setPosition({400.0f, 300.0f});
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
    if (hasTexture) {
      sprite.move(movement * speed);
      sf::Vector2f pos = sprite.getPosition();
      if (pos.x < 20)
        sprite.setPosition({20.0f, pos.y});
      if (pos.x > 780)
        sprite.setPosition({780.0f, pos.y});
      if (pos.y < 20)
        sprite.setPosition({pos.x, 20.0f});
      if (pos.y > 580)
        sprite.setPosition({pos.x, 580.0f});
    } else {
      fallback.move(movement * speed);
      sf::Vector2f pos = fallback.getPosition();
      if (pos.x < 20)
        fallback.setPosition({20.0f, pos.y});
      if (pos.x > 780)
        fallback.setPosition({780.0f, pos.y});
      if (pos.y < 20)
        fallback.setPosition({pos.x, 20.0f});
      if (pos.y > 580)
        fallback.setPosition({pos.x, 580.0f});
    }
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f myPos =
        hasTexture ? sprite.getPosition() : fallback.getPosition();
    float dx = mousePos.x - myPos.x;
    float dy = mousePos.y - myPos.y;
    currentAngle = std::atan2(dy, dx) * 180.0f / PI;
    if (hasTexture) {
      bool facingRight = (dx >= 0.0f);
      if (facingRight) {
        if (hasRight) {
          sprite.setTexture(*texRight, true);
          sprite.setScale({0.2f, 0.2f});
        } else {
          sprite.setTexture(*texLeft, true);
          sprite.setScale({-0.2f, 0.2f});
        }
        sprite.setRotation(sf::degrees(currentAngle));
      } else {
        if (hasLeft) {
          sprite.setTexture(*texLeft, true);
          sprite.setScale({0.2f, 0.2f});
        } else {
          sprite.setTexture(*texRight, true);
          sprite.setScale({-0.2f, 0.2f});
        }
        sprite.setRotation(sf::degrees(currentAngle - 180.0f));
      }
    } else {
      fallback.setRotation(sf::degrees(currentAngle));
    }
  }
  void draw(sf::RenderWindow &window) {
    if (hasTexture)
      window.draw(sprite);
    else
      window.draw(fallback);
  }
  sf::Vector2f getPosition() const {
    return hasTexture ? sprite.getPosition() : fallback.getPosition();
  }
  float getAngle() const { return currentAngle; }
};

class Enemy {
private:
  sf::Sprite sprite;
  sf::CircleShape fallback;
  bool hasTexture;
  float speed;

public:
  Enemy(sf::Vector2f position, float speedMultiplier, const sf::Texture &tex,
        bool useTex)
      : sprite(tex) {
    speed = 1.2f * speedMultiplier;
    hasTexture = useTex;
    if (hasTexture) {
      sprite.setTexture(tex);
      sprite.setOrigin({tex.getSize().x / 2.0f, tex.getSize().y / 2.0f});
      sprite.setPosition(position);
      sprite.setScale({0.1f, 0.1f});
    } else {
      fallback.setRadius(15.0f);
      fallback.setFillColor(sf::Color::Red);
      fallback.setOrigin({15.0f, 15.0f});
      fallback.setPosition(position);
    }
  }
  void update(sf::Vector2f playerPosition) {
    sf::Vector2f myPos =
        hasTexture ? sprite.getPosition() : fallback.getPosition();
    sf::Vector2f direction = playerPosition - myPos;
    float distance =
        std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (distance > 0.0f) {
      direction.x /= distance;
      direction.y /= distance;
      if (hasTexture)
        sprite.move(direction * speed);
      else
        fallback.move(direction * speed);
    }
  }
  void draw(sf::RenderWindow &window) {
    if (hasTexture)
      window.draw(sprite);
    else
      window.draw(fallback);
  }
  sf::Vector2f getPosition() const {
    return hasTexture ? sprite.getPosition() : fallback.getPosition();
  }
};

enum GameState { MENU, PLAYING, GAME_OVER };
int main() {
  sf::RenderWindow window(sf::VideoMode({800, 600}),
                          "Top-Down Shooter - Ahmet Enes Ozturk");
  window.setFramerateLimit(60);
  srand(static_cast<unsigned int>(time(0)));
  GameState state = MENU;
  sf::Font font;
  bool fontLoaded =
      font.openFromFile("/System/Library/Fonts/Supplemental/Arial.ttf");
  if (!fontLoaded)
    fontLoaded = font.openFromFile("assets/arial.ttf");
  const std::string highScorePath = "highscore.txt";
  int highScore = loadHighScore(highScorePath);
  sf::Texture tPlayer, tPlayerLeft, tEnemy, tEnemyRight, tEnemyLeft, tBullet,
      tHealth, tSpeed, tCoin;
  bool uPlayer = tPlayer.loadFromFile("assets/player_right.png");
  if (!uPlayer)
    uPlayer = tPlayer.loadFromFile("assets/player.png");
  bool uPlayerLeft = tPlayerLeft.loadFromFile("assets/player_left.png");
  bool uEnemy = tEnemy.loadFromFile("assets/enemy.png");
  bool uEnemyRight = tEnemyRight.loadFromFile("assets/enemy_right.png");
  bool uEnemyLeft = tEnemyLeft.loadFromFile("assets/enemy_left.png");
  bool uBullet = tBullet.loadFromFile("assets/bullet.png");
  bool uHealth = tHealth.loadFromFile("assets/powerup_health.png");
  bool uSpeed = tSpeed.loadFromFile("assets/powerup_speed.png");
  bool uCoin = tCoin.loadFromFile("assets/coin.png");
  const sf::Texture &enemyRightTex = uEnemyRight ? tEnemyRight : tEnemy;
  bool enemyRightUse = uEnemyRight || uEnemy;
  const sf::Texture &enemyLeftTex = uEnemyLeft ? tEnemyLeft : tEnemy;
  bool enemyLeftUse = uEnemyLeft || uEnemy;
  Player player(tPlayer, uPlayer, tPlayerLeft, uPlayerLeft);
  std::vector<Bullet> bullets;
  std::vector<Enemy> enemies;
  std::vector<PowerUp> powerups;
  std::vector<Coin> coins;
  int score = 0;
  int coinCount = 0;
  int currentWave = 0;
  float waveSpeedMultiplier = 1.0f;
  sf::Clock shootClock;
  sf::RectangleShape playButton({200.0f, 60.0f});
  playButton.setFillColor(sf::Color::Blue);
  playButton.setPosition({300.0f, 270.0f});
  sf::RectangleShape healthBarBack({150.0f, 20.0f});
  healthBarBack.setFillColor(sf::Color(100, 0, 0));
  healthBarBack.setPosition({20.0f, 20.0f});
  sf::RectangleShape healthBarFront({150.0f, 20.0f});
  healthBarFront.setFillColor(sf::Color::Green);
  healthBarFront.setPosition({20.0f, 20.0f});
  sf::Text titleText(font, "TOP-DOWN SHOOTER", 48);
  titleText.setFillColor(sf::Color::White);
  titleText.setStyle(sf::Text::Bold);
  sf::Text buttonText(font, "OYUNA BASLA", 26);
  buttonText.setFillColor(sf::Color::White);
  buttonText.setStyle(sf::Text::Bold);
  sf::Text menuHighScoreText(font, "", 26);
  menuHighScoreText.setFillColor(sf::Color::Yellow);
  sf::Text hpText(font, "", 14);
  hpText.setFillColor(sf::Color::White);
  hpText.setStyle(sf::Text::Bold);
  sf::Text scoreText(font, "", 18);
  scoreText.setFillColor(sf::Color::White);
  sf::Text gameOverText(font, "OYUN BITTI", 48);
  gameOverText.setFillColor(sf::Color::White);
  gameOverText.setStyle(sf::Text::Bold);
  sf::Text gameOverInfo(font, "", 24);
  gameOverInfo.setFillColor(sf::Color::White);
  sf::Text gameOverHint(font, "Menu icin tikla", 20);
  gameOverHint.setFillColor(sf::Color::White);
  auto startGame = [&]() {
    state = PLAYING;
    player.reset();
    score = 0;
    coinCount = 0;
    currentWave = 0;
    waveSpeedMultiplier = 1.0f;
    enemies.clear();
    bullets.clear();
    powerups.clear();
    coins.clear();
  };
  while (window.isOpen()) {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>())
        window.close();
      if (state == MENU && event->is<sf::Event::MouseButtonPressed>()) {
        sf::Vector2i mPos = sf::Mouse::getPosition(window);
        if (playButton.getGlobalBounds().contains(
                {static_cast<float>(mPos.x), static_cast<float>(mPos.y)})) {
          startGame();
        }
      }
      if (state == GAME_OVER && event->is<sf::Event::MouseButtonPressed>()) {
        state = MENU;
      }
    }
    if (state == PLAYING) {
      if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        if (shootClock.getElapsedTime().asSeconds() >= player.shootCooldown) {
          bullets.push_back(Bullet(player.getPosition(), player.getAngle(),
                                   tBullet, uBullet));
          shootClock.restart();
        }
      }
      if (enemies.empty()) {
        currentWave++;
        int enemiesToSpawn = currentWave * 3;
        waveSpeedMultiplier += 0.15f;
        for (int i = 0; i < enemiesToSpawn; i++) {
          float randomY = static_cast<float>(rand() % 540 + 30);
          bool fromLeft = (rand() % 2 == 0);
          if (fromLeft) {
            enemies.push_back(Enemy({-50.0f - (i * 50), randomY},
                                    waveSpeedMultiplier, enemyLeftTex,
                                    enemyLeftUse));
          } else {
            enemies.push_back(Enemy({850.0f + (i * 50), randomY},
                                    waveSpeedMultiplier, enemyRightTex,
                                    enemyRightUse));
          }
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
          if (std::sqrt(diff.x * diff.x + diff.y * diff.y) < 25.0f) {
            coins.push_back(Coin(enemies[e].getPosition(), tCoin, uCoin));
            if (rand() % 100 < 30) {
              sf::Vector2f puPos =
                  enemies[e].getPosition() + sf::Vector2f(55.0f, 0.0f);
              int puType = rand() % 2;
              const sf::Texture &puTex = (puType == 0) ? tHealth : tSpeed;
              bool puUse = (puType == 0) ? uHealth : uSpeed;
              powerups.push_back(PowerUp(puPos, puType, puTex, puUse));
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
          if (player.health <= 0) {
            player.health = 0;
            if (coinCount > highScore) {
              highScore = coinCount;
              saveHighScore(highScorePath, highScore);
            }
            state = GAME_OVER;
          }
        }
      }
      for (size_t i = 0; i < powerups.size();) {
        sf::Vector2f diff = player.getPosition() - powerups[i].getPosition();
        if (std::sqrt(diff.x * diff.x + diff.y * diff.y) < 30.0f) {
          if (powerups[i].getType() == 0)
            player.health = std::min(player.maxHealth, player.health + 20);
          else
            player.shootCooldown =
                std::max(0.10f, player.shootCooldown - 0.03f);
          powerups.erase(powerups.begin() + i);
        } else {
          i++;
        }
      }
      for (size_t i = 0; i < coins.size();) {
        sf::Vector2f diff = player.getPosition() - coins[i].getPosition();
        if (std::sqrt(diff.x * diff.x + diff.y * diff.y) < 30.0f) {
          coinCount++;
          coins.erase(coins.begin() + i);
        } else {
          i++;
        }
      }
      healthBarFront.setSize(
          {(static_cast<float>(player.health) / player.maxHealth) * 150.0f,
           20.0f});
    }
    window.clear(GRASS_COLOR);
    if (state == MENU) {
      titleText.setString("TOP-DOWN SHOOTER");
      centerText(titleText);
      titleText.setPosition({400.0f, 160.0f});
      window.draw(titleText);
      menuHighScoreText.setString("En Yuksek Coin: " +
                                  std::to_string(highScore));
      centerText(menuHighScoreText);
      menuHighScoreText.setPosition({400.0f, 220.0f});
      window.draw(menuHighScoreText);
      window.draw(playButton);
      centerText(buttonText);
      buttonText.setPosition({400.0f, 300.0f});
      window.draw(buttonText);
    } else if (state == PLAYING) {
      player.draw(window);
      for (auto &enemy : enemies)
        enemy.draw(window);
      for (auto &bullet : bullets)
        bullet.draw(window);
      for (auto &powerup : powerups)
        powerup.draw(window);
      for (auto &coin : coins)
        coin.draw(window);
      window.draw(healthBarBack);
      window.draw(healthBarFront);
      hpText.setString(std::to_string(player.health) + " HP");
      centerText(hpText);
      hpText.setPosition({95.0f, 30.0f});
      window.draw(hpText);
      scoreText.setString("Coin: " + std::to_string(coinCount) +
                          "   Dalga: " + std::to_string(currentWave) +
                          "   En Yuksek: " + std::to_string(highScore));
      scoreText.setPosition({20.0f, 48.0f});
      window.draw(scoreText);
    } else if (state == GAME_OVER) {
      centerText(gameOverText);
      gameOverText.setPosition({400.0f, 240.0f});
      window.draw(gameOverText);
      gameOverInfo.setString("Coin: " + std::to_string(coinCount) +
                             "    En Yuksek: " + std::to_string(highScore));
      centerText(gameOverInfo);
      gameOverInfo.setPosition({400.0f, 300.0f});
      window.draw(gameOverInfo);
      centerText(gameOverHint);
      gameOverHint.setPosition({400.0f, 350.0f});
      window.draw(gameOverHint);
    }
    window.display();
  }
  return 0;
}