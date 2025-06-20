#include "Validator.h"
#include <fmt/core.h>
#include <algorithm>
#include <unordered_set>
#include <cmath>

Validator::Validator() = default;

Validator::Validator(const ValidationConfig& config) : config(config) {}

Validator::~Validator() = default;

ValidationResult Validator::validate(const SceneData& sceneData) {
    ValidationResult result;
    result.filePath = sceneData.filePath;
    
    // Validate scene-level limits
    validateSceneLimits(sceneData, result.globalIssues);
    
    // Validate naming conventions
    if (config.enforceNamingConventions) {
        validateNamingConventions(sceneData, result.globalIssues);
    }
    
    // Validate each mesh
    result.meshResults.reserve(sceneData.meshes.size());
    for (const auto& mesh : sceneData.meshes) {
        MeshValidationResult meshResult;
        validateMesh(mesh, meshResult);
        result.meshResults.push_back(std::move(meshResult));
    }
    
    // Validate each material
    result.materialResults.reserve(sceneData.materials.size());
    for (const auto& material : sceneData.materials) {
        MaterialValidationResult materialResult;
        validateMaterial(material, materialResult);
        result.materialResults.push_back(std::move(materialResult));
    }
    
    // Validate each texture
    for (const auto& texture : sceneData.textures) {
        validateTexture(texture, result.globalIssues);
    }
    
    // Calculate statistics
    calculateStatistics(sceneData, result);
    
    // Determine overall validation result
    for (const auto& issue : result.globalIssues) {
        if (issue.severity == Severity::ERROR || issue.severity == Severity::CRITICAL) {
            result.isValid = false;
            break;
        }
    }
    
    if (result.isValid) {
        for (const auto& meshResult : result.meshResults) {
            for (const auto& issue : meshResult.issues) {
                if (issue.severity == Severity::ERROR || issue.severity == Severity::CRITICAL) {
                    result.isValid = false;
                    break;
                }
            }
            if (!result.isValid) break;
        }
    }
    
    if (result.isValid) {
        for (const auto& materialResult : result.materialResults) {
            for (const auto& issue : materialResult.issues) {
                if (issue.severity == Severity::ERROR || issue.severity == Severity::CRITICAL) {
                    result.isValid = false;
                    break;
                }
            }
            if (!result.isValid) break;
        }
    }
    
    return result;
}

void Validator::validateMesh(const MeshData& mesh, MeshValidationResult& result) {
    result.meshName = mesh.name;
    result.vertexCount = mesh.vertices.size();
    result.triangleCount = mesh.triangleCount;
    result.materialCount = mesh.materialIndices.size();
    
    // Validate polygon count
    validatePolygonCount(mesh, result.issues);
    
    // Validate UV coordinates
    if (config.checkUVRange || config.checkUVOverlaps) {
        validateUVCoordinates(mesh, result.issues);
    }
    
    // Validate vertex data
    validateVertexData(mesh, result.issues);
    
    // Validate mesh naming
    if (config.enforceNamingConventions) {
        validateMeshNaming(mesh.name, result.issues);
    }
    
    // Set UV validation flag
    result.hasValidUVs = checkUVRange(mesh.uvCoords);
}

void Validator::validateMaterial(const MaterialData& material, MaterialValidationResult& result) {
    result.materialName = material.name;
    
    // Collect all textures
    if (!material.albedoTexture.empty()) {
        result.textures.push_back(material.albedoTexture);
    }
    if (!material.normalTexture.empty()) {
        result.textures.push_back(material.normalTexture);
    }
    if (!material.metallicRoughnessTexture.empty()) {
        result.textures.push_back(material.metallicRoughnessTexture);
    }
    if (!material.emissiveTexture.empty()) {
        result.textures.push_back(material.emissiveTexture);
    }
    if (!material.occlusionTexture.empty()) {
        result.textures.push_back(material.occlusionTexture);
    }
    
    // Validate material properties
    validateMaterialProperties(material, result.issues);
    
    // Validate material textures
    validateMaterialTextures(material, result.issues);
    
    // Validate material naming
    if (config.enforceNamingConventions) {
        validateMaterialNaming(material.name, result.issues);
    }
    
    result.hasValidTextures = true;
    for (const auto& issue : result.issues) {
        if (issue.severity == Severity::ERROR || issue.severity == Severity::CRITICAL) {
            result.hasValidTextures = false;
            break;
        }
    }
}

