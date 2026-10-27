#pragma once

#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <string>
#include <vector>

#include "ExperimentRunner.hpp"

class ExperimentReporter {
    private:
        /**
         * @brief Escapes and formats a string to be safely inserted into a CSV cell.
         * @param raw The raw string.
         * @return The CSV-safe string.
         */
        std::string csvField(const std::string &raw);
        
    public:
        ExperimentReporter();
    
        /**
         * @brief Generates a CSV file containing the metrics of all executed experiments.
         * @param filepath The destination path for the CSV file.
         * @param results The list of results to export.
         */
        void generateReportCSV(const std::string &filepath, const std::vector<RunResult> &results);
};