#pragma once

#include "ValidationResult.h"
#include "ValidationConfig.h"
#include "AssetLoader.h"
#include <glm/glm.hpp>
#include <memory>

class Validator {
public:
    Validator();
    explicit Validator(const ValidationConfig& config);
    ~Validator();
    
    // Main validation function
    ValidationResult validate(const SceneData& sceneData);
    
    // Individual validation functions
    void validateMesh(const MeshData& mesh, MeshValidationResult& result);
    void validateMaterial(const MaterialData& material, MaterialValidationResult& result);
    void validateTexture(const TextureData& texture, std::vector<ValidationIssue>& issues);
    void validateNamingConventions(const SceneData& sceneData, std::vector<ValidationIssue>& issues);
    void validateSceneLimits(const SceneData& sceneData, std::vector<ValidationIssue>& issues);
    
    // Configuration management
    void setConfig(const ValidationConfig& config) { this->config = config; }
    const ValidationConfig& getConfig() const { return config; }

private:
    ValidationConfig config;
    
    // Detailed validation functions
    void validatePolygonCount(const MeshData& mesh, std::vector<ValidationIssue>& issues);
    void validateUVCoordinates(const MeshData& mesh, std::vector<ValidationIssue>& issues);
    void validateVertexData(const MeshData& mesh, std::vector<ValidationIssue>& issues);
    void validateMeshNaming(const std::string& name, std::vector<ValidationIssue>& issues);
    
    void validateMaterialProperties(const MaterialData& material, std::vector<ValidationIssue>& issues);
    void validateMaterialTextures(const MaterialData& material, std::vector<ValidationIssue>& issues);
    void validateMaterialNaming(const std::string& name, std::vector<ValidationIssue>& issues);
    
    void validateTextureResolution(const TextureData& texture, std::vector<ValidationIssue>& issues);
    void validateTextureFormat(const TextureData& texture, std::vector<ValidationIssue>& issues);
    void validateTexturePowerOfTwo(const TextureData& texture, std::vector<ValidationIssue>& issues);
    void validateTextureNaming(const std::string& name, std::vector<ValidationIssue>& issues);
    
    // UV validation helpers
    bool checkUVRange(const std::vector<glm::vec2>& uvCoords);
    bool checkUVOverlaps(const std::vector<glm::vec2>& uvCoords, const std::vector<uint32_t>& indices);
    float calculateUVOverlapPercentage(const std::vector<glm::vec2>& uvCoords, const std::vector<uint32_t>& indices);
    
    // Utility functions
    bool isPowerOfTwo(uint32_t value);
    std::string getSeverityString(Severity severity);
    std::string generateSuggestion(const std::string& category, const std::string& issue);
    
    // Statistics
    void calculateStatistics(const SceneData& sceneData, ValidationResult& result);
};