#include "frcs.h"
bool view = 0;
fstream outfile;
bool Rand(int p) {
	if (p >= 100)return true;
	if (p <= 0)return false;
	float g = ((rand() % 8192) / 8192.0) * 100.0;
	return (float(p) >= g);
}
vector<string> 套装选择检查 = { "套装选择", "设置1", "设置2", "设置3", "设置4", "设置5" };
vector<string> 形制选择检查 = { "形制选择", "秘术","法术","法宝","本命法宝·剑刀枪","本命法宝·锤印塔","本命法宝·幡图镜","本命法宝·玉莲瓶" };
vector<string> 五行选择检查 = { "五行选择", "金","木","水","火","土","雷" };
vector<string> 特效选择检查 = { "特效选择", "", "秘术·天元", "秘术·加攻","秘术·加防","秘术·加血","秘术·加闪","秘术·加暴","秘术·眩晕","法宝·无视","灵核·青魔","灵核·青龙","灵核·墨魂","灵核·死狱","灵核·灭魂","灵核·白虹","灵核·太荒","灵核·九阳","灵核·天问","灵核·蚀天" };
//vector<string> 出战设置检查 = { "出战设置", "设置1", "设置2", "设置3", "设置4", "设置5" };
int GetInt(vector<vector<string>>& table, int row, int col) {
	if (row < 0)throw -1;
	if (col < 0)throw -1;
	if (table.size() <= row)throw -1;
	if (table[row].size() <= col)throw - 1;
	return atoi(table[row][col].c_str());
}
float GetFloat(vector<vector<string>>& table, int row, int col) {
	if (row < 0)throw -1;
	if (col < 0)throw -1;
	if (table.size() <= row)throw -1;
	if (table[row].size() <= col)throw -1;
	return float(atof(table[row][col].c_str()));
}
string GetString(vector<vector<string>>& table, int row, int col, vector<string> check = {}) {
	if (row < 0)throw -1;
	if (col < 0)throw -1;
	if (table.size() <= row)throw -1;
	if (table[row].size() <= col)throw -1;
	if (check.size() > 1) {
		bool found = 0;
		for (int i = 1; i < check.size(); i++) {
			if (table[row][col] == check[i]) found = 1;
		}
		if (found == 0) {
			string err = check[0] + "(当前：" + table[row][col] + ")必须是以下字符串中的一个：" + check[1];
			for (int i = 2; i < check.size(); i++)err += ("  " + check[i]);
			throw(err);
		}
	}
	return table[row][col];
}

