#pragma once

#include <ctime>
#include <algorithm>
#include <vector>
#include <sstream>

struct ScoreRecord {
    int score;
    int lines;
    int level;
    std::string datetime;
};

class ScoreManager {
public:
    explicit ScoreManager(const std::string& filename = "tetris_records.txt");

    void addRecord(int score, int lines, int level);
    std::vector<ScoreRecord> getTopScores() const;

private:
    std::string filename;
    std::vector<ScoreRecord> records;

    void load();
    void save();
    void sortAndTrim();
};