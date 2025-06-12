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
        aiProcess_Triangulate | aiProcess_FlipUVs);
    
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        info.errorMessage = importer.GetErrorString();
        return info;
    }
    
    info.meshCount = scene->mNumMeshes;
    info.materialCount = scene->mNumMaterials;
    info.isValid = true;
    
    return info;
}