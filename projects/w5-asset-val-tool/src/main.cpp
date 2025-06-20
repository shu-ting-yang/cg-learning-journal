#include <iostream>
#include <string>
#include <filesystem>
#include <fmt/core.h>
#include "AssetLoader.h"
#include "Validator.h"
#include "Reporter.h"
#include "ValidationConfig.h"

void printUsage(const char* programName) {
    fmt::print("Usage: {} [OPTIONS] <input_file>\n", programName);
    fmt::print("\nOptions:\n");
    fmt::print("  -c, --config <file>     Use custom validation config file\n");
    fmt::print("  -o, --output <file>     Output report file (default: report.json)\n");
    fmt::print("  -f, --format <format>   Report format: json, html, console (default: json)\n");
    fmt::print("  -v, --verbose           Enable verbose output\n");
    fmt::print("  --no-stats             Disable statistics in report\n");
    fmt::print("  --no-suggestions       Disable suggestions in report\n");
    fmt::print("  -h, --help             Show this help message\n");
    fmt::print("\nExamples:\n");
    fmt::print("  {} model.gltf\n", programName);
    fmt::print("  {} -c custom_config.json -o validation_report.json model.gltf\n", programName);
    fmt::print("  {} -f html -o report.html model.gltf\n", programName);
    fmt::print("  {} -f console model.gltf\n", programName);
}

ReportFormat parseReportFormat(const std::string& formatStr) {
    if (formatStr == "json") return ReportFormat::JSON;
    if (formatStr == "html") return ReportFormat::HTML;
    if (formatStr == "console") return ReportFormat::CONSOLE;
    
    fmt::print(stderr, "Unknown report format '{}', using JSON\n", formatStr);
    return ReportFormat::JSON;
}

int main(int argc, char* argv[]) {
    // Command line arguments
    std::string inputFile;
    std::string configFile = "validation_config.json";
    std::string outputFile = "report.json";
    ReportFormat reportFormat = ReportFormat::JSON;
    bool verbose = false;
    bool includeStats = true;
    bool includeSuggestions = true;
    
    // Parse command line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            printUsage(argv[0]);
            return 0;
        }
        else if (arg == "-v" || arg == "--verbose") {
            verbose = true;
        }
        else if (arg == "--no-stats") {
            includeStats = false;
        }
        else if (arg == "--no-suggestions") {
            includeSuggestions = false;
        }
        else if ((arg == "-c" || arg == "--config") && i + 1 < argc) {
            configFile = argv[++i];
        }
        else if ((arg == "-o" || arg == "--output") && i + 1 < argc) {
            outputFile = argv[++i];
        }
        else if ((arg == "-f" || arg == "--format") && i + 1 < argc) {
            reportFormat = parseReportFormat(argv[++i]);
        }
        else if (arg[0] != '-') {
            inputFile = arg;
        }
        else {
            fmt::print(stderr, "Unknown option: {}\n", arg);
            printUsage(argv[0]);
            return 1;
        }
    }
    
    // Validate input
    if (inputFile.empty()) {
        fmt::print(stderr, "Error: No input file specified\n");
        printUsage(argv[0]);
        return 1;
    }
    
    if (!std::filesystem::exists(inputFile)) {
        fmt::print(stderr, "Error: Input file '{}' does not exist\n", inputFile);
        return 1;
    }
    
    try {
        // Load configuration
        ValidationConfig config;
        if (std::filesystem::exists(configFile)) {
            if (verbose) {
                fmt::print("Loading configuration from: {}\n", configFile);
            }
            config = ValidationConfig::loadFromFile(configFile);
        } else {
            if (verbose) {
                fmt::print("Using default configuration (config file '{}' not found)\n", configFile);
            }
        }
        
        // Initialize components
        AssetLoader loader;
        loader.setVerboseLogging(verbose);
        
        Validator validator(config);
        
        Reporter reporter;
        reporter.setVerboseOutput(verbose);
        reporter.setIncludeStatistics(includeStats);
        reporter.setIncludeSuggestions(includeSuggestions);
        
        // Load asset
        if (verbose) {
            fmt::print("Loading asset: {}\n", inputFile);
        }
        
        SceneData sceneData;
        if (!loader.loadGLTF(inputFile, sceneData)) {
            fmt::print(stderr, "Error loading GLTF file: {}\n", loader.getLastError());
            return 1;
        }
        
        if (verbose) {
            fmt::print("Loaded {} meshes, {} materials, {} textures\n", 
                      sceneData.meshes.size(), 
                      sceneData.materials.size(), 
                      sceneData.textures.size());
        }
        
        // Validate asset
        if (verbose) {
            fmt::print("Validating asset...\n");
        }
        
        ValidationResult result = validator.validate(sceneData);
        
        // Generate report
        bool reportSuccess = false;
        
        if (reportFormat == ReportFormat::CONSOLE) {
            reportSuccess = reporter.generateConsoleReport(result);
        } else {
            if (verbose) {
                fmt::print("Generating report: {}\n", outputFile);
            }
            reportSuccess = reporter.generateReport(result, outputFile, reportFormat);
        }
        
        if (!reportSuccess) {
            fmt::print(stderr, "Error generating report: {}\n", reporter.getLastError());
            return 1;
        }
        
        // Print summary
        uint32_t totalIssues = result.globalIssues.size();
        for (const auto& meshResult : result.meshResults) {
            totalIssues += meshResult.issues.size();
        }
        for (const auto& materialResult : result.materialResults) {
            totalIssues += materialResult.issues.size();
        }
        
        if (result.isValid) {
            fmt::print("✓ Validation PASSED");
            if (totalIssues > 0) {
                fmt::print(" with {} warnings/info messages", totalIssues);
            }
            fmt::print("\n");
        } else {
            fmt::print("✗ Validation FAILED with {} issues\n", totalIssues);
        }
        
        if (reportFormat != ReportFormat::CONSOLE) {
            fmt::print("Report saved to: {}\n", outputFile);
        }
        
        return result.isValid ? 0 : 1;
        
    } catch (const std::exception& e) {
        fmt::print(stderr, "Fatal error: {}\n", e.what());
        return 1;
    }
}