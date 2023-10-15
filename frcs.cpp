#include "frcs.h"
bool view = 0;
fstream outfile;
bool Rand(int p) {
	if (p >= 100)return true;
	if (p <= 0)return false;
	float g = ((rand() % 8192) / 8192.0) * 100.0;
	return (float(p) >= g);
}
vector<string> ��װѡ���� = { "��װѡ��", "����1", "����2", "����3", "����4", "����5" };
vector<string> ����ѡ���� = { "����ѡ��", "����","����","����","��������������ǹ","������������ӡ��","�����������ͼ��","��������������ƿ" };
vector<string> ����ѡ���� = { "����ѡ��", "��","ľ","ˮ","��","��","��" };
vector<string> ��Чѡ���� = { "��Чѡ��", "", "��������Ԫ", "�������ӹ�","�������ӷ�","��������Ѫ","����������","�������ӱ�","������ѣ��","����������","��ˡ���ħ","��ˡ�����","��ˡ�ī��","��ˡ�����","��ˡ����","��ˡ��׺�","��ˡ�̫��","��ˡ�����","��ˡ�����","��ˡ�ʴ��" };
//vector<string> ��ս���ü�� = { "��ս����", "����1", "����2", "����3", "����4", "����5" };
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
			string err = check[0] + "(��ǰ��" + table[row][col] + ")�����������ַ����е�һ����" + check[1];
			for (int i = 2; i < check.size(); i++)err += ("  " + check[i]);
			throw(err);
		}
	}
	return table[row][col];
}

Person::Person(string name, vector<vector<string>>& tool_table, vector<vector<string>>& attr_table, vector<vector<string>>& pet_table) {
	���� = name;
	for (int i = 1; i < ATTRSIZE; i++)���[i] = GetInt(attr_table, i - 1, 1);
	���� = Ѫ��;
	�ϴγ��� = 0;
	//for (int i = 1; i < ATTRSIZE; i++)cout << ���[i] << " "; cout << endl;

	string setid_str = GetString(tool_table, 1, 0, ��װѡ����);
	int setid = atoi(setid_str.substr(4, 5).c_str());

	int cnt = 0;
	for (int i = 1; i < tool_table.size(); i++) {
		bool found = 0;
		for (int j = 1; j < tool_table.size(); j++) {
			int rank = GetInt(tool_table, j, setid);
			if (found == 0 && rank == i) {
				found = 1;
				Unit u(this, tool_table, j);
				����.push_back(u);
			}
			else if (found == 1 && rank == i) {
				throw("�����ظ��ĳ�ս˳��");
			}
			else if (rank < 0 || rank >= tool_table.size()) {
				throw("�쳣�ĳ�ս˳��");
			}
		}
	}

	Pet* p = new Pet(this, pet_table);
	���� = p;
	if (p->���� == "")���� = NULL;
}
int Person::����(int ����) {
	int ret = ���[����];
	for (int i = 0; i < ״̬.size(); i++) {
		if (״̬[i].���� == ����) ret += ״̬[i].��ֵ;
	}
	return ret;
}
void Person::AddBuff(int type, int amount, int round) {
	if (type == Ѫ && amount > 0) {
		���� = min(Ѫ��, ���� + amount);
		return;
	}
	if (type == Ѫ && amount <= 0) {
		���� = max(0, ���� + amount);
		�ϴγ��� += (-amount);
		return;
	}
	Buff buff;
	buff.���� = type;
	buff.��ֵ = amount;
	buff.�غ� = round;
	״̬.push_back(buff);
}
void Person::ƽA() {
	pair<int, int> �˺���� = �з�->Attacked(����);
	string output = ���� + "��" + �з�->���� + "��� " + to_string(�˺����.first) + " ���˺���";
	if (view) cout << "    " << output << endl;
}
pair<int, int> Person::Attacked(int physic_harm, string attr, int attr_harm, int attr_thrust) {
	int attr_def =
		attr == "��" ? �� :
		attr == "ľ" ? ľ�� :
		attr == "ˮ" ? ˮ�� :
		attr == "��" ? �� :
		attr == "��" ? ���� :
		attr == "��" ? �׿� : 0;
	attr_def = max(attr_def - attr_thrust, 0);
	physic_harm = max(0, physic_harm - ����);
	physic_harm = physic_harm + rand() % 5 + 1;
	attr_harm = max(0, attr_harm - attr_def);
	int harm = physic_harm + attr_harm;
	���� = max(0, ���� - harm);
	�ϴγ��� = harm;
	return { harm, attr_harm };
}
int Person::AttackedByPet(int harm) {
	���� = max(0, ���� - harm);
	return harm;
}
void Person::Round() {
	//cout << "״̬����"; for (int i = 0; i < ״̬.size(); i++)cout << ״̬[i].���� << " "; cout << endl;
	if (���� == 0 || �з�->���� == 0)return;
	bool dizzy = 0;
	for (int i = 0; i < ״̬.size(); i++) {
		if (״̬[i].���� == ��) dizzy = 1;
	}
	vector<Buff> tmp1 = ״̬; ״̬.clear();
	for (int i = 0; i < tmp1.size(); i++) if (tmp1[i].���� != ��) ״̬.push_back(tmp1[i]);

	bool ��봴��д󱬷� = 0;
	for (int i = 0; i < ����.size(); i++) {
		if (����[i].��Ч���� == "��������Ԫ" && ����[i].�������� > 0 && ���� < 0.3 * Ѫ��) {
			����[i].ʹ��();
			��봴��д󱬷� = 1;
		}
	}
	if (!��봴��д󱬷�) {
		if (!dizzy) {
			int rand_index = rand() % 100;
			for (int i = 0; i < ����.size() && rand_index >= 0; i++) {
				if (����[i].�������� > 0) {
					rand_index -= ����[i].��������;
					if (rand_index < 0) {
						����[i].ʹ��();
					}
				}
			}
			if (rand_index >= 0) { ƽA(); }
		}
		else if (view) cout << "    " << ���� << "���ڻ���״̬��" << endl;
	}

	if (���� != NULL)����->Round();
	
	for (int i = 0; i < ״̬.size(); i++) ״̬[i].�غ�--;
	vector<Buff> tmp2 = ״̬; ״̬.clear();
	for (int i = 0; i < tmp2.size(); i++) if (tmp2[i].�غ� > 0) ״̬.push_back(tmp2[i]);
}

