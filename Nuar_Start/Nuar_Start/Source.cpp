#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <Windows.h>
#include <fstream>

//����� �������� ��� ������
class NPlayer {
public:
	//���������� ���(���) ������
	std::string getName() {
		return name;
	}
	//������������� ���(���) ������
	void setName(std::string name0) {
		name = name0;
	};
private:
	//NCharacter self;
	bool isDead = false;
	std::string name = "EmptyNick";
};

//����� ��������� ���� (����� ����� ������� �� ������ ��������(������))
class NCharacter
{
public:
	NPlayer* player = NULL;
	bool isDead = false;
	bool isSuspect = false;
	bool isJustified = false;
	bool haveCoin = false;
	bool onField = false;

	std::string name = "NoName";
};

//����� ������
class NDeck{
public:
	//������ � ����� ���������� 
	//�� ������� � ������ ������� ������ �������� �� ������� ������ (1 ��� 2)
	void readCharacters();
	//������������ ������
	void shakeRand();
	//���������� ������� ����� ������
	NCharacter giveOne();
private:
	//���� ������
	std::queue<NCharacter> deck;
	//���������� ���� � ������
	int number;
};

//����� ����(����� �����). ���� ������� ����� ����������� � ��������� ��� �������� �� ����������� � ����
class NField {
public:
	//��������� ���� ����������� 5*5 6*6 7*7 � ��
	void FillField(int size) {
		if (size != 5 && size != 6 && size != 7) { std::cout << "Error, not possible field size (5 6 7)! You entered" << size << "!\n"; system("pause"); }
		std::ifstream in("Characters.txt");
		if (!in.is_open()) { std::cout << "Error, Character file is not found!\n"; }
		field.resize(size);
		for (int i = 0; i < size; ++i) {
			field[i].resize(size);
			for (int j = 0; j < size; ++j) {
				in >> field[i][j].name;
				field[i][j].isSuspect = true;
			}
		}
	}
	//���������� ����
	void ShakeFieldOF();
	//�������� ����� (�������� ������ ����� ��� ������ �� ���������� ��� �����) � ���������� ����� ����, ��� �����
	bool Survey(int row, int column, NPlayer* player) {
		int U, D, L, R = 0;
		if (row - 1 < 0) { L = 0; }
		else L = row - 1;
		if (row + 1 >= field.size()) { R = field.size() - 1; }
		else R = row + 1;
		if (column - 1 < 0) { U = 0; }
		else U = column - 1;
		if (column + 1 >= field[0].size()) { D = field[0].size() - 1; }
		else D = column + 1;

		bool isSomeOneHere = false;
		for (int i = L; i <= R; ++i) {
			for (int j = U; j <= D; ++j) {
				if ((field[i][j].player != NULL) && (field[i][j].player != player)) {
					std::cout << field[i][j].player->getName() << " is here!\n";
					isSomeOneHere = true;
				}
			}
		}

		if (isSomeOneHere) { return true; }
		else { 
			std::cout << "Noone is here!\n";
			return false;
		}
	};

	bool Kill(int row, int column, NPlayer* killer) {
		if (row < 0 ||
			column < 0 ||
			row >= field.size() ||
			column >= field[row].size()
			)
		{
			std::cout << "Character with this coords does not exist!\n";
			return false;
		}
		if(field[row][column].player == killer)
		{
			std::cout << "Are you stupid? You'ra trying to kill your self...\n";
			return false;
		}
		if(field[row][column].isDead)
		{
			std::cout << "This character are already killed\n";
			return false;
		}
		//c�� ����� ���������� �� �� ��� �� ������ ��� �����
		if (!isNear(row, column, killer))
		{
			std::cout << "Killer must be close to victim\n";
			return false;
		}

		field[row][column].isDead = true;
		return true;
	}
	// ���������������� ������� ��� ������� � ������� 
	bool isNear(int row, int column, NPlayer* killer) {
		int U, D, L, R = 0;
		if (row - 1 < 0) { L = 0; }
		else L = row - 1;
		if (row + 1 >= field.size()) { R = field.size() - 1; }
		else R = row + 1;
		if (column - 1 < 0) { U = 0; }
		else U = column - 1;
		if (column + 1 >= field[0].size()) { D = field[0].size() - 1; }
		else D = column + 1;

		for (int i = L; i <= R; ++i) 
			for (int j = U; j <= D; ++j) 
				if (field[i][j].player == killer) 
				{
					return true;
				}
		return false;
	}
	//tut t�kaut
	bool Choose(int row, int column, NPlayer* insp)
	{
		if (row < 0 ||
			column < 0 ||
			row >= field.size() ||
			column >= field[row].size()
			)
		{
			std::cout << "Character with this coords does not exist!\n";
			return false;
		}
		if (field[row][column].player == insp)
		{
			std::cout << "Are you stupid? You'ra trying to kill your self...\n";
			return false;
		}
		if (field[row][column].player != NULL)
			return true;
		return false;
	}
	//���� ������� ����� ����������
	void PrintMe() {
		for (int i = 0; i < field.size(); ++i) {
			for (int j = 0; j < field[i].size(); ++j) {
				HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 15));
				if (field[i][j].isJustified) 
				{
					SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 3));
				}
				else 
				{
					if(field[i][j].isDead)
						SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 4));
				}
				std::cout.width(20);
				std::cout << field[i][j].name << " ";
			}
			std::cout << "\n\n";
		}
	};
	void Hide(NCharacter newC, NPlayer* player) {
		for(int i = 0; i < field.size(); i++)
			for (int j = 0; j < field[i].size(); j++)
			{
				if(field[i][j].player == player)
				{
					field[i][j].player = NULL;
					field[i][j].isJustified = true;
				}
				if (field[i][j].name == newC.name)
					field[i][j].player = player;
			}
	}
	bool Justify(int row, int column, NPlayer* insp)
	{
		if (row < 0 ||
			column < 0 ||
			row >= field.size() ||
			column >= field[row].size()
			)
		{
			std::cout << "Character with this coords does not exist!\n";
			return false;
		}
		if (field[row][column].player == insp)
		{
			std::cout << "Are you stupid? You'ra trying to justified your self...\n";
			return false;
		}
		if (field[row][column].isDead)
		{
			std::cout << "This character are already killed\n";
			return false;
		}
		if (field[row][column].isJustified)
		{
			std::cout << "This character are already justified\n";
			return false;
		}
		field[row][column].isJustified = true;
		return Survey(row, column, insp);
	}
	void Move(int row, int column, char dir);
	//������� ��������
	void Refreash();

	//������������� ������ ��� ��������� (����� ��������� ����������� ���� ������)
	void SetPlayerIdentity(NCharacter character, NPlayer* player) {

		for (int i = 0; i < field.size(); ++i) {
			for (int j = 0; j < field[i].size(); ++j) {
				if (field[i][j].name == character.name) {
					field[i][j].player = player;
					return;
				}
			}
		}
		std::cout << "Error, Character is not found! :)))\n";
	}
