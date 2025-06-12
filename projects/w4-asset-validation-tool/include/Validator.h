#pragma once
#include "AssetLoader.h"
#include <vector>
#include <string>

struct ValidationResult {
    bool passed = true;
    std::vector<std::string> warnings;
    std::vector<std::string> errors;
};

class Validator {
public:
    ValidationResult validateAsset(const AssetInfo& asset);
private:
    void checkBasicRequirements(const AssetInfo& asset, ValidationResult& result);
};