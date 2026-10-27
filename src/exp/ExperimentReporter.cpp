#include "ExperimentReporter.hpp"

ExperimentReporter::ExperimentReporter() {}

std::string ExperimentReporter::csvField(const std::string &raw) {
    if (raw.find_first_of(",\"\n\r") == std::string::npos) return raw;
    std::string escaped = "\"";
    for(char c : raw) {
        if(c == '"') escaped += "\"\"";
        else escaped += c;
    }
    return escaped + "\"";
}

void ExperimentReporter::generateReportCSV(const std::string &filepath, const std::vector<RunResult> &results) {
    std::ofstream out(filepath);
    if (!out.is_open())
        throw std::runtime_error("Useless path: " + filepath);

    out << std::setprecision(10);
    
    out << "label,timestamp,seed,n,k,x,t,K,ok,cost,seconds,error,solution\n";
            
    for (const RunResult &r : results) {
        const RunConfig &c = r.config;
        out << csvField(c.label) << ','
            << c.seed << ','
            << csvField(r.timestamp) << ','
            << c.params.n << ','
            << c.params.k << ','
            << c.params.x << ','
            << c.params.t << ','
            << c.params.K << ','
            << (r.ok ? "factible" : "no factible") << ','
            << r.cost << ',' 
            << std::fixed << std::setprecision(3) << r.seconds << std::defaultfloat << ','
            << csvField(r.error) << ','
            << csvField(r.solutionString) << '\n';                                           
    }           
}