#include "ScoreManager.h"

#include <fstream>
#include <algorithm>
#include <ctime>
#include <sstream>

ScoreManager::ScoreManager(const std::string& filename)
    : filename(filename)
{
    load();
}

void ScoreManager::load() {
    records.clear();

    std::ifstream file(filename);
    if (!file.is_open())
        return;

    std::string line;
    while (std::getline(file, line)) {
        ScoreRecord r;

        size_t p = line.find(']');
        if (p == std::string::npos)
            continue;

        r.datetime = line.substr(1, p - 1);

        std::sscanf(
            line.c_str() + p,
            "] Score: %d | Lines: %d | Level: %d",
            &r.score, &r.lines, &r.level
        );

        records.push_back(r);
    }
}

void ScoreManager::addRecord(int score, int lines, int level) {
    std::time_t now = std::time(nullptr);
    char buffer[100];
    std::strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", std::localtime(&now));

    records.push_back({
        score,
        lines,
        level,
        buffer
    });

    sortAndTrim();
    save();
}

void ScoreManager::sortAndTrim() {
    std::sort(records.begin(), records.end(),
        [](const ScoreRecord& a, const ScoreRecord& b) {
            return a.score > b.score;
        });

    if (records.size() > 10)
        records.resize(10);
}

void ScoreManager::save() {
    std::ofstream file(filename, std::ios::trunc);

    for (const auto& r : records) {
        file << "[" << r.datetime << "] "
             << "Score: " << r.score
             << " | Lines: " << r.lines
             << " | Level: " << r.level
             << "\n";
    }
}

std::vector<ScoreRecord> ScoreManager::getTopScores() const {
    return records;
}