Unit::Unit(Person* owner, vector<vector<string>>& tool_table, int line) {
	//cout << "  "; for (int i = 1; i < tool_table[line].size(); i++)cout << tool_table[line][i] << " "; cout << endl;
	���� = GetString(tool_table, line, 6);
	���� = GetString(tool_table, line, 7, ����ѡ����);
	���� = GetString(tool_table, line, 8, ����ѡ����);
	�������� = GetInt(tool_table, line, 9);
	�������� = int(GetFloat(tool_table, line, 10) * 100);
	�˺�ϵ�� = GetFloat(tool_table, line, 11);
	�˺����� = GetInt(tool_table, line, 12);
	�˺����� = GetFloat(tool_table, line, 13);
	��Ч���� = GetString(tool_table, line, 15, ��Чѡ����);
	��Ч���� = GetFloat(tool_table, line, 16);
	if (��Ч����.substr(0, 4) == "���" && ��Ч���� > 1)��Ч���� /= 100.0;
	���� = owner;
}
void Unit::ʹ��() {
	string output = ����->���� + "����" + ���� + "��" + ����->���� + "��� ";

	int ��ֵ = int((1.0 + �˺�����) * (�˺�ϵ�� * ����->���� + �˺�����));
	int ���� = ��Ч���� == "����������" ? int(��Ч����) : 0;
	pair<int,int> �˺���� = ����->�з�->Attacked(����->����, ����, ��ֵ, ����);
	��������--;
	output = output + to_string(�˺����.first) + " (" + ���� + "��+" + to_string(�˺����.second) + ")���˺���";
	if (��Ч���� == "��������Ԫ")����->AddBuff(Ѫ, +0.2 * ����->Ѫ��, 0);
	else if (��Ч���� == "�������ӹ�") ����->AddBuff(��, +��Ч����, 10);
	else if (��Ч���� == "�������ӷ�") ����->AddBuff(��, +��Ч����, 10);
	else if (��Ч���� == "��������Ѫ") ����->AddBuff(Ѫ, +��Ч����, 0);
	else if (��Ч���� == "����������") ����->AddBuff(��, +��Ч����, 10);
	else if (��Ч���� == "�������ӱ�") ����->AddBuff(��, +��Ч����, 10);
	else if (��Ч���� == "������ѣ��") ����->AddBuff(��, +��Ч����, 1);
	else if (��Ч���� == "��ˡ���ħ") ����->AddBuff(Ѫ, +��Ч���� * ����->����, 0);
	else if (��Ч���� == "��ˡ�����") ����->AddBuff(��, -��Ч���� * ����->����, 2);
	else if (��Ч���� == "��ˡ�ī��") ����->AddBuff(Ѫ, -��Ч���� * ����->Ѫ��, 0);
	else if (��Ч���� == "��ˡ�����") ����->AddBuff(Ѫ, +��Ч���� * ����->����, 0);
	else if (��Ч���� == "��ˡ��׺�") ����->AddBuff(Ѫ, -��Ч���� * ����->�ϴγ���, 0);
	else if (��Ч���� == "��ˡ�̫��") ����->AddBuff(��, +��Ч���� * ����->����, 2);
	else if (��Ч���� == "��ˡ�����");
	else if (��Ч���� == "��ˡ�ʴ��") ����->AddBuff(��, -��Ч���� * ����->����, 2);
	else if (��Ч���� == "��ˡ����") {
		����->AddBuff(��, -��Ч���� * ����->��, 2);
		����->AddBuff(ľ, -��Ч���� * ����->ľ��, 2);
		����->AddBuff(ˮ, -��Ч���� * ����->ˮ��, 2);
		����->AddBuff(��, -��Ч���� * ����->��, 2);
		����->AddBuff(��, -��Ч���� * ����->����, 2);
		����->AddBuff(��, -��Ч���� * ����->�׿�, 2);
	}
	else if (��Ч���� == "��ˡ�����") {
		����->AddBuff(��, +��Ч���� * ����->����, 2);
		����->AddBuff(��, +��Ч���� * ����->��, 2);
		����->AddBuff(ľ, +��Ч���� * ����->ľ��, 2);
		����->AddBuff(ˮ, +��Ч���� * ����->ˮ��, 2);
		����->AddBuff(��, +��Ч���� * ����->��, 2);
		����->AddBuff(��, +��Ч���� * ����->����, 2);
		����->AddBuff(��, +��Ч���� * ����->�׿�, 2);
	}
	else;

	if (��Ч���� == "��������Ԫ")output += ("���������ָ�" + to_string(0.2 * ����->Ѫ��) + "��");
	else if (��Ч���� == "�������ӹ�") output += ("���������" + to_string(int(��Ч����)) + "�㣬����10�غϡ�");
	else if (��Ч���� == "�������ӷ�") output += ("����������" + to_string(int(��Ч����)) + "�㣬����10�غϡ�");
	else if (��Ч���� == "��������Ѫ") output += ("���������ָ�" + to_string(int(��Ч����)) + "��");
	else if (��Ч���� == "����������") output += ("�����������" + to_string(int(��Ч����)) + "�㣬����10�غϡ�");
	else if (��Ч���� == "�������ӱ�") output += ("���������" + to_string(int(��Ч����)) + "�㣬����10�غϡ�");
	else if (��Ч���� == "������ѣ��") output += ("��ѣ�ζԷ�1�غϡ�");
	else if (��Ч���� == "��ˡ���ħ") output += ("��������Ч���������ָ���������" + to_string(int(��Ч���� * ����->����)) + "��");
	else if (��Ч���� == "��ˡ�����") output += ("��������Ч�����������ٵз�����" + to_string(��Ч����)) + "��";
	else if (��Ч���� == "��ˡ�ī��") output += ("��������Ч�����������⸽���˺�" + to_string(��Ч����)) + "��";
	else if (��Ч���� == "��ˡ�����") output += ("��������Ч���������ָ���������" + to_string(int(��Ч���� * ����->����)) + "��");
	else if (��Ч���� == "��ˡ��׺�") output += ("��������Ч�����������⸽���˺�" + to_string(��Ч����)) + "��";
	else if (��Ч���� == "��ˡ�̫��") output += ("��������Ч������������������" + to_string(��Ч����)) + "��";
	else if (��Ч���� == "��ˡ�����") output += ("��������Ч�����������⸽���˺�" + to_string(��Ч����)) + "��";
	else if (��Ч���� == "��ˡ�ʴ��") output += ("��������Ч�����������ٵз�����" + to_string(��Ч����)) + "��";
	else if (��Ч���� == "��ˡ����") output += ("��������Ч�����������ٵз�����" + to_string(��Ч����)) + "��";
	else if (��Ч���� == "��ˡ�����") output += ("��������Ч����������������˫��" + to_string(��Ч����)) + "��";
	else;
	
	if (���� == "������������ӡ��" && Rand(2)) {
		���� = "��������������ǹ";
		����->AddBuff(��, +��Ч����, 1);
		output += "����������������Ч���������з���ѣ��1�غϡ�";
	}

	if (view) cout << "    " << output << endl;
}

