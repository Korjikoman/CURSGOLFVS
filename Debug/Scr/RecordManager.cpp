#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <limits>

class RecordManager {
private:
    std::vector<int> records;

public:
    RecordManager() {
        // ������������� �������� ��� 10 �������, ��������� �������� - �������������
        records.resize(10, std::numeric_limits<int>::max());
        loadRecords();
    }

    // �������� �������� �� �����
    void loadRecords() {
        std::ifstream file("assets/records/records.txt");
        if (file.is_open()) {
            std::string line;
            int level = 0;
            while (std::getline(file, line) && level < 10) {
                records[level] = std::stoi(line);
                level++;
            }
            file.close();
        }
    }

    // ���������� �������� � ����
    void saveRecords() {
        std::ofstream file("assets/records/records.txt");
        if (file.is_open()) {
            for (int i = 0; i < 10; i++) {
                file << records[i] << "\n";
            }
            file.close();
        }
    }

    // �������� ������ ��� ������������� ������
    int getRecord(int level) const {
        if (level >= 1 && level <= 10) {
            return records[level - 1];
        }
        return std::numeric_limits<int>::max();
    }

    // ���������� ����� ������ ��� ������������� ������
    void setRecord(int level, int strokes) {
        if (level >= 1 && level <= 10) {
            if (strokes < records[level - 1]) {
                records[level - 1] = strokes;
                saveRecords();  // �������������� ���� � ������ ���������
            }
        }
    }
};
