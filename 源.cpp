#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>
#include <windows.h>
#include "frcs.h"
using namespace std;
extern bool view;
extern fstream outfile;
fstream infile;
void Print(const vector<vector<string>>& data) {
    // Find the maximum length for each column
    vector<size_t> maxColumnLength(data[0].size(), 0);
    for (const auto& row : data) {
        for (size_t i = 0; i < row.size(); ++i) {
            maxColumnLength[i] = max(maxColumnLength[i], row[i].length());
        }
    }

    // Print the data with left alignment
    for (const auto& row : data) {
        for (size_t i = 0; i < row.size(); ++i) {
            std::cout << row[i];
            // Calculate the number of spaces to add for alignment
            size_t spacesToAdd = maxColumnLength[i] - row[i].length() + 1;
            for (size_t j = 0; j < spacesToAdd; ++j) {
                std::cout << ' ';
            }
        }
        std::cout << "\n";
    }
}
void ReadFile(string name, vector<vector<string>>& tool_table, vector<vector<string>>& attr_table, vector<vector<string>>& pet_table) {
    bool print = 0;
    tool_table.clear(); attr_table.clear();
    system(string("xlsReader.exe " + name).c_str());

    ifstream file;
    file.open(string("py/" + name + "-道具.csv").c_str(), std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        throw("未正确生成文件" + string("py/" + name + "-道具.csv") + "，请确保道友文件夹中有 " + name + ".xlsx 表格，并且相应.xlsx和csv没有被进程占用");
    }
    string line;
    while (getline(file, line)) {
        /*
        int left = 0;
        vector<string> values;
        while (1) {
            for (int i = left; i < line.size(); i++) {
                if (line[i] == ',') {
                    values.push_back(line.substr(left, i - left));
                    left = i + 1;
                    continue;
                }
            }
            values.push_back(line.substr(left, line.size() - left));
            break;
        }
        */
        stringstream linestream(line);
        string value;
        vector<string> values;
        while (getline(linestream, value, ',')) {
            if (value.size() > 0 && int(value[value.size() - 1]) == 13)value = value.substr(0, value.size() - 1);
            values.push_back(value);
        }
        tool_table.push_back(values);
    }
    file.close();
    if (print) {
        Print(tool_table);
    }
    
    file.open(string("py/" + name + "-属性.csv").c_str(), std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        throw("未正确生成文件" + string("py/" + name + "-属性.csv") + "，请确保道友文件夹中有 " + name + ".xlsx 表格，并且相应.xlsx和csv没有被进程占用");
    }
    while (getline(file, line)) {
        stringstream linestream(line);
        string value;
        vector<string> values;
        while (getline(linestream, value, ',')) {
            if (value.size() > 0 && int(value[value.size() - 1]) == 13)value = value.substr(0, value.size() - 1);
            values.push_back(value);
        }
        attr_table.push_back(values);
    }
    file.close();
    if (print) {
        Print(attr_table);
    }

    file.open(string("py/" + name + "-妖宠.csv").c_str(), std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        throw("未正确生成文件" + string("py/" + name + "-妖宠.csv") + "，请确保道友文件夹中有 " + name + ".xlsx 表格，并且相应.xlsx和csv没有被进程占用");
    }
    while (getline(file, line)) {
        stringstream linestream(line);
        string value;
        vector<string> values;
        while (getline(linestream, value, ',')) {
            if (value.size() > 0 && int(value[value.size() - 1]) == 13)value = value.substr(0, value.size() - 1);
            values.push_back(value);
        }
        pet_table.push_back(values);
    }
    file.close();
    if (print) {
        Print(pet_table);
    }
}
int main() {
    srand(time(0));

    try {
        infile.open("参战道友.txt", ios::in);
        string atker, defer;
        infile >> atker; atker = atker.substr(8, atker.size() - 8);
        infile >> defer; defer = defer.substr(8, defer.size() - 8);
        cout << "已设置进攻方：" << atker << "，防守方：" << defer << endl;
        cout << "开始读取配置文件(约10秒)..." << endl;

        vector<vector<string>> tool_table1, name_table1, pet_table1, tool_table2, name_table2, pet_table2;
        ReadFile(atker, tool_table1, name_table1, pet_table1);
        ReadFile(defer, tool_table2, name_table2, pet_table2);
        cout << "配置文件读取完成" << endl;
        cout << "============================================================================" << endl << endl;
        //outfile.open("战斗结果.txt", ios::out | ios::ate);

        cout << "        1000次模拟结果统计：" << endl;
        int def_cnt = 0;
        for (int i = 0; i < 1000; i++) {
            Person a(atker, tool_table1, name_table1, pet_table1);
            Person b(defer, tool_table2, name_table2, pet_table2);
            def_cnt += War(a, b);

        }
        cout << atker << "胜率：" << float(1000 - def_cnt) / 10 << "%" << endl << endl;
        cout << "        1次模拟详细过程显示：" << endl;
        view = 1;
        Person a(atker, tool_table1, name_table1, pet_table1);
        Person b(defer, tool_table2, name_table2, pet_table2);
        def_cnt += War(a, b);

        Sleep(1000 * 60 * 10);
    }
    catch (int ErrorCode) {
        cout << "请检查表格" << endl;
    }
    catch (string ErrorCode) {
        cout << ErrorCode << endl;
    }
    return 0;
}
