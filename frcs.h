#ifndef __FRCS__
#define __FRCS__
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <vector>
using namespace std;
#define 敌人 主人->敌方
#define ATTRSIZE 13
#define 攻 1
#define 防 2
#define 血 3
#define 闪 4
#define 速 5
#define 暴 6
#define 金 7
#define 木 8
#define 水 9
#define 火 10
#define 土 11
#define 雷 12
#define 晕 13

#define 攻击 属性(攻)
#define 防御 属性(防)
#define 血量 属性(血)
#define 闪避 属性(闪)
#define 速度 属性(速)
#define 暴击 属性(暴)
#define 金抗 属性(金)
#define 木抗 属性(木)
#define 水抗 属性(水)
#define 火抗 属性(火)
#define 土抗 属性(土)
#define 雷抗 属性(雷)

bool Rand(int p);

struct Buff {
	int 类型;
	int 数值;
	int 回合;
};

class Person; class Unit; class Pet;
class Person {
public:
	Person* 敌方;
	Pet* 妖宠;
	string 名称;
	int 面板[ATTRSIZE];
	vector<Buff> 状态;
	int 属性(int 类型);
	int 生命;
	int 上次承伤;
	vector<Unit> 道具;
	Person(string name, vector<vector<string>>& tool_table, vector<vector<string>>& name_table, vector<vector<string>>& pet_table);
	void AddBuff(int type, int amount, int round);
	void 平A();
	pair<int, int> Attacked(int physic_harm, string attr = "", int attr_harm = 0, int attr_thrust = 0);
	int AttackedByPet(int harm);
	void Round();
};
class Unit {
public:
	string 形制;
	string 名称;
	string 五行;
	int 触发次数;
	int 触发概率;
	float 伤害系数;
	int 伤害常数;
	float 伤害提升;
	string 特效类型;
	float 特效参数;
	Person* 主人;
	Unit(Person* owner, vector<vector<string>>& tool_table, int line);
	void 使用();
};
class Pet {
public:
	Person* 主人;
	string 名称;
	int 面板[7];
	int 属性(int 类型);
	Pet(Person* owner, vector<vector<string>>& pet_table);
	void Round();
};
bool War(Person& 进攻者, Person& 防御者);
#endif