Person::Person(string name, vector<vector<string>>& tool_table, vector<vector<string>>& attr_table, vector<vector<string>>& pet_table) {
	名称 = name;
	for (int i = 1; i < ATTRSIZE; i++)面板[i] = GetInt(attr_table, i - 1, 1);
	生命 = 血量;
	上次承伤 = 0;
	//for (int i = 1; i < ATTRSIZE; i++)cout << 面板[i] << " "; cout << endl;

	string setid_str = GetString(tool_table, 1, 0, 套装选择检查);
	int setid = atoi(setid_str.substr(4, 5).c_str());

	int cnt = 0;
	for (int i = 1; i < tool_table.size(); i++) {
		bool found = 0;
		for (int j = 1; j < tool_table.size(); j++) {
			int rank = GetInt(tool_table, j, setid);
			if (found == 0 && rank == i) {
				found = 1;
				Unit u(this, tool_table, j);
				道具.push_back(u);
			}
			else if (found == 1 && rank == i) {
				throw("存在重复的出战顺序");
			}
			else if (rank < 0 || rank >= tool_table.size()) {
				throw("异常的出战顺序");
			}
		}
	}

	Pet* p = new Pet(this, pet_table);
	妖宠 = p;
	if (p->名称 == "")妖宠 = NULL;
}
int Person::属性(int 类型) {
	int ret = 面板[类型];
	for (int i = 0; i < 状态.size(); i++) {
		if (状态[i].类型 == 类型) ret += 状态[i].数值;
	}
	return ret;
}
void Person::AddBuff(int type, int amount, int round) {
	if (type == 血 && amount > 0) {
		生命 = min(血量, 生命 + amount);
		return;
	}
	if (type == 血 && amount <= 0) {
		生命 = max(0, 生命 + amount);
		上次承伤 += (-amount);
		return;
	}
	Buff buff;
	buff.类型 = type;
	buff.数值 = amount;
	buff.回合 = round;
	状态.push_back(buff);
}
void Person::平A() {
	pair<int, int> 伤害结果 = 敌方->Attacked(攻击);
	string output = 名称 + "对" + 敌方->名称 + "造成 " + to_string(伤害结果.first) + " 点伤害。";
	if (view) cout << "    " << output << endl;
}
pair<int, int> Person::Attacked(int physic_harm, string attr, int attr_harm, int attr_thrust) {
	int attr_def =
		attr == "金" ? 金抗 :
		attr == "木" ? 木抗 :
		attr == "水" ? 水抗 :
		attr == "火" ? 火抗 :
		attr == "土" ? 土抗 :
		attr == "雷" ? 雷抗 : 0;
	attr_def = max(attr_def - attr_thrust, 0);
	physic_harm = max(0, physic_harm - 防御);
	physic_harm = physic_harm + rand() % 5 + 1;
	attr_harm = max(0, attr_harm - attr_def);
	int harm = physic_harm + attr_harm;
	生命 = max(0, 生命 - harm);
	上次承伤 = harm;
	return { harm, attr_harm };
}
int Person::AttackedByPet(int harm) {
	生命 = max(0, 生命 - harm);
	return harm;
}
void Person::Round() {
	//cout << "状态栏："; for (int i = 0; i < 状态.size(); i++)cout << 状态[i].类型 << " "; cout << endl;
	if (生命 == 0 || 敌方->生命 == 0)return;
	bool dizzy = 0;
	for (int i = 0; i < 状态.size(); i++) {
		if (状态[i].类型 == 晕) dizzy = 1;
	}
	vector<Buff> tmp1 = 状态; 状态.clear();
	for (int i = 0; i < tmp1.size(); i++) if (tmp1[i].类型 != 晕) 状态.push_back(tmp1[i]);

	bool 万氪大佬大爆发 = 0;
	for (int i = 0; i < 道具.size(); i++) {
		if (道具[i].特效类型 == "秘术·天元" && 道具[i].触发次数 > 0 && 生命 < 0.3 * 血量) {
			道具[i].使用();
			万氪大佬大爆发 = 1;
		}
	}
	if (!万氪大佬大爆发) {
		if (!dizzy) {
			int rand_index = rand() % 100;
			for (int i = 0; i < 道具.size() && rand_index >= 0; i++) {
				if (道具[i].触发次数 > 0) {
					rand_index -= 道具[i].触发概率;
					if (rand_index < 0) {
						道具[i].使用();
					}
				}
			}
			if (rand_index >= 0) { 平A(); }
		}
		else if (view) cout << "    " << 名称 << "处于昏迷状态。" << endl;
	}

	if (妖宠 != NULL)妖宠->Round();
	
	for (int i = 0; i < 状态.size(); i++) 状态[i].回合--;
	vector<Buff> tmp2 = 状态; 状态.clear();
	for (int i = 0; i < tmp2.size(); i++) if (tmp2[i].回合 > 0) 状态.push_back(tmp2[i]);
}

