#pragma once
#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>

// Forward declarations
struct aiMesh;
struct aiScene;

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

struct MeshData {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::string name;
};

struct AssetInfo {
    std::string filename;
    int meshCount = 0;
    int materialCount = 0;
    int totalVerticesCount = 0;
    bool isValid = false;
    std::string errorMessage;
    
    std::vector<MeshData> meshes;
    
    // Default constructor
    AssetInfo() = default;
    
    // Move constructor and assignment for efficiency
    AssetInfo(AssetInfo&&) = default;
    AssetInfo& operator=(AssetInfo&&) = default;
    
    // Delete copy constructor and assignment to avoid accidental copies
    AssetInfo(const AssetInfo&) = delete;
    AssetInfo& operator=(const AssetInfo&) = delete;
};

class AssetLoader {
public:
    AssetInfo loadAsset(const std::string& filepath);
    
private:
    void processNode(const aiScene* scene, MeshData& meshData, const aiMesh* mesh);
};