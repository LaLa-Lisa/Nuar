#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <Windows.h>
#include <fstream>

class NPlayer {
public:
	std::string getName() {
		return name;
	}
	void setName(std::string name0) {
		name = name0;
	};
private:
	//NCharacter self;
	bool isDead = false;
	std::string name = "EmptyNick";
};


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

class NDeck{
public:
	//������ � ����� ���������� 
	//�� ������� � ������ ������� ������ �������� �� ������� ������ (1 ��� 2)
	void readCharecters();
	void shakeRand();
	NCharacter giveOne();
private:
	std::queue<NCharacter> deck;
	int number;
};


class NField {
public:
	//opros
	void FillField(int size) {
		std::ifstream in("Characters.txt");
		if (!in.is_open()) { std::cout << "Error, Character file is not found!\n"; }
		field.resize(5);
		for (int i = 0; i < size; ++i) {
			field[i].resize(5);
			for (int j = 0; j < size; ++j) {
				in >> field[i][j].name;
				field[i][j].isSuspect = true;
			}
		}
	}

	void ShakeFieldOF();																										//���������� ����
	bool Survey(int row, int column, NPlayer& player) {
		int U = 0;
		int D = 0;
		int L = 0;
		int R = 0;
		if (row - 1 < 0) { L = 0; }
		else L = row - 1;
		if (row + 1 >= field.size()) { R = field.size() - 1; }
		else R = row + 1;
		if (column - 1 < 0) { U = 0; }
		else U = column - 1;
		if (column + 1 >= field[0].size()) { D = field[0].size() - 1; }
		else D = column + 1;

		for (int i = L; i <= R; ++i) {

			for (int j = U; j <= D; ++j) {
				if ((field[i][j].player != NULL) && (field[i][j].player != &player)) {
					std::cout << field[i][j].player->getName() << " is here!\n";
					return true;
				}
			}

		}

		return false;
	};
	void Kill(int row, int column);
	bool Choose(int row, int column);																							//tut t�kaut 
	void PrintMe() {
		for (int i = 0; i < field.size(); ++i) {
			for (int j = 0; j < field[i].size(); ++j) {
				std::cout << field[i][j].name << " ";
			}
			std::cout << "\n";
		}
	};
	void Hide();
	void Justify(int row, int column);
	void Move(int row, int column, char dir);
	//������� ��������
	void Refreash();

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
	std::vector<std::vector<NCharacter>> field;
	int deadNumber = 0;
};




class Inspector {
public:
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
		field->Survey(row, column, originPlayer);
	};
	void Choose(int row, int column, NField& field, NDeck& deck) {
		field.Choose(row, column);
	};
	void Justify(int row, int column, NField& field) {
		field.Justify(row, column);
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
	NCharacter Hand[4];
};

class Bandit {
public:
	Bandit(std::string PersonName, NDeck& deck, NField& field) {
		if (sizeof(PersonName) >= 1) {
			originPlayer.setName(PersonName);
			player = deck.giveOne();

			field.SetPlayerIdentity(player, &originPlayer);
		}
		else std::cout << "Error, Wrong name\n";
	}

	void Kill(int row, int column, NField& field) {
		field.Kill(row, column);
	};
	void Hide(NDeck& deck, NField& field) {
		
	};
	void Move(int row, int column, char dir, NField& field);
	void Refresh(NField& field);
private:
	NPlayer originPlayer;
	NCharacter player;
};


class NGame1 {
public:
	void printField() {
		field.PrintMe();
	}
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
	NPlayer incpector;
	bool gameOver = false;
	//��������
	std::string gameName = "Bandit vs Inspector";
};


int main() {
	setlocale(LC_ALL, "Russian");

	NField F;
	F.FillField(5);
	F.PrintMe();

	int g, d;
	std::cin >> g >> d;

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
	F.Survey(g, d, Lesha);



	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 7));
	std::cout << "hello world\n";
	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 2));
	std::cout << "fuck you\n";
	system("pause");
	return 0;
}