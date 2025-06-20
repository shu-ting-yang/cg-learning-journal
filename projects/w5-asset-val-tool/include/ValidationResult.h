#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

enum class Severity {
    INFO,
    WARNING,
    ERROR,
    CRITICAL
};

struct ValidationIssue {
    Severity severity;
    std::string category;
    std::string message;
    std::string location;
    std::string suggestion;
    
    ValidationIssue(Severity sev, const std::string& cat, const std::string& msg, 
                   const std::string& loc = "", const std::string& sug = "")
        : severity(sev), category(cat), message(msg), location(loc), suggestion(sug) {}
};

struct MeshValidationResult {
    std::string meshName;
    uint32_t vertexCount;
    uint32_t triangleCount;
    uint32_t materialCount;
    std::vector<ValidationIssue> issues;
    bool hasValidUVs;
    
    MeshValidationResult() : vertexCount(0), triangleCount(0), materialCount(0), hasValidUVs(true) {}
};

struct MaterialValidationResult {
    std::string materialName;
    std::vector<std::string> textures;
    std::vector<ValidationIssue> issues;
    bool hasValidTextures;
    
    MaterialValidationResult() : hasValidTextures(true) {}
};

struct ValidationResult {
    std::string filePath;
    bool isValid;
    std::vector<MeshValidationResult> meshResults;
    std::vector<MaterialValidationResult> materialResults;
    std::vector<ValidationIssue> globalIssues;
    
    // Statistics
    uint32_t totalVertices;
    uint32_t totalTriangles;
    uint32_t totalMeshes;
    uint32_t totalMaterials;
    uint32_t totalTextures;
    
    ValidationResult() : isValid(true), totalVertices(0), totalTriangles(0), 
                        totalMeshes(0), totalMaterials(0), totalTextures(0) {}
    
    void addIssue(const ValidationIssue& issue) {
        globalIssues.push_back(issue);
        if (issue.severity == Severity::ERROR || issue.severity == Severity::CRITICAL) {
            isValid = false;
        }
    }
};

// JSON serialization helpers
void to_json(nlohmann::json& j, const Severity& severity);
void to_json(nlohmann::json& j, const ValidationIssue& issue);
void to_json(nlohmann::json& j, const MeshValidationResult& result);
void to_json(nlohmann::json& j, const MaterialValidationResult& result);
void to_json(nlohmann::json& j, const ValidationResult& result);