Unit::Unit(Person* owner, vector<vector<string>>& tool_table, int line) {
	//cout << "  "; for (int i = 1; i < tool_table[line].size(); i++)cout << tool_table[line][i] << " "; cout << endl;
	名称 = GetString(tool_table, line, 6);
	形制 = GetString(tool_table, line, 7, 形制选择检查);
	五行 = GetString(tool_table, line, 8, 五行选择检查);
	触发次数 = GetInt(tool_table, line, 9);
	触发概率 = int(GetFloat(tool_table, line, 10) * 100);
	伤害系数 = GetFloat(tool_table, line, 11);
	伤害常数 = GetInt(tool_table, line, 12);
	伤害提升 = GetFloat(tool_table, line, 13);
	特效类型 = GetString(tool_table, line, 15, 特效选择检查);
	特效参数 = GetFloat(tool_table, line, 16);
	if (特效类型.substr(0, 4) == "灵核" && 特效参数 > 1)特效参数 /= 100.0;
	主人 = owner;
}
void Unit::使用() {
	string output = 主人->名称 + "发动" + 名称 + "对" + 敌人->名称 + "造成 ";

	int 数值 = int((1.0 + 伤害提升) * (伤害系数 * 主人->攻击 + 伤害常数));
	int 无视 = 特效类型 == "法宝·无视" ? int(特效参数) : 0;
	pair<int,int> 伤害结果 = 主人->敌方->Attacked(主人->攻击, 五行, 数值, 无视);
	触发次数--;
	output = output + to_string(伤害结果.first) + " (" + 五行 + "伤+" + to_string(伤害结果.second) + ")点伤害。";
	if (特效类型 == "秘术·天元")主人->AddBuff(血, +0.2 * 主人->血量, 0);
	else if (特效类型 == "秘术·加攻") 主人->AddBuff(攻, +特效参数, 10);
	else if (特效类型 == "秘术·加防") 主人->AddBuff(防, +特效参数, 10);
	else if (特效类型 == "秘术·加血") 主人->AddBuff(血, +特效参数, 0);
	else if (特效类型 == "秘术·加闪") 主人->AddBuff(闪, +特效参数, 10);
	else if (特效类型 == "秘术·加暴") 主人->AddBuff(暴, +特效参数, 10);
	else if (特效类型 == "秘术·眩晕") 敌人->AddBuff(晕, +特效参数, 1);
	else if (特效类型 == "灵核·青魔") 主人->AddBuff(血, +特效参数 * 主人->攻击, 0);
	else if (特效类型 == "灵核·青龙") 敌人->AddBuff(攻, -特效参数 * 敌人->攻击, 2);
	else if (特效类型 == "灵核·墨魂") 敌人->AddBuff(血, -特效参数 * 敌人->血量, 0);
	else if (特效类型 == "灵核·死狱") 主人->AddBuff(血, +特效参数 * 主人->防御, 0);
	else if (特效类型 == "灵核·白虹") 敌人->AddBuff(血, -特效参数 * 主人->上次承伤, 0);
	else if (特效类型 == "灵核·太荒") 主人->AddBuff(攻, +特效参数 * 主人->攻击, 2);
	else if (特效类型 == "灵核·九阳");
	else if (特效类型 == "灵核·蚀天") 敌人->AddBuff(防, -特效参数 * 敌人->防御, 2);
	else if (特效类型 == "灵核·灭魂") {
		敌人->AddBuff(金, -特效参数 * 敌人->金抗, 2);
		敌人->AddBuff(木, -特效参数 * 敌人->木抗, 2);
		敌人->AddBuff(水, -特效参数 * 敌人->水抗, 2);
		敌人->AddBuff(火, -特效参数 * 敌人->火抗, 2);
		敌人->AddBuff(土, -特效参数 * 敌人->土抗, 2);
		敌人->AddBuff(雷, -特效参数 * 敌人->雷抗, 2);
	}
	else if (特效类型 == "灵核·天问") {
		主人->AddBuff(防, +特效参数 * 主人->防御, 2);
		主人->AddBuff(金, +特效参数 * 主人->金抗, 2);
		主人->AddBuff(木, +特效参数 * 主人->木抗, 2);
		主人->AddBuff(水, +特效参数 * 主人->水抗, 2);
		主人->AddBuff(火, +特效参数 * 主人->火抗, 2);
		主人->AddBuff(土, +特效参数 * 主人->土抗, 2);
		主人->AddBuff(雷, +特效参数 * 主人->雷抗, 2);
	}
	else;

	if (特效类型 == "秘术·天元")output += ("自身生命恢复" + to_string(0.2 * 主人->血量) + "点");
	else if (特效类型 == "秘术·加攻") output += ("自身攻击提高" + to_string(int(特效参数)) + "点，持续10回合。");
	else if (特效类型 == "秘术·加防") output += ("自身防御提高" + to_string(int(特效参数)) + "点，持续10回合。");
	else if (特效类型 == "秘术·加血") output += ("自身生命恢复" + to_string(int(特效参数)) + "点");
	else if (特效类型 == "秘术·加闪") output += ("自身闪避提高" + to_string(int(特效参数)) + "点，持续10回合。");
	else if (特效类型 == "秘术·加暴") output += ("自身暴击提高" + to_string(int(特效参数)) + "点，持续10回合。");
	else if (特效类型 == "秘术·眩晕") output += ("并眩晕对方1回合。");
	else if (特效类型 == "灵核·青魔") output += ("本命法宝效果发动，恢复自身生命" + to_string(int(特效参数 * 主人->攻击)) + "。");
	else if (特效类型 == "灵核·青龙") output += ("本命法宝效果发动，减少敌方攻击" + to_string(特效参数)) + "。";
	else if (特效类型 == "灵核·墨魂") output += ("本命法宝效果发动，额外附加伤害" + to_string(特效参数)) + "。";
	else if (特效类型 == "灵核·死狱") output += ("本命法宝效果发动，恢复自身生命" + to_string(int(特效参数 * 主人->防御)) + "。");
	else if (特效类型 == "灵核·白虹") output += ("本命法宝效果发动，额外附加伤害" + to_string(特效参数)) + "。";
	else if (特效类型 == "灵核·太荒") output += ("本命法宝效果发动，增加自身攻击" + to_string(特效参数)) + "。";
	else if (特效类型 == "灵核·九阳") output += ("本命法宝效果发动，额外附加伤害" + to_string(特效参数)) + "。";
	else if (特效类型 == "灵核·蚀天") output += ("本命法宝效果发动，减少敌方防御" + to_string(特效参数)) + "。";
	else if (特效类型 == "灵核·灭魂") output += ("本命法宝效果发动，减少敌方抗性" + to_string(特效参数)) + "。";
	else if (特效类型 == "灵核·天问") output += ("本命法宝效果发动，增加自身双抗" + to_string(特效参数)) + "。";
	else;
	
	if (形制 == "本命法宝·锤印塔" && Rand(2)) {
		形制 = "本命法宝·剑刀枪";
		敌人->AddBuff(晕, +特效参数, 1);
		output += "本命法宝形制特殊效果发动，敌方被眩晕1回合。";
	}

	if (view) cout << "    " << output << endl;
}

