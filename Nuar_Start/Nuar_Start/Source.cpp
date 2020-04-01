#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <Windows.h>
#include <fstream>
#include <ctime>

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
	void readCharacters(std::string filename, int size) {
		if (size != 5 && size != 6 && size != 7) { std::cout << "Error, not possible game size (5 6 7)! You entered" << size << "!\n"; system("pause"); }
		std::ifstream in(filename);
		if (!in.is_open()) { std::cout << "Error, Character file is not found!\n"; system("pause"); }
		if (!deck.empty()) { std::cout << "Warning, deck is not empty! Can be mistakes!\n"; system("pause"); }
		//���������� � ������ ���������� �� ����� ����� �����������
		for (int i = 0; i < size*size; ++i) {
			NCharacter buf;
			in >> buf.name;
			deck.push(buf);
		}
		in.close();
	};
	//������������ ������
	void shakeRand() {
		std::vector<NCharacter> buffer;
		int ssizeDeck = deck.size();
		for (int i = 0; i < ssizeDeck; ++i) {
			buffer.push_back(deck.front());
			deck.pop();
		}

		//std::default_random_engine dre(time(NULL));
		std::random_shuffle(buffer.begin(), buffer.end());

		for (int i = 0; i < ssizeDeck; ++i) {
			deck.push(buffer.back());
			buffer.pop_back();
		}
	};

	//���������� ������� ����� ������
	NCharacter giveOne() {
		if (!deck.empty()) {
			NCharacter buf = deck.front();
			deck.pop();
			std::cout << "������:" << buf.name << "\n";
			return buf;
		}
		else { 
			std::cout << "Error, No cards!\n";
			NCharacter buf;
			return buf;
		}
	};

	int deckSize() {
		return deck.size();
	}
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
		in.close();
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
		std::cout << "Mistake\n";
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
	bool Hide(NCharacter newC, NPlayer* player, NDeck &deck, NCharacter &player2) {
		if (deck.deckSize() < 1) { std::cout << "Error, no cards\n"; }
		player2 = deck.giveOne();
		bool changedPer = false;
		for (int i = 0; i < field.size(); i++) {
			for (int j = 0; j < field[i].size(); j++)
			{
				if (field[i][j].name == newC.name) {
					if (field[i][j].isDead) { std::cout << "�������� �� ������ ��� �����!\n"; return false; }
					field[i][j].player = player;
					changedPer = true;
				}
				if (field[i][j].player == player && changedPer)
				{
					field[i][j].player = NULL;
					field[i][j].isJustified = true;
					return true;
				}
			}
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
			if (deck.deckSize() < 1) { std::cout << "Error, NoName\n"; system("pause"); }
			player = deck.giveOne();
			for (int i = 0; i < 3; ++i) {
				if (deck.deckSize() < 1) { std::cout << "Error, NoName\n"; system("pause"); }
				Hand[i] = deck.giveOne();
			}

			field.SetPlayerIdentity(player, &originPlayer);
		}
		else std::cout << "Error, Wrong name\n";
	}

	void MakeSurvey(int row, int column, NField* field) {
		field->Survey(row, column, &originPlayer);
	};
	bool Choose(int row, int column, NField& field, NDeck& deck) {							//����� ���������� ������???
		return field.Choose(row, column, &originPlayer);
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
	bool ISDEAD() {
		return player.isDead;
	}
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
			if (deck.deckSize() < 1) { std::cout << "Error, NoName\n"; system("pause"); }
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
		field.Hide(player, &originPlayer, deck, player);
	};
	void Move(int row, int column, char dir, NField& field) {
		field.Move(row, column, dir);
	};
	void Refresh(NField& field) {
		field.Refreash();
	};
	bool ISDEAD() {
		return player.isDead;
	}
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
		std::string SSS = "Characters.txt";
		deck.readCharacters(SSS, 5);
		deck.shakeRand();
		field.FillField(5);
		std::string name1 = "Alex";
		std::string name2 = "Lesha";
		Bandit bandit(name1, deck, field);
		Inspector inspector(name2, deck, field);

		while (!gameOver) {
			using std::cout;
			using std::cin;

			printField();
			//��� ������� (���� �������� �� ������) (���� ���-�� ������� ������ break)
			cout << "��� �������\n";
			int choose,x,y;
			cin >> choose;
			switch (choose) {
			case 1: cout << "������� ���������� ����������\n"; cin >> x; cin >> y; bandit.Kill(x, y, field); break;
			case 2: cout << "��������\n"; bandit.Hide(deck, field); break;
			}
			printField();
			if (inspector.ISDEAD()) { gameOver = true; cout << "Game Over! Bandit" << name1 << "Wins!\n"; system("pause"); }
			//��� ���������� (���� �������� �� ������)
			cout << "��� ����������\n";
			cin >> choose;
			switch (choose) {
			case 1: cout << "������� ���������� ����������\n"; cin >> x; cin >> y; inspector.Choose(x, y, field, deck); break;
			case 2: cout << "�����������\n"; cin >> x; cin >> y; inspector.Justify(x, y, field); break;
			}
			printField();
			if (bandit.ISDEAD()) { gameOver = true; cout << "Game Over! Inspector" << name2 << "Wins!\n"; system("pause"); }
		}
	}

private:
	NDeck deck;
	//��������� ������ � �� � ������������ ����
	NField field;
	//��� ������ ������ � �������� (�� ������ ������� � ������� ����������� � ��������� � �������)
	//NPlayer bandit;
	//NPlayer inspector;
	//Bandit bandit("sd", deck, field);
	//Inspector inspector;
	bool gameOver = false;
	//��������
	std::string gameName = "Bandit vs Inspector\n";
};


int main() {
	srand(time(NULL));
	setlocale(LC_ALL, "Russian");

	NGame1 a;
	a.gameRun();
	/*
	NDeck aaa;
	std::string SSS = "Characters.txt";
	aaa.readCharacters(SSS, 5);
	aaa.shakeRand();
	std::cout << aaa.giveOne().name << "\n";
	std::cout << aaa.giveOne().name << "\n";
	std::cout << aaa.giveOne().name << "\n";
	std::cout << aaa.giveOne().name << "\n";
	std::cout << aaa.giveOne().name << "\n";
	std::cout << aaa.giveOne().name << "\n";




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
	*/
	system("pause");
	return 0;
}