private:
	//���� ���� ����������
	std::vector<std::vector<NCharacter>> field;
	//����� ���������� ������
	int deadNumber = 0;
};


//��������� (�����)
class Inspector {
public:
	//���������� ���������� ��� ������, ������ ����� ������ ����� � ����, ����� ����� ��������� �� ����
	Inspector(std::string PersonName, NDeck& deck, NField& field) {
		if (sizeof(PersonName) >= 1) {
			originPlayer.setName(PersonName);
			player = deck.giveOne();
			for (int i = 0; i < 3; ++i) {
				Hand[i] = deck.giveOne();
			}

			field.SetPlayerIdentity(player, &originPlayer);
		}
		else std::cout << "Error, Wrong name\n";
	}

	void MakeSurvey(int row, int column, NField* field) {
		field->Survey(row, column, &originPlayer);
	};
	void Choose(int row, int column, NField& field, NDeck& deck) {							//����� ���������� ������???
		field.Choose(row, column, &originPlayer);
	};
	void Justify(int row, int column, NField& field) {
		field.Justify(row, column, &originPlayer);
	};
	void Move(int row, int column, char dir, NField& field) {
		field.Move(row, column, dir);
	};
	void Refresh(NField& field) {
		field.Refreash();
	};
private:
	NPlayer originPlayer;
	NCharacter player;
	//4 ����� � ���� - �������� (������ 3, �� ����� ������ ����� ����� �� ������ - ����������� 4-�)
	NCharacter Hand[4];
};

class Bandit {
public:
	//������� ���������� ��� ������, ������ ����� ������ ����� � ����, ����� ����� ��������� �� ����
	Bandit(std::string PersonName, NDeck& deck, NField& field) {
		if (sizeof(PersonName) >= 1) {
			originPlayer.setName(PersonName);
			player = deck.giveOne();

			field.SetPlayerIdentity(player, &originPlayer);
		}
		else std::cout << "Error, Wrong name\n";
	}

	//���������� true ���� �������� �������
	bool Kill(int row, int column, NField& field) {
		return field.Kill(row, column, &originPlayer);
	}
	void Hide(NDeck& deck, NField& field) {
		player = deck.giveOne();
		field.Hide(player, &originPlayer);
	};
	void Move(int row, int column, char dir, NField& field) {
		field.Move(row, column, dir);
	};
	void Refresh(NField& field) {
		field.Refreash();
	};
private:
	NPlayer originPlayer;
	NCharacter player;
};


class NGame1 {
public:
	//������� ������� ����
	void printField() {
		field.PrintMe();
	}
	//��� ����
	void gameRun() {
		while (!gameOver) {
			printField();
			//��� ������� (���� �������� �� ������) (���� ���-�� ������� ������ break)
			printField();
			//��� ���������� (���� �������� �� ������)
			printField();
		}
	}

private:
	NDeck deck;
	//��������� ������ � �� � ������������ ����
	NField field;
	//��� ������ ������ � �������� (�� ������ ������� � ������� ����������� � ��������� � �������)
	NPlayer bandit;
	NPlayer inspector;
	bool gameOver = false;
	//��������
	std::string gameName = "Bandit vs Inspector\n";
};


int main() {
	setlocale(LC_ALL, "Russian");

	NField F;
	F.FillField(5);
	//F.PrintMe();

	int g, d;
	//std::cin >> g >> d;

	NCharacter CharLina;
	CharLina.name = "�����";
	NCharacter CharLesha;
	CharLesha.name = "��������";
	NPlayer Lina;
	Lina.setName("Lina");
	NPlayer Lesha;
	Lesha.setName("����-������");
	F.SetPlayerIdentity(CharLina, &Lina);
	F.SetPlayerIdentity(CharLesha, &Lesha);
	F.Kill(1, 1, &Lina);
	F.Justify(0, 1, &Lesha);
	//F.Survey(g, d, &Lesha);
	F.PrintMe();



	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 7));
	std::cout << "hello world\n";
	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 2));
	std::cout << "fuck you\n";
	system("pause");
	return 0;
}