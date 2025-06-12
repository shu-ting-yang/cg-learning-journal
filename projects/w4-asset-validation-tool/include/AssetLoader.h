#pragma once
#include <string>
#include <vector>

// Simple structure to start with
struct AssetInfo {
    std::string filename;
    int meshCount = 0;
    int materialCount = 0;
    bool isValid = false;
    std::string errorMessage;
};

class AssetLoader {
public:
    AssetInfo loadAsset(const std::string& filepath);
};