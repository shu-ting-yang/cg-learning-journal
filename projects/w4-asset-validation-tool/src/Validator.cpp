#include "Validator.h"

ValidationResult Validator::validateAsset(const AssetInfo& asset) {
    ValidationResult result;
    
    if (!asset.isValid) {
        result.passed = false;
        result.errors.push_back("Failed to load asset: " + asset.errorMessage);
        return result;
    }
    
    checkBasicRequirements(asset, result);
    return result;
}

void Validator::checkBasicRequirements(const AssetInfo& asset, ValidationResult& result) {
    if (asset.meshCount == 0) {
        result.errors.push_back("No meshes found in asset");
        result.passed = false;
    }
    
    if (asset.materialCount == 0) {
        result.warnings.push_back("No materials found - asset may not render correctly");
    }
}