int Pet::属性(int 类型) {
	int ret = 面板[类型];
	return ret;
}
Pet::Pet(Person* owner, vector<vector<string>>& pet_table) {
	主人 = owner;
	if (pet_table[0].size() == 1)名称 = "";
	else 名称 = GetString(pet_table, 0, 1);
	for (int i = 0; i < 7; i++)面板[i] = 0;
	if (pet_table[1].size() == 1)面板[攻] = 0;
	else 面板[攻] = GetInt(pet_table, 1, 1);
}
void Pet::Round() {
	string output = "    ";
	int 伤害结果 = 敌人->AttackedByPet(攻击);
	output += (主人->名称 +"的 " + string(名称) + " 发动普通攻击，对" + 敌人->名称 + "造成 " + to_string(伤害结果) + " 点伤害。");
	if (view) cout << output << endl;
}

pair<bool, pair<int, int>> War(Person& 进攻者, Person& 防御者) {
	进攻者.敌方 = &防御者;
	防御者.敌方 = &进攻者;
	Person* 顺序[2];
	if (进攻者.速度 < 防御者.速度) { 顺序[0] = &防御者; 顺序[1] = &进攻者; }
	else { 顺序[0] = &进攻者; 顺序[1] = &防御者; }
	int 防御者第7回合生命 = 防御者.生命;
	for (int r = 0; r < 50; r++) {
		if (view) cout << "回合" << r+1 << "：" << endl;
		if (view) cout << "    【当前生命：" << 顺序[0]->名称 << 顺序[0]->生命 << "  " << 顺序[1]->名称 << 顺序[1]->生命 << "】" <<endl;
		顺序[0]->Round();
		if (顺序[1]->生命 == 0 && view) { cout << 顺序[1]->名称 << "被" << 顺序[0]->名称 << "击败。" << endl; break; }
		顺序[1]->Round();
		if (顺序[0]->生命 == 0 && view) { cout << 顺序[0]->名称 << "被" << 顺序[1]->名称 << "击败。" << endl; break; }
		if (r < 7) 防御者第7回合生命 = 防御者.生命;
	}
	if (顺序[0]->生命 != 0 && 顺序[1]->生命 != 0 && view) {
		cout << 进攻者.名称 << "因体力不支败走。" << endl;
		return { 1, {防御者第7回合生命, 防御者.生命} };
	}
	else if (进攻者.生命 == 0)return {1, {防御者第7回合生命, 防御者.生命}};
	else return { 0, {防御者第7回合生命, 防御者.生命} };
}