int Pet::����(int ����) {
	int ret = ���[����];
	return ret;
}
Pet::Pet(Person* owner, vector<vector<string>>& pet_table) {
	���� = owner;
	if (pet_table[0].size() == 1)���� = "";
	else ���� = GetString(pet_table, 0, 1);
	for (int i = 0; i < 7; i++)���[i] = 0;
	if (pet_table[1].size() == 1)���[��] = 0;
	else ���[��] = GetInt(pet_table, 1, 1);
}
void Pet::Round() {
	string output = "    ";
	int �˺���� = ����->AttackedByPet(����);
	output += (����->���� +"�� " + string(����) + " ������ͨ��������" + ����->���� + "��� " + to_string(�˺����) + " ���˺���");
	if (view) cout << output << endl;
}

pair<bool, pair<int, int>> War(Person& ������, Person& ������) {
	������.�з� = &������;
	������.�з� = &������;
	Person* ˳��[2];
	if (������.�ٶ� < ������.�ٶ�) { ˳��[0] = &������; ˳��[1] = &������; }
	else { ˳��[0] = &������; ˳��[1] = &������; }
	int �����ߵ�7�غ����� = ������.����;
	for (int r = 0; r < 50; r++) {
		if (view) cout << "�غ�" << r+1 << "��" << endl;
		if (view) cout << "    ����ǰ������" << ˳��[0]->���� << ˳��[0]->���� << "  " << ˳��[1]->���� << ˳��[1]->���� << "��" <<endl;
		˳��[0]->Round();
		if (˳��[1]->���� == 0 && view) { cout << ˳��[1]->���� << "��" << ˳��[0]->���� << "���ܡ�" << endl; break; }
		˳��[1]->Round();
		if (˳��[0]->���� == 0 && view) { cout << ˳��[0]->���� << "��" << ˳��[1]->���� << "���ܡ�" << endl; break; }
		if (r < 7) �����ߵ�7�غ����� = ������.����;
	}
	if (˳��[0]->���� != 0 && ˳��[1]->���� != 0 && view) {
		cout << ������.���� << "��������֧���ߡ�" << endl;
		return { 1, {�����ߵ�7�غ�����, ������.����} };
	}
	else if (������.���� == 0)return {1, {�����ߵ�7�غ�����, ������.����}};
	else return { 0, {�����ߵ�7�غ�����, ������.����} };
}
