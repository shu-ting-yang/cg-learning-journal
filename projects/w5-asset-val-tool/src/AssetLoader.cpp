#include "AssetLoader.h"
#include <fmt/core.h>
#include <filesystem>
#include <fstream>

AssetLoader::AssetLoader() : verboseLogging(false) {
    // Configure Assimp importer
    importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, 
        aiPrimitiveType_POINT | aiPrimitiveType_LINE);
}

AssetLoader::~AssetLoader() = default;

bool AssetLoader::loadGLTF(const std::string& filePath, SceneData& sceneData) {
    lastError.clear();
    sceneData = SceneData(); // Reset scene data
    sceneData.filePath = filePath;
    
    if (!fileExists(filePath)) {
        lastError = "File does not exist: " + filePath;
        return false;
    }
    
    logInfo("Loading GLTF file: " + filePath);
    
    // Load scene with Assimp
    const aiScene* scene = importer.ReadFile(filePath, 
        aiProcess_Triangulate |
        aiProcess_FlipUVs |
        aiProcess_CalcTangentSpace |
        aiProcess_GenSmoothNormals |
        aiProcess_JoinIdenticalVertices |
        aiProcess_ValidateDataStructure |
        aiProcess_ImproveCacheLocality |
        aiProcess_RemoveRedundantMaterials |
        aiProcess_SortByPType);
    
    if (!scene) {
        lastError = "Assimp error: " + std::string(importer.GetErrorString());
        return false;
    }
    
    if (!scene->mRootNode) {
        lastError = "Scene has no root node";
        return false;
    }
    
    logInfo(fmt::format("Scene loaded: {} meshes, {} materials", 
                       scene->mNumMeshes, scene->mNumMaterials));
    
    try {
        // Process materials first
        processMaterials(scene, sceneData);
        
        // Process textures
        processTextures(scene, sceneData, getDirectoryPath(filePath));
        
        // Process scene hierarchy
        processNode(scene->mRootNode, scene, sceneData);
        
        // Calculate totals
        sceneData.totalVertices = 0;
        sceneData.totalTriangles = 0;
        for (const auto& mesh : sceneData.meshes) {
            sceneData.totalVertices += mesh.vertices.size();
            sceneData.totalTriangles += mesh.triangleCount;
        }
        
        logInfo(fmt::format("Processing complete: {} vertices, {} triangles", 
                           sceneData.totalVertices, sceneData.totalTriangles));
        
        return true;
        
    } catch (const std::exception& e) {
        lastError = "Processing error: " + std::string(e.what());
        return false;
    }
}

void AssetLoader::processNode(aiNode* node, const aiScene* scene, SceneData& sceneData) {
    // Process all meshes in this node
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh, scene, sceneData);
    }
    
    // Process child nodes recursively
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene, sceneData);
    }
}

void AssetLoader::processMesh(aiMesh* mesh, const aiScene* scene, SceneData& sceneData) {
    MeshData meshData;
    meshData.name = mesh->mName.C_Str();
    
    if (meshData.name.empty()) {
        meshData.name = "mesh_" + std::to_string(sceneData.meshes.size());
    }
    
    logInfo("Processing mesh: " + meshData.name);
    
    // Extract vertices
    meshData.vertices.reserve(mesh->mNumVertices);
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        glm::vec3 vertex;
        vertex.x = mesh->mVertices[i].x;
        vertex.y = mesh->mVertices[i].y;
        vertex.z = mesh->mVertices[i].z;
        meshData.vertices.push_back(vertex);
    }
    
    // Extract normals
    if (mesh->HasNormals()) {
        meshData.normals.reserve(mesh->mNumVertices);
        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            glm::vec3 normal;
            normal.x = mesh->mNormals[i].x;
            normal.y = mesh->mNormals[i].y;
            normal.z = mesh->mNormals[i].z;
            meshData.normals.push_back(normal);
        }
    }
    
    // Extract UV coordinates (first channel only)
    if (mesh->HasTextureCoords(0)) {
        meshData.uvCoords.reserve(mesh->mNumVertices);
        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            glm::vec2 uv;
            uv.x = mesh->mTextureCoords[0][i].x;
            uv.y = mesh->mTextureCoords[0][i].y;
            meshData.uvCoords.push_back(uv);
        }
    }
    
    // Extract indices
    meshData.triangleCount = 0;
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        if (face.mNumIndices == 3) { // Triangles only
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                meshData.indices.push_back(face.mIndices[j]);
            }
            meshData.triangleCount++;
        }
    }
    
    // Store material index
    if (mesh->mMaterialIndex < sceneData.materials.size()) {
        meshData.materialIndices.push_back(mesh->mMaterialIndex);
    }
    
    sceneData.meshes.push_back(std::move(meshData));
}

