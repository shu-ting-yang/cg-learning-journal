#include "AssetLoader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

AssetInfo AssetLoader::loadAsset(const std::string& filepath) {
    AssetInfo info;
    info.filename = filepath;
    
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filepath, 
        aiProcess_Triangulate | 
        aiProcess_FlipUVs | 
        aiProcess_GenNormals |
        aiProcess_CalcTangentSpace);
    
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        info.errorMessage = importer.GetErrorString();
        info.isValid = false;
        return info;
    }
    
    info.isValid = true;
    info.meshCount = scene->mNumMeshes;
    info.materialCount = scene->mNumMaterials;
    
    // Reserve space for meshes
    info.meshes.reserve(info.meshCount);
    
    // Process each mesh
    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        const aiMesh* mesh = scene->mMeshes[i];
        MeshData meshData;
        
        processNode(scene, meshData, mesh);
        
        info.totalVerticesCount += static_cast<int>(meshData.vertices.size());
        info.meshes.push_back(std::move(meshData));
    }
    
    return info;
}

void AssetLoader::processNode(const aiScene* scene, MeshData& meshData, const aiMesh* mesh) {
    // Set mesh name
    meshData.name = mesh->mName.C_Str();
    
    // Reserve space for vertices and indices
    meshData.vertices.reserve(mesh->mNumVertices);
    meshData.indices.reserve(mesh->mNumFaces * 3);
    
    // Process vertices
    for (unsigned int v = 0; v < mesh->mNumVertices; v++) {
        Vertex vertex;
        
        // Position
        vertex.position = glm::vec3(
            mesh->mVertices[v].x, 
            mesh->mVertices[v].y, 
            mesh->mVertices[v].z
        );
        
        // Normals
        if (mesh->HasNormals()) {
            vertex.normal = glm::vec3(
                mesh->mNormals[v].x, 
                mesh->mNormals[v].y, 
                mesh->mNormals[v].z
            );
        } else {
            vertex.normal = glm::vec3(0.0f, 1.0f, 0.0f); // Default up vector
        }
        
        // Texture coordinates
        if (mesh->HasTextureCoords(0)) {
            vertex.texCoords = glm::vec2(
                mesh->mTextureCoords[0][v].x, 
                mesh->mTextureCoords[0][v].y
            );
        } else {
            vertex.texCoords = glm::vec2(0.0f, 0.0f);
        }
        
        meshData.vertices.push_back(vertex);
    }
    
    // Process indices
    for (unsigned int f = 0; f < mesh->mNumFaces; f++) {
        const aiFace& face = mesh->mFaces[f];
        
        // Ensure the face is triangulated
        if (face.mNumIndices == 3) {
            for (unsigned int i = 0; i < face.mNumIndices; i++) {
                meshData.indices.push_back(face.mIndices[i]);
            }
        } else {
            std::cerr << "Warning: Non-triangular face found in mesh " 
                      << meshData.name << std::endl;
        }
    }
}