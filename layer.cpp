#include "header/layer.h"
#include "header/pch.h"

// Layer constructor
Layer::Layer(const std::string& fileName, float gridSize)
    : gridSize(gridSize) {
    loadFromFile(fileName);
    createSprites();
}

void Layer::loadFromFile(const std::string& fileName) {
    std::string fullName = "text/" + fileName;
    std::ifstream file(fullName);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << fileName << std::endl;
        return;
    }

    std::string line;

    // Read the first line for texture mapping
    std::getline(file, line);
    std::istringstream textureStream(line);
    int textureID;
    std::string textureFileName;
    while (textureStream >> textureID >> textureFileName) {
        // Prepend the folder path
        std::string fullPath = "images/" + textureFileName;

        // Load the texture and store it in the map
        sf::Texture texture;
        if (!texture.loadFromFile(fullPath)) {
            std::cerr << "Error: Could not load texture: " << fullPath << std::endl;
            return;
        }
        textures[textureID] = texture;
    }

    // Read the remaining lines for the grid
    while (std::getline(file, line)) {
        std::vector<Cell> row;
        std::istringstream stream(line);
        int type;
        float x = 0.0f;
        while (stream >> type) {
            // Only create a cell if the type is a valid texture ID
            if (textures.find(type) != textures.end()) {
                row.push_back(Cell(sf::Vector2f(x * gridSize, grid.size() * gridSize), type));
            } else {
                row.push_back(Cell());  // Empty cell (type = 0)
            }
            x++;
        }
        grid.push_back(row);
    }
}


// Create sprites based on grid data and texture mapping
void Layer::createSprites() {
    for (size_t y = 0; y < grid.size(); y++) {
        for (size_t x = 0; x < grid[y].size(); x++) {
            // Only create a sprite if the cell's type is valid
            if (grid[y][x].spriteType != 0) {
                // Get the corresponding texture from the textures map
                sf::Sprite sprite(textures[grid[y][x].spriteType]);
                sprite.setPosition(grid[y][x].position);
                sprites.push_back(sprite);
            }
        }
    }
}

// Draw the grid's sprites to the window
void Layer::draw(sf::RenderWindow& window) {
    for (const auto& sprite : sprites) {
        window.draw(sprite);
    }
}
