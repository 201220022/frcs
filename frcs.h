#ifndef __FRCS__
#define __FRCS__
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <vector>
using namespace std;
#define ���� ����->�з�
#define ATTRSIZE 13
#define �� 1
#define �� 2
#define Ѫ 3
#define �� 4
#define �� 5
#define �� 6
#define �� 7
#define ľ 8
#define ˮ 9
#define �� 10
#define �� 11
#define �� 12
#define �� 13

#define ���� ����(��)
#define ���� ����(��)
#define Ѫ�� ����(Ѫ)
#define ���� ����(��)
#define �ٶ� ����(��)
#define ���� ����(��)
#define �� ����(��)
#define ľ�� ����(ľ)
#define ˮ�� ����(ˮ)
#define �� ����(��)
#define ���� ����(��)
#define �׿� ����(��)

bool Rand(int p);

struct Buff {
	int ����;
	int ��ֵ;
	int �غ�;
};

class Person; class Unit; class Pet;
class Person {
public:
	Person* �з�;
	Pet* ����;
	string ����;
	int ���[ATTRSIZE];
	vector<Buff> ״̬;
	int ����(int ����);
	int ����;
	int �ϴγ���;
	vector<Unit> ����;
	Person(string name, vector<vector<string>>& tool_table, vector<vector<string>>& name_table, vector<vector<string>>& pet_table);
	void AddBuff(int type, int amount, int round);
	void ƽA();
	pair<int, int> Attacked(int physic_harm, string attr = "", int attr_harm = 0, int attr_thrust = 0);
	int AttackedByPet(int harm);
	void Round();
};
class Unit {
public:
	string ����;
	string ����;
	string ����;
	int ��������;
	int ��������;
	float �˺�ϵ��;
	int �˺�����;
	float �˺�����;
	string ��Ч����;
	float ��Ч����;
	Person* ����;
	Unit(Person* owner, vector<vector<string>>& tool_table, int line);
	void ʹ��();
};
class Pet {
public:
	Person* ����;
	string ����;
	int ���[7];
	int ����(int ����);
	Pet(Person* owner, vector<vector<string>>& pet_table);
	void Round();
};
bool War(Person& ������, Person& ������);
#endif