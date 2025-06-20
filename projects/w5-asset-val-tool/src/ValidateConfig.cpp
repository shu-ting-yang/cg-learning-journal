#include "ValidationConfig.h"
#include <fstream>
#include <fmt/core.h>

ValidationConfig ValidationConfig::loadFromFile(const std::string& configPath) {
    ValidationConfig config;
    
    try {
        std::ifstream file(configPath);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open config file: " + configPath);
        }
        
        nlohmann::json j;
        file >> j;
        
        from_json(j, config);
        
    } catch (const std::exception& e) {
        fmt::print(stderr, "Warning: Failed to load config from '{}': {}\n", configPath, e.what());
        fmt::print(stderr, "Using default configuration\n");
    }
    
    return config;
}

void ValidationConfig::saveToFile(const std::string& configPath) const {
    try {
        std::ofstream file(configPath);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot create config file: " + configPath);
        }
        
        nlohmann::json j;
        to_json(j, *this);
        
        file << j.dump(4);
        
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to save config to '" + configPath + "': " + e.what());
    }
}

std::regex ValidationConfig::getMeshNameRegex() const {
    return std::regex(meshNamePattern);
}

std::regex ValidationConfig::getMaterialNameRegex() const {
    return std::regex(materialNamePattern);
}

std::regex ValidationConfig::getTextureNameRegex() const {
    return std::regex(textureNamePattern);
}

void to_json(nlohmann::json& j, const ValidationConfig& config) {
    j = nlohmann::json{
        {"polygon_limits", {
            {"max_triangles_per_mesh", config.maxTrianglesPerMesh},
            {"max_vertices_per_mesh", config.maxVerticesPerMesh},
            {"max_total_triangles", config.maxTotalTriangles}
        }},
        {"texture_limits", {
            {"max_texture_width", config.maxTextureWidth},
            {"max_texture_height", config.maxTextureHeight},
            {"require_power_of_two", config.requirePowerOfTwo},
            {"allowed_texture_formats", config.allowedTextureFormats}
        }},
        {"uv_validation", {
            {"check_uv_range", config.checkUVRange},
            {"check_uv_overlaps", config.checkUVOverlaps},
            {"uv_tolerance", config.uvTolerance}
        }},
        {"naming_conventions", {
            {"enforce_naming_conventions", config.enforceNamingConventions},
            {"mesh_name_pattern", config.meshNamePattern},
            {"material_name_pattern", config.materialNamePattern},
            {"texture_name_pattern", config.textureNamePattern}
        }},
        {"material_validation", {
            {"require_albedo_texture", config.requireAlbedoTexture},
            {"require_normal_texture", config.requireNormalTexture},
            {"require_metallic_roughness_texture", config.requireMetallicRoughnessTexture}
        }},
        {"file_validation", {
            {"check_file_exists", config.checkFileExists},
            {"validate_texture_files", config.validateTextureFiles}
        }},
        {"performance", {
            {"enable_detailed_logging", config.enableDetailedLogging},
            {"generate_suggestions", config.generateSuggestions}
        }}
    };
}

void from_json(const nlohmann::json& j, ValidationConfig& config) {
    // Polygon limits
    if (j.contains("polygon_limits")) {
        const auto& poly = j["polygon_limits"];
        if (poly.contains("max_triangles_per_mesh")) {
            config.maxTrianglesPerMesh = poly["max_triangles_per_mesh"];
        }
        if (poly.contains("max_vertices_per_mesh")) {
            config.maxVerticesPerMesh = poly["max_vertices_per_mesh"];
        }
        if (poly.contains("max_total_triangles")) {
            config.maxTotalTriangles = poly["max_total_triangles"];
        }
    }
    
    // Texture limits
    if (j.contains("texture_limits")) {
        const auto& tex = j["texture_limits"];
        if (tex.contains("max_texture_width")) {
            config.maxTextureWidth = tex["max_texture_width"];
        }
        if (tex.contains("max_texture_height")) {
            config.maxTextureHeight = tex["max_texture_height"];
        }
        if (tex.contains("require_power_of_two")) {
            config.requirePowerOfTwo = tex["require_power_of_two"];
        }
        if (tex.contains("allowed_texture_formats")) {
            config.allowedTextureFormats = tex["allowed_texture_formats"];
        }
    }
    
    // UV validation
    if (j.contains("uv_validation")) {
        const auto& uv = j["uv_validation"];
        if (uv.contains("check_uv_range")) {
            config.checkUVRange = uv["check_uv_range"];
        }
        if (uv.contains("check_uv_overlaps")) {
            config.checkUVOverlaps = uv["check_uv_overlaps"];
        }
        if (uv.contains("uv_tolerance")) {
            config.uvTolerance = uv["uv_tolerance"];
        }
    }
    
    // Naming conventions
    if (j.contains("naming_conventions")) {
        const auto& naming = j["naming_conventions"];
        if (naming.contains("enforce_naming_conventions")) {
            config.enforceNamingConventions = naming["enforce_naming_conventions"];
        }
        if (naming.contains("mesh_name_pattern")) {
            config.meshNamePattern = naming["mesh_name_pattern"];
        }
        if (naming.contains("material_name_pattern")) {
            config.materialNamePattern = naming["material_name_pattern"];
        }
        if (naming.contains("texture_name_pattern")) {
            config.textureNamePattern = naming["texture_name_pattern"];
        }
    }
    
    // Material validation
    if (j.contains("material_validation")) {
        const auto& mat = j["material_validation"];
        if (mat.contains("require_albedo_texture")) {
            config.requireAlbedoTexture = mat["require_albedo_texture"];
        }
        if (mat.contains("require_normal_texture")) {
            config.requireNormalTexture = mat["require_normal_texture"];
        }
        if (mat.contains("require_metallic_roughness_texture")) {
            config.requireMetallicRoughnessTexture = mat["require_metallic_roughness_texture"];
        }
    }
    
    // File validation
    if (j.contains("file_validation")) {
        const auto& file = j["file_validation"];
        if (file.contains("check_file_exists")) {
            config.checkFileExists = file["check_file_exists"];
        }
        if (file.contains("validate_texture_files")) {
            config.validateTextureFiles = file["validate_texture_files"];
        }
    }
    
    // Performance
    if (j.contains("performance")) {
        const auto& perf = j["performance"];
        if (perf.contains("enable_detailed_logging")) {
            config.enableDetailedLogging = perf["enable_detailed_logging"];
        }
        if (perf.contains("generate_suggestions")) {
            config.generateSuggestions = perf["generate_suggestions"];
        }
    }
}