void Validator::validateTexture(const TextureData& texture, std::vector<ValidationIssue>& issues) {
    // Validate texture resolution
    validateTextureResolution(texture, issues);
    
    // Validate texture format
    validateTextureFormat(texture, issues);
    
    // Validate power of two
    if (config.requirePowerOfTwo) {
        validateTexturePowerOfTwo(texture, issues);
    }
    
    // Validate texture naming
    if (config.enforceNamingConventions) {
        validateTextureNaming(texture.name, issues);
    }
}

void Validator::validateNamingConventions(const SceneData& sceneData, std::vector<ValidationIssue>& issues) {
    // Mesh names are validated per-mesh
    // Material names are validated per-material
    // Texture names are validated per-texture
    
    // Check for duplicate names
    std::unordered_set<std::string> meshNames;
    for (const auto& mesh : sceneData.meshes) {
        if (!meshNames.insert(mesh.name).second) {
            issues.emplace_back(Severity::WARNING, "naming", 
                               "Duplicate mesh name: " + mesh.name,
                               "Scene", "Use unique names for all meshes");
        }
    }
    
    std::unordered_set<std::string> materialNames;
    for (const auto& material : sceneData.materials) {
        if (!materialNames.insert(material.name).second) {
            issues.emplace_back(Severity::WARNING, "naming", 
                               "Duplicate material name: " + material.name,
                               "Scene", "Use unique names for all materials");
        }
    }
}

void Validator::validateSceneLimits(const SceneData& sceneData, std::vector<ValidationIssue>& issues) {
    // Check total triangle count
    if (sceneData.totalTriangles > config.maxTotalTriangles) {
        issues.emplace_back(Severity::ERROR, "performance", 
                           fmt::format("Total triangle count ({}) exceeds limit ({})", 
                                     sceneData.totalTriangles, config.maxTotalTriangles),
                           "Scene", 
                           fmt::format("Reduce total triangles to under {}", config.maxTotalTriangles));
    }
    
    // Check mesh count
    if (sceneData.meshes.size() > 1000) { // Arbitrary large limit
        issues.emplace_back(Severity::WARNING, "performance", 
                           fmt::format("High mesh count: {}", sceneData.meshes.size()),
                           "Scene", "Consider merging meshes to improve performance");
    }
}

void Validator::validatePolygonCount(const MeshData& mesh, std::vector<ValidationIssue>& issues) {
    // Check triangle count
    if (mesh.triangleCount > config.maxTrianglesPerMesh) {
        issues.emplace_back(Severity::ERROR, "geometry", 
                           fmt::format("Triangle count ({}) exceeds limit ({})", 
                                     mesh.triangleCount, config.maxTrianglesPerMesh),
                           mesh.name, 
                           fmt::format("Reduce triangles to under {}", config.maxTrianglesPerMesh));
    }
    
    // Check vertex count
    if (mesh.vertices.size() > config.maxVerticesPerMesh) {
        issues.emplace_back(Severity::ERROR, "geometry", 
                           fmt::format("Vertex count ({}) exceeds limit ({})", 
                                     mesh.vertices.size(), config.maxVerticesPerMesh),
                           mesh.name, 
                           fmt::format("Reduce vertices to under {}", config.maxVerticesPerMesh));
    }
    
    // Warn about very low poly meshes
    if (mesh.triangleCount < 10 && mesh.triangleCount > 0) {
        issues.emplace_back(Severity::INFO, "geometry", 
                           fmt::format("Very low triangle count: {}", mesh.triangleCount),
                           mesh.name, "Consider if this mesh is necessary");
    }
}

void Validator::validateUVCoordinates(const MeshData& mesh, std::vector<ValidationIssue>& issues) {
    if (mesh.uvCoords.empty()) {
        issues.emplace_back(Severity::WARNING, "uv", 
                           "Mesh has no UV coordinates",
                           mesh.name, "Add UV coordinates for texturing");
        return;
    }
    
    if (config.checkUVRange) {
        bool validRange = checkUVRange(mesh.uvCoords);
        if (!validRange) {
            issues.emplace_back(Severity::WARNING, "uv", 
                               "UV coordinates outside 0-1 range detected",
                               mesh.name, "Ensure UV coordinates are within 0-1 range");
        }
    }