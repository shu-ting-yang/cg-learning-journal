#pragma once

#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct MeshData {
    std::string name;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvCoords;
    std::vector<uint32_t> indices;
    std::vector<uint32_t> materialIndices;
    uint32_t triangleCount;
    
    MeshData() : triangleCount(0) {}
};

struct MaterialData {
    std::string name;
    glm::vec3 albedo;
    float metallic;
    float roughness;
    std::string albedoTexture;
    std::string normalTexture;
    std::string metallicRoughnessTexture;
    std::string emissiveTexture;
    std::string occlusionTexture;
    
    MaterialData() : albedo(1.0f), metallic(0.0f), roughness(1.0f) {}
};

struct TextureData {
    std::string name;
    std::string filePath;
    uint32_t width;
    uint32_t height;
    uint32_t channels;
    std::string format;
    
    TextureData() : width(0), height(0), channels(0) {}
};

struct SceneData {
    std::string filePath;
    std::vector<MeshData> meshes;
    std::vector<MaterialData> materials;
    std::vector<TextureData> textures;
    
    // Scene statistics
    uint32_t totalVertices;
    uint32_t totalTriangles;
    
    SceneData() : totalVertices(0), totalTriangles(0) {}
};

class AssetLoader {
public:
    AssetLoader();
    ~AssetLoader();
    
    // Load GLTF file and extract all data
    bool loadGLTF(const std::string& filePath, SceneData& sceneData);
    
    // Get last error message
    const std::string& getLastError() const { return lastError; }
    
    // Enable/disable verbose logging
    void setVerboseLogging(bool enabled) { verboseLogging = enabled; }

private:
    Assimp::Importer importer;
    std::string lastError;
    bool verboseLogging;
    
    // Internal processing functions
    void processNode(aiNode* node, const aiScene* scene, SceneData& sceneData);
    void processMesh(aiMesh* mesh, const aiScene* scene, SceneData& sceneData);
    void processMaterials(const aiScene* scene, SceneData& sceneData);
    void processTextures(const aiScene* scene, SceneData& sceneData, const std::string& basePath);
    
    // Helper functions
    std::string extractFileName(const std::string& filePath);
    std::string extractFileExtension(const std::string& filePath);
    std::string getDirectoryPath(const std::string& filePath);
    bool fileExists(const std::string& filePath);
    
    // Texture analysis
    bool analyzeTexture(const std::string& texturePath, TextureData& textureData);
    
    // Logging
    void logInfo(const std::string& message);
    void logWarning(const std::string& message);
    void logError(const std::string& message);
};