void AssetLoader::processMaterials(const aiScene* scene, SceneData& sceneData) {
    sceneData.materials.reserve(scene->mNumMaterials);
    
    for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
        aiMaterial* material = scene->mMaterials[i];
        MaterialData matData;
        
        // Get material name
        aiString name;
        if (material->Get(AI_MATKEY_NAME, name) == AI_SUCCESS) {
            matData.name = name.C_Str();
        } else {
            matData.name = "material_" + std::to_string(i);
        }
        
        // Get material properties
        aiColor3D color;
        if (material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS) {
            matData.albedo = glm::vec3(color.r, color.g, color.b);
        }
        
        float metallic = 0.0f;
        material->Get(AI_MATKEY_METALLIC_FACTOR, metallic);
        matData.metallic = metallic;
        
        float roughness = 1.0f;
        material->Get(AI_MATKEY_ROUGHNESS_FACTOR, roughness);
        matData.roughness = roughness;
        
        // Get texture paths
        aiString texPath;
        if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texPath) == AI_SUCCESS) {
            matData.albedoTexture = texPath.C_Str();
        }
        if (material->GetTexture(aiTextureType_NORMALS, 0, &texPath) == AI_SUCCESS) {
            matData.normalTexture = texPath.C_Str();
        }
        if (material->GetTexture(aiTextureType_METALNESS, 0, &texPath) == AI_SUCCESS) {
            matData.metallicRoughnessTexture = texPath.C_Str();
        }
        if (material->GetTexture(aiTextureType_EMISSIVE, 0, &texPath) == AI_SUCCESS) {
            matData.emissiveTexture = texPath.C_Str();
        }
        if (material->GetTexture(aiTextureType_AMBIENT_OCCLUSION, 0, &texPath) == AI_SUCCESS) {
            matData.occlusionTexture = texPath.C_Str();
        }
        
        logInfo("Processed material: " + matData.name);
        sceneData.materials.push_back(std::move(matData));
    }
}

void AssetLoader::processTextures(const aiScene* scene, SceneData& sceneData, const std::string& basePath) {
    std::vector<std::string> textureFiles;
    
    // Collect all texture file paths from materials
    for (const auto& material : sceneData.materials) {
        if (!material.albedoTexture.empty()) {
            textureFiles.push_back(material.albedoTexture);
        }
        if (!material.normalTexture.empty()) {
            textureFiles.push_back(material.normalTexture);
        }
        if (!material.metallicRoughnessTexture.empty()) {
            textureFiles.push_back(material.metallicRoughnessTexture);
        }
        if (!material.emissiveTexture.empty()) {
            textureFiles.push_back(material.emissiveTexture);
        }
        if (!material.occlusionTexture.empty()) {
            textureFiles.push_back(material.occlusionTexture);
        }
    }
    
    // Remove duplicates
    std::sort(textureFiles.begin(), textureFiles.end());
    textureFiles.erase(std::unique(textureFiles.begin(), textureFiles.end()), textureFiles.end());
    
    // Process each unique texture
    for (const auto& texFile : textureFiles) {
        TextureData texData;
        texData.name = extractFileName(texFile);
        texData.filePath = texFile;
        texData.format = extractFileExtension(texFile);
        
        // Try to analyze texture file
        std::string fullPath = basePath + "/" + texFile;
        if (fileExists(fullPath)) {
            analyzeTexture(fullPath, texData);
        } else {
            logWarning("Texture file not found: " + fullPath);
        }
        
        sceneData.textures.push_back(std::move(texData));
    }
}

std::string AssetLoader::extractFileName(const std::string& filePath) {
    std::filesystem::path path(filePath);
    return path.filename().string();
}

std::string AssetLoader::extractFileExtension(const std::string& filePath) {
    std::filesystem::path path(filePath);
    std::string ext = path.extension().string();
    if (!ext.empty() && ext[0] == '.') {
        ext = ext.substr(1);
    }
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return ext;
}

std::string AssetLoader::getDirectoryPath(const std::string& filePath) {
    std::filesystem::path path(filePath);
    return path.parent_path().string();
}

bool AssetLoader::fileExists(const std::string& filePath) {
    return std::filesystem::exists(filePath);
}

bool AssetLoader::analyzeTexture(const std::string& texturePath, TextureData& textureData) {
    // Simple analysis - in a real implementation, you'd use image loading library
    // For now, we'll just check file size and set some defaults
    try {
        auto fileSize = std::filesystem::file_size(texturePath);
        
        // Estimate dimensions based on file size (very rough)
        if (fileSize > 4 * 1024 * 1024) { // > 4MB
            textureData.width = 2048;
            textureData.height = 2048;
        } else if (fileSize > 1024 * 1024) { // > 1MB
            textureData.width = 1024;
            textureData.height = 1024;
        } else if (fileSize > 256 * 1024) { // > 256KB
            textureData.width = 512;
            textureData.height = 512;
        } else {
            textureData.width = 256;
            textureData.height = 256;
        }
        
        textureData.channels = 4; // Assume RGBA
        return true;
        
    } catch (const std::exception& e) {
        logWarning("Failed to analyze texture: " + std::string(e.what()));
        return false;
    }
}

void AssetLoader::logInfo(const std::string& message) {
    if (verboseLogging) {
        fmt::print("[INFO] {}\n", message);
    }
}

void AssetLoader::logWarning(const std::string& message) {
    if (verboseLogging) {
        fmt::print("[WARNING] {}\n", message);
    }
}

void AssetLoader::logError(const std::string& message) {
    fmt::print(stderr, "[ERROR] {}\n", message);
}