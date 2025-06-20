#pragma once

#include <string>
#include <vector>
#include <regex>
#include <nlohmann/json.hpp>

struct ValidationConfig {
    // Polygon limits
    uint32_t maxTrianglesPerMesh = 50000;
    uint32_t maxVerticesPerMesh = 65536;
    uint32_t maxTotalTriangles = 500000;
    
    // Texture limits
    uint32_t maxTextureWidth = 4096;
    uint32_t maxTextureHeight = 4096;
    bool requirePowerOfTwo = true;
    std::vector<std::string> allowedTextureFormats = {"png", "jpg", "jpeg", "tga", "bmp"};
    
    // UV validation
    bool checkUVRange = true;
    bool checkUVOverlaps = true;
    float uvTolerance = 0.001f;
    
    // Naming conventions
    bool enforceNamingConventions = true;
    std::string meshNamePattern = "^[a-zA-Z][a-zA-Z0-9_]*$";
    std::string materialNamePattern = "^[a-zA-Z][a-zA-Z0-9_]*$";
    std::string textureNamePattern = "^[a-zA-Z][a-zA-Z0-9_]*\\.(png|jpg|jpeg|tga|bmp)$";
    
    // Material validation
    bool requireAlbedoTexture = false;
    bool requireNormalTexture = false;
    bool requireMetallicRoughnessTexture = false;
    
    // File validation
    bool checkFileExists = true;
    bool validateTextureFiles = true;
    
    // Performance
    bool enableDetailedLogging = true;
    bool generateSuggestions = true;
    
    // Load configuration from JSON file
    static ValidationConfig loadFromFile(const std::string& configPath);
    
    // Save configuration to JSON file
    void saveToFile(const std::string& configPath) const;
    
    // Get compiled regex patterns
    std::regex getMeshNameRegex() const;
    std::regex getMaterialNameRegex() const;
    std::regex getTextureNameRegex() const;
};

// JSON serialization
void to_json(nlohmann::json& j, const ValidationConfig& config);
void from_json(const nlohmann::json& j, ValidationConfig& config);