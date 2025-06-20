#pragma once

#include "ValidationResult.h"
#include <string>
#include <fstream>

enum class ReportFormat {
    JSON,
    HTML,
    CONSOLE
};

class Reporter {
public:
    Reporter();
    ~Reporter();
    
    // Generate reports in different formats
    bool generateJSONReport(const ValidationResult& result, const std::string& outputPath);
    bool generateHTMLReport(const ValidationResult& result, const std::string& outputPath);
    bool generateConsoleReport(const ValidationResult& result);
    
    // Generate report based on format
    bool generateReport(const ValidationResult& result, const std::string& outputPath, ReportFormat format);
    
    // Configuration
    void setVerboseOutput(bool verbose) { verboseOutput = verbose; }
    void setIncludeStatistics(bool include) { includeStatistics = include; }
    void setIncludeSuggestions(bool include) { includeSuggestions = include; }
    
    // Get last error
    const std::string& getLastError() const { return lastError; }

private:
    bool verboseOutput;
    bool includeStatistics;
    bool includeSuggestions;
    std::string lastError;
    
    // JSON report helpers
    nlohmann::json createJSONReport(const ValidationResult& result);
    void addStatisticsToJSON(nlohmann::json& report, const ValidationResult& result);
    void addIssueSummaryToJSON(nlohmann::json& report, const ValidationResult& result);
    
    // HTML report helpers
    std::string createHTMLReport(const ValidationResult& result);
    std::string generateHTMLHeader(const std::string& title);
    std::string generateHTMLSummary(const ValidationResult& result);
    std::string generateHTMLStatistics(const ValidationResult& result);
    std::string generateHTMLIssueTable(const std::vector<ValidationIssue>& issues, const std::string& title);
    std::string generateHTMLMeshDetails(const std::vector<MeshValidationResult>& meshResults);
    std::string generateHTMLMaterialDetails(const std::vector<MaterialValidationResult>& materialResults);
    std::string generateHTMLFooter();
    
    // Console report helpers
    void printConsoleSummary(const ValidationResult& result);
    void printConsoleStatistics(const ValidationResult& result);
    void printConsoleIssues(const std::vector<ValidationIssue>& issues, const std::string& title);
    void printConsoleMeshDetails(const std::vector<MeshValidationResult>& meshResults);
    void printConsoleMaterialDetails(const std::vector<MaterialValidationResult>& materialResults);
    
    // Utility functions
    std::string getSeverityColor(Severity severity);
    std::string getSeverityIcon(Severity severity);
    std::string formatFileSize(uint64_t bytes);
    std::string getCurrentTimestamp();
    
    // Issue summary helpers
    struct IssueSummary {
        uint32_t infoCount = 0;
        uint32_t warningCount = 0;
        uint32_t errorCount = 0;
        uint32_t criticalCount = 0;
        
        void addIssue(Severity severity);
        uint32_t getTotalCount() const;
    };
    
    IssueSummary calculateIssueSummary(const ValidationResult& result);
    
    // File I/O helpers
    bool writeToFile(const std::string& content, const std::string& filePath);
    std::string escapeHTML(const std::string& text);
};