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
    if (name != "pvp����װ��" && name != "pve����װ��")system(string("xlsReader.exe " + name).c_str());

    ifstream file;
    file.open(string("py/" + name + "-����.csv").c_str(), std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        throw("δ��ȷ�����ļ�" + string("py/" + name + "-����.csv") + "����ȷ�������ļ������� " + name + ".xlsx ��񣬲�����Ӧ.xlsx��csvû�б�����ռ��");
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
    
    file.open(string("py/" + name + "-����.csv").c_str(), std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        throw("δ��ȷ�����ļ�" + string("py/" + name + "-����.csv") + "����ȷ�������ļ������� " + name + ".xlsx ��񣬲�����Ӧ.xlsx��csvû�б�����ռ��");
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

    file.open(string("py/" + name + "-����.csv").c_str(), std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        throw("δ��ȷ�����ļ�" + string("py/" + name + "-����.csv") + "����ȷ�������ļ������� " + name + ".xlsx ��񣬲�����Ӧ.xlsx��csvû�б�����ռ��");
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
        infile.open("��ս����.txt", ios::in);
        string atker, defer, pvper = "pvp����װ��", pveer = "pve����װ��";
        infile >> atker; atker = atker.substr(8, atker.size() - 8);
        infile >> defer; defer = defer.substr(8, defer.size() - 8);
        cout << "�����ý�������" << atker << "�����ط���" << defer << endl;
        cout << "��ʼ��ȡ�����ļ�(Լ10--30��)..." << endl;

        vector<vector<string>> tool_table1, name_table1, pet_table1, tool_table2, name_table2, pet_table2, tool_table3, name_table3, pet_table3, tool_table4, name_table4, pet_table4;
        ReadFile(atker, tool_table1, name_table1, pet_table1);
        ReadFile(defer, tool_table2, name_table2, pet_table2);
        ReadFile(pvper, tool_table3, name_table3, pet_table3);
        ReadFile(pveer, tool_table4, name_table4, pet_table4);
        cout << "�����ļ���ȡ���" << endl;
        cout << "============================================================================" << endl << endl;
        cout << "        �������ģ�⣺" << endl;
        long long total = 0;
        for (int i = 0; i < 10000; i++) {
            Person a(atker, tool_table1, name_table1, pet_table1);
            Person b(pvper, tool_table3, name_table3, pet_table3);
            total += (1000000000 - War(a, b).second.first);
        }
        cout << "for PVP, " << atker << "7�γ������������" << total/10000 << endl;
        total = 0;
        for (int i = 0; i < 10000; i++) {
            Person a(atker, tool_table1, name_table1, pet_table1);
            Person b(pveer, tool_table4, name_table4, pet_table4);
            total += (1000000000 - War(a, b).second.second);
        }
        cout << "for PVE, " << atker << "50�γ������������" << total / 10000 << endl;
        cout << endl;
        cout << "============================================================================" << endl << endl;
        //outfile.open("ս�����.txt", ios::out | ios::ate);

        cout << "        1000��ģ����ͳ�ƣ�" << endl;
        int def_cnt = 0;
        for (int i = 0; i < 1000; i++) {
            Person a(atker, tool_table1, name_table1, pet_table1);
            Person b(defer, tool_table2, name_table2, pet_table2);
            def_cnt += War(a, b).first;

        }
        cout << atker << "ʤ�ʣ�" << float(1000 - def_cnt) / 10 << "%" << endl << endl;
        cout << endl;
        cout << "============================================================================" << endl << endl;
        cout << "        1��ģ����ϸ������ʾ��" << endl;
        view = 1;
        Person a(atker, tool_table1, name_table1, pet_table1);
        Person b(defer, tool_table2, name_table2, pet_table2);
        def_cnt += War(a, b).first;

        Sleep(1000 * 60 * 10);
    }
    catch (int ErrorCode) {
        cout << "������" << endl;
    }
    catch (string ErrorCode) {
        cout << ErrorCode << endl;
    }
    return 0;
}
