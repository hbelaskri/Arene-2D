#include "Collectibles.h"
#include <iostream>

Collectibles::Collectibles(const Background& bg)
    : currentFrame(0), frameTime(0.f), animationSpeed(0.1f)
{
    if (!coinTexture.loadFromFile("../Assets/pacmanPack/Coin.png")) {
        std::cerr << "Erreur: Impossible de charger Coin.png" << std::endl;
    }
    if (!bigCoinTexture.loadFromFile("../Assets/pacmanPack/BigCoin.png")) {
        std::cerr << "Erreur: Impossible de charger BigCoin.png" << std::endl;
    }

    coinFrameSize.x = static_cast<int>(coinTexture.getSize().x) / 8;
    coinFrameSize.y = static_cast<int>(coinTexture.getSize().y);

    bigCoinFrameSize.x = static_cast<int>(bigCoinTexture.getSize().x) / 8;
    bigCoinFrameSize.y = static_cast<int>(bigCoinTexture.getSize().y);

    std::vector<sf::Vector2i> cornerPositions = {
        {1, 1}, {28, 1}, {1, 18}, {28, 18}
    };

    for (int y = 0; y < Background::MAP_HEIGHT; ++y) {
        for (int x = 0; x < Background::MAP_WIDTH; ++x) {

            if (bg.getTile(x, y) == 0) {
                sf::Vector2i currentPos = { x, y };
                bool isCorner = false;

                for (const auto& corner : cornerPositions) {
                    if (corner == currentPos) {
                        isCorner = true;
                        break;
                    }
                }

                if (isCorner) {
                    Item item(bigCoinTexture); 
                    item.gridPos = currentPos;
                    setupSprite(item.sprite, bigCoinFrameSize, currentPos, 0.9f);
                    powerPellets.push_back(item);
                }
                else {
                    Item item(coinTexture); 
                    item.gridPos = currentPos;
                    setupSprite(item.sprite, coinFrameSize, currentPos, 0.5f);
                    coins.push_back(item);
                }
            }
        }
    }
}

void Collectibles::setupSprite(sf::Sprite& sprite, sf::Vector2i& frameSize, sf::Vector2i pos, float scaleFactor) {

    sprite.setTextureRect(sf::IntRect({ 0, 0 }, { frameSize.x, frameSize.y }));
    sprite.setOrigin({ static_cast<float>(frameSize.x) / 2.f, static_cast<float>(frameSize.y) / 2.f });

    float targetSize = Background::TILE_SIZE * scaleFactor;
    float scale = targetSize / frameSize.x;
    sprite.setScale({ scale, scale });

    sprite.setPosition(Navigation::GridToWorld(pos));
}

void Collectibles::update(float deltaTime) {
    frameTime += deltaTime;
    if (frameTime >= animationSpeed) {
        frameTime = 0.f;
        currentFrame = (currentFrame + 1) % 8;
    }

    for (auto& p : powerPellets) {
        if (p.isActive) {
            p.sprite.setTextureRect(sf::IntRect({ currentFrame * bigCoinFrameSize.x, 0 }, { bigCoinFrameSize.x, bigCoinFrameSize.y }));
        }
    }

    for (auto& c : coins) {
        if (c.isActive) {
            c.sprite.setTextureRect(sf::IntRect({ currentFrame * coinFrameSize.x, 0 }, { coinFrameSize.x, coinFrameSize.y }));
        }
    }
}

bool Collectibles::checkCollision(const sf::FloatRect& playerBounds) {
    bool powerPelletEaten = false;

    for (auto& p : powerPellets) {
        if (p.isActive && p.sprite.getGlobalBounds().findIntersection(playerBounds)) {
            p.isActive = false;
            powerPelletEaten = true;
        }
    }

    for (auto& c : coins) {
        if (c.isActive && c.sprite.getGlobalBounds().findIntersection(playerBounds)) {
            c.isActive = false;
        }
    }

    return powerPelletEaten;
}

bool Collectibles::allCollected() const {
    for (const auto& p : powerPellets) if (p.isActive) return false;
    for (const auto& c : coins) if (c.isActive) return false;
    return true;
}

void Collectibles::draw(sf::RenderWindow& window) {
    for (const auto& c : coins) {
        if (c.isActive) window.draw(c.sprite);
    }
    for (const auto& p : powerPellets) {
        if (p.isActive) window.draw(p.sprite);
    }
}

void Collectibles::reset() {
    for (auto& p : powerPellets) p.isActive = true;
    for (auto& c : coins) c.isActive = true;
}