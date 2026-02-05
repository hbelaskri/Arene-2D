#include "Background.h"

Background::Background() : tileSprite(texture) {
    for (int i = 0; i < 15; ++i)
        for (int j = 0; j < 20; ++j) map[i][j] = 0;
}

bool Background::load(const std::string& texturePath) {
    if (!texture.loadFromFile(texturePath)) return false;
    tileSprite.setTexture(texture);

   
    sf::IntRect rectT{ {325, 10}, {120, 80} };
    sf::IntRect rectCarre{ {140, 10}, {80, 80} };
    sf::IntRect rectH{ {140, 312}, {150, 40} }; 
    sf::IntRect rectV{ {145, 145}, {40, 120} }; 
   
    formes.push_back({ rectH, {0.f, 0.f}, {20.f, 1.f} });   
    formes.push_back({ rectH, {0.f, 560.f}, {20.f, 1.f} });   
    formes.push_back({ rectV, {0.f, 40.f}, {1.f, 13.f} });     
    formes.push_back({ rectV, {740.f, 60.f}, {1.f, 16.f} }); 

   
    formes.push_back({ rectCarre, {80.f, 80.f}, {6.f, 4.f} });

    formes.push_back({ rectCarre, {480.f, 80.f}, {6.f, 4.f} });


   /* formes.push_back({ rectT, {350.f, 260.f}, {2.5f, 4.f} });*/


    formes.push_back({ rectCarre, {80.f, 360.f}, {6.f, 4.f} });

    formes.push_back({ rectCarre, {480.f, 360.f}, {6.f, 4.f} });


    for (int i = 0; i < 15; ++i) for (int j = 0; j < 20; ++j) map[i][j] = 0;

    for (const auto& f : formes) {
        int startX = static_cast<int>(f.position.x / 20.f);
        int startY = static_cast<int>(f.position.y / 20.f);
        int widthTiles = static_cast<int>(f.sizeInTiles.x);
        int heightTiles = static_cast<int>(f.sizeInTiles.y);

        for (int x = 0; x < widthTiles; x++) {
            for (int y = 0; y < heightTiles; y++) {
                int gridX = startX + x;
                int gridY = startY + y;
                if (gridX >= 0 && gridX < 20 && gridY >= 0 && gridY < 15) {
                    map[gridY][gridX] = 1;
                }
            }
        }
    }
    return true;
}

void Background::draw(sf::RenderWindow& window) const {
    for (const auto& f : formes) {
        tileSprite.setTextureRect(f.textureRect);
        tileSprite.setPosition(f.position);

        auto texSize = tileSprite.getTextureRect().size;
        tileSprite.setScale({
            (f.sizeInTiles.x * 40.f) / static_cast<float>(texSize.x),
            (f.sizeInTiles.y * 40.f) / static_cast<float>(texSize.y)
            });

        tileSprite.setColor(sf::Color(70, 110, 255)); 
        window.draw(tileSprite);
    }
}

bool Background::isWall(int gridX, int gridY) const {
    if (gridX >= 0 && gridX < 20 && gridY >= 0 && gridY < 15) {
        return map[gridY][gridX] == 1;
    }
    return true;
}