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
	bool isDead = false;
private:
	//NCharacter self;
	std::string name = "EmptyNick";
};

//������
class NClient {
public:
	//��� ������������ �������� (���������/������)
	std::string init(std::string inRole) {
		//�������� �� �������
		if (sizeof(inRole) < 1) return "err1";
		if (inRole != "Inspector" && inRole != "Bandit") return "err2";
		if (inRole == "Bandit") role = BANDIT;
		return "ok";
	};
	//��� ������ ������ ��� 
	std::string makeStep(std::string inMassege) {
		using std::cin;
		using std::cout;

		std::string ans = "";

		char choose, x, y, updown;
		cout << inMassege << "\n";
		cin >> choose;
		switch (choose) {
		case '1': {
			cout << "������� ����������\n";
			cin >> x;
			cin >> y;
			ans = '1';
			ans.push_back(x);
			ans.push_back(y);
			break;
		}
		case '2': {
			switch (role) {
			case INSPECTOR: 
			{
				cout << "O����������\n";
				cin >> x; 
				cin >> y;
				ans = '2';
				ans.push_back(x);
				ans.push_back(y);
				break;
			}
			case BANDIT: 
			{
				cout << "��������\n";
				ans = '2';
				break;
			}
			}
			break;
		}
		case '3': {
			cout << "�������\n";
			cin >> x;
			cin >> y;
			cin >> updown;
			ans = '3';
			ans.push_back(x);
			ans.push_back(y);
			ans.push_back(updown);
			break;
		}
		}
		return ans;
	};
	//��� � ������� ��������� ���� � ������� 
	std::string printField(std::string inField) {
		//�������� �� �������
		if (sizeof(inField) < 1) return "err1";
		//�������� ������������ ������
		if (inField[0] != '%' ||
			inField[inField.size() - 1] != '%')
			return "err2";
		//�������� ��� �� ������ �� ������
		int iter = 0;
		for (int i = 0; i < fieldSizeY; i++) 
		{
			for (int j = 0; j < fieldSizeX; j++) 
			{
				std::string color = "";
				std::string name = "";
				iter++;
				if (iter >= inField.size()) return "err3";
				//��������� ��� ���������
				while (inField[iter] != '%')
				{
					name.push_back(inField[iter]);
					iter++;
					if (iter >= inField.size()) return "err4";
				}
				iter++;
				if (iter >= inField.size()) return "err5";
				//��������� ���� 
				while (inField[iter] != '%') 
				{
					color.push_back(inField[iter]);
					iter++;
					if (iter >= inField.size()) return "err6";
				}
				HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
				if(color == "red") SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 4));
				if (color == "blue") SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 3));
				if (color == "white") SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 15));
				std::cout.width(20);
				std::cout << name << " ";
				//��� �� ��� ��������� ����� ������� �� �������� 
				SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 15));
			}
			std::cout << "\n\n";
		}
		return "ok";
	};
	//�������� ������� ���� 
	std::string fieldReSize(std::string inSize){
		//�������� �� �������
		if (sizeof(inSize) < 1) return "err1";
		//�������� ������������ ������
		if (inSize[0] != '%' ||
			inSize[inSize.size() - 1] != '%')
			return "err3";
		int iter = 0;
		std::string Xsize = "";
		while (inSize[iter] != '%')
		{
			Xsize.push_back(inSize[iter]);
			iter++;
			if (iter >= inSize.size()) return "err3";
		}
		iter++;
		if (iter >= inSize.size()) return "err4";
		std::string Ysize = "";
		while (inSize[iter] != '%')
		{
			Ysize.push_back(inSize[iter]);
			iter++;
			if (iter >= inSize.size()) return "err5";
		}
		fieldSizeX = std::stoi(Xsize);
		fieldSizeY = std::stoi(Ysize);
	}
private:
	enum who_0 {INSPECTOR, BANDIT};
	//����� �� ��������� ������ ����� �����������
	who_0 role = INSPECTOR;
	//�� ��������� ���� 5 �� 5
	int fieldSizeX = 5;
	int fieldSizeY = 5;
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
			return true;
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

		if (field[row][column].player != NULL)
			field[row][column].player->isDead = true;
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
		if (!isNear(row, column, insp)) {std::cout << "Inspector must be close to victim!\n"; return false;}
		if (field[row][column].player != NULL) {
			field[row][column].player->isDead = true;
			return true;
		}
		std::cout << "Mistake\n";
		return true;
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
	bool Hide(NCharacter &old, NCharacter& newC, NPlayer* player) {
		bool changedPer = false;
		for (int i = 0; i < field.size(); i++) {
			for (int j = 0; j < field[i].size(); j++)
			{
				if (field[i][j].name == newC.name) {
					if (field[i][j].isDead) 
					{ 
						std::cout << "�������� �� ������ ��� �����!\n"; 
						return true; 
					}
					field[i][j].player = player;
					changedPer = true;
				}
			}
		}
		for (int i = 0; i < field.size(); i++) {
			for (int j = 0; j < field[i].size(); j++)
			{
				if (field[i][j].name == old.name && changedPer)
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
	bool Move(int row, int column, char dir) {
		if (dir != 'u' && dir != 'd' && dir != 'l' && dir != 'r') { 
			std::cout << "Error, Wrong direction (u d l r)";
			return false;
		}
		if (row < 0 || column < 0 || 
			row >= field.size() || 
			column >= field[row].size()
			) 
		{ 
			std::cout << "Character with this coords does not exist!\n";
			return false;
		}
		//���� � ����� ��������� ������� � ������ ������, ����� ������� ��������� � ���� (����� ���������� ���, �� ���������� ������)
		std::vector<NCharacter> buff;
		if (dir == 'u' || dir == 'd') {
			buff.resize(field.size());
			for (int i = 0; i < buff.size(); ++i) {
				buff[i] = field[i][column];
			}
			if (dir == 'u') {
				NCharacter buffer = buff.front();
				for (int i = 0; i < buff.size()-1; ++i) {
					buff[i] = buff[i+1];
				}
				buff.back() = buffer;
			}
			if (dir == 'd') {
				NCharacter buffer = buff.back();
				for (int i = buff.size() - 1; i > 0; --i) {
					buff[i] = buff[i - 1];
				}
				buff.front() = buffer;
			}
			for (int i = 0; i < buff.size(); ++i) {
				field[i][column] = buff[i];
			}
		}
		if (dir == 'l' || dir == 'r') {
			buff.resize(field[0].size());
			for (int i = 0; i < buff.size(); ++i) {
				buff[i] = field[row][i];
			}
			if (dir == 'l') {
				NCharacter buffer = buff.front();
				for (int i = 0; i < buff.size() - 1; ++i) {
					buff[i] = buff[i + 1];
				}
				buff.back() = buffer;
			}
			if (dir == 'r') {
				NCharacter buffer = buff.back();
				for (int i = buff.size() - 1; i > 0; --i) {
					buff[i] = buff[i - 1];
				}
				buff.front() = buffer;
			}
			for (int i = 0; i < buff.size(); ++i) {
				field[row][i] = buff[i];
			}
		}
		return true;
	};
	//������� ��������
	void Refreash(NClient* client) {
		int columnNum = 0;	bool columnCan;
		int rowNum = 0;	bool rowCan;
		for (int i = 0; i < field.size(); i++) 
		{
			columnCan = false;
			for (int j = 0; j < field[i].size(); j++) 
			{
				if (!columnCan && field[i][j].isDead == true) 
				{
					columnNum++;
					columnCan = true;
				}
			}
		}
		for (int i = 0; i < field[0].size(); i++)
		{
			rowCan = false;
			for (int j = 0; j < field.size(); j++)
			{
				if (!rowCan && field[j][i].isDead == true)
				{
					rowNum++;
					rowCan = true;
				}
			}
		}
		if (rowNum == field.size()) {
			
			for (int i = 0; i < field.size(); i++)
			{
				std::vector<NCharacter> temp(field.size());
				columnCan = false;
				for (int j = 0; j < field[i].size(); j++)
				{
					if (!columnCan && field[i][j].isDead == true)
					{
						columnCan = true;
					}
					else 
					{
						temp.push_back(field[i][j]);
					}
				}
				field[i] = temp;
			}
		}
		client->fieldReSize(std::to_string(field.size()) + "%" + std::to_string(field[0].size()) + "%");
	};

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
	std::string sendNoods() {
		std::string outp = "%";
		for (int i = 0; i < field.size(); ++i) {
			for (int j = 0; j < field[i].size(); ++j) {
				outp += field[i][j].name;
				if (field[i][j].isDead)
				{
					outp += "%red%";
				}
				else
				{
					if (field[i][j].isJustified)
						outp += "%blue%";
					else
						outp += "%white%";
				}
			}
		}
		return outp;
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
	Inspector(std::string PersonName, NDeck& deck, NField& field, NClient* client0) {
		if (sizeof(PersonName) >= 1) {
			originPlayer.setName(PersonName);
			if (deck.deckSize() < 1) 
			{ 
				std::cout << "Error, NoName\n"; 
				system("pause"); 
			}
			player = deck.giveOne();
			for (int i = 0; i < 3; ++i) 
			{
				if (deck.deckSize() < 1)
				{ 
					std::cout << "Error, NoName\n"; 
					system("pause"); 
				}
				Hand[i] = deck.giveOne();
			}

			field.SetPlayerIdentity(player, &originPlayer);

			client = client0;
		}
		else std::cout << "Error, Wrong name\n";
	}

	void MakeSurvey(int row, int column, NField* field) {
		field->Survey(row, column, &originPlayer);
	};
	bool Choose(int row, int column, NField& field, NDeck& deck) {							//����� ���������� ������???
		return field.Choose(row, column, &originPlayer);
	};
	bool Justify(int row, int column, NField& field) {
		return field.Justify(row, column, &originPlayer);
	};
	bool Move(int row, int column, char dir, NField& field) {
		return field.Move(row, column, dir);
	};
	void Refresh(NField& field) {
		//field.Refreash();
	};
	bool ISDEAD() {
		return originPlayer.isDead;
	}
	NClient* client;
private:
	NPlayer originPlayer;
	NCharacter player;
	
	//4 ����� � ���� - �������� (������ 3, �� ����� ������ ����� ����� �� ������ - ����������� 4-�)
	NCharacter Hand[4];
};

class Bandit {
public:
	//������� ���������� ��� ������, ������ ����� ������ ����� � ����, ����� ����� ��������� �� ����
	Bandit(std::string PersonName, NDeck& deck, NField& field, NClient* client0) {
		if (sizeof(PersonName) >= 1) {
			originPlayer.setName(PersonName);
			if (deck.deckSize() < 1) { std::cout << "Error, NoName\n"; system("pause"); }
			player = deck.giveOne();

			field.SetPlayerIdentity(player, &originPlayer);
			client = client0;
		}
		else std::cout << "Error, Wrong name\n";

	}

	//���������� true ���� �������� �������
	bool Kill(int row, int column, NField& field) {
		return field.Kill(row, column, &originPlayer);
	}
	bool Hide(NDeck& deck, NField& field) {
		if (deck.deckSize() < 1)
		{
			std::cout << "Error, no cards\n";
			return false;
		}	//����� ����� �����, ���������
		NCharacter old = player;
		player = deck.giveOne();

		return field.Hide(old, player, &originPlayer);
	};
	bool Move(int row, int column, char dir, NField& field) {
		return field.Move(row, column, dir);
	};
	void Refresh(NField& field) {
		//field.Refreash();
	};
	bool ISDEAD() {
		return originPlayer.isDead;
	}
	NClient* client;
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

	std::string rightInput() {

	}
	//��� ����
	void gameRun() {
		std::string SSS = "Characters.txt";
		deck.readCharacters(SSS, 5);
		deck.shakeRand();
		field.FillField(5);
		std::string name1 = "Alex";
		std::string name2 = "Lesha";
		Bandit bandit(name1, deck, field, &client1);
		Inspector inspector(name2, deck, field, &client2);

		//������, ������1 ���� ��� ������ �������� ����� (��� ������� ��)))
		client1.init("Bandit");

		while (!gameOver) {
			using std::cout;
			using std::cin;
			std::string inp;
			//������� ���� � �������1
			client1.printField(field.sendNoods());
			cout << "��� �������\n";
			again1:
			//��������� ����� �� �������
			inp = bandit.client->makeStep("����");
			if (sizeof(inp) < 1) cout << "error1, game while\n";
			switch (inp[0]) {
			case '1': {
				if (!bandit.Kill((int)inp[1] - 48, (int)inp[2] - 48, field)) 
					{ 
						goto again1; 
					};
				break;
			}
			case '2': {
					if (!bandit.Hide(deck, field)) 
					{ 
						goto again1; 
					}
				break;
			}
			case '3': {
				if (!bandit.Move((int)inp[1] - 48, (int)inp[2] - 48, inp[3], field))
					{ 
						goto again1; 
					}
				break;
			}
			default: {
				cout << "input mistake, game while1\n";
				break;
			}
			}
			//������� ���� � �������2
			client2.printField(field.sendNoods());
			cout << "��� ����������\n";
		again2:
			inp = inspector.client->makeStep("����");
			if (sizeof(inp) < 1) cout << "error2, game while\n";
			switch (inp[0]) {
			case '1': {
				//� ������ ��� ���� ���������?
				if (!inspector.Choose((int)inp[1] - 48, (int)inp[2] - 48, field, deck))
				{
					goto again2;
				};
				break;
			}
			case '2': {
				if (!inspector.Justify((int)inp[1] - 48, (int)inp[2] - 48, field))
				{
					goto again2;
				}
				break;
			}
			case '3': {
				if (!inspector.Move((int)inp[1] - 48, (int)inp[2] - 48, inp[3], field))
				{
					goto again2;
				}
				break;
			}
			default: {
				cout << "input mistake, game while2\n";
				break;
			}
			}
		}

		//while (!gameOver) {
		//	using std::cout;
		//	using std::cin;

		//	printField();
		//	//��� ������� (���� �������� �� ������) (���� ���-�� ������� ������ break)
		//	cout << "��� �������\n";
		//	int choose,x,y;
		//	char updown;
		//	cin >> choose;
		//	again1:
		//	switch (choose) {
		//	case 1: {
		//		cout << "������� ���������� ����������\n";
		//		cin >> x; cin >> y;
		//		if (!bandit.Kill(x, y, field)) 
		//		{ 
		//			goto again1; 
		//		};
		//		break;
		//	}
		//	case 2: {
		//		cout << "��������\n";
		//		if (!bandit.Hide(deck, field)) 
		//		{ 
		//			goto again1; 
		//		}
		//		break;
		//	}
		//	case 3: {
		//		cout << "�������\n";
		//		cin >> x; cin >> y; cin >> updown;
		//		if (!bandit.Move(x, y, updown, field)) 
		//		{ 
		//			goto again1; 
		//		}
		//		break;
		//	}
		//	}
		//	printField();
		//	if (inspector.ISDEAD()) 
		//	{ 
		//		gameOver = true; 
		//		cout << "game over! bandit" << name1 << "wins!\n"; 
		//		system("pause"); 
		//	}
		//	//��� ���������� (���� �������� �� ������)
		//	cout << "��� ����������\n";
		//	cin >> choose;
		//	again2:
		//	switch (choose) {
		//	case 1: {
		//		cout << "������� ���������� ���������\n";
		//		cin >> x; cin >> y;
		//		if (!inspector.Choose(x, y, field, deck)) 
		//		{ 
		//			goto again2; 
		//		}
		//		break;
		//	}
		//	case 2: {
		//		cout << "�����������\n";
		//		cin >> x; cin >> y;
		//		if (!inspector.Justify(x, y, field)) 
		//		{ 
		//			goto again2; 
		//		}
		//		break;
		//	}
		//	case 3: {
		//		cout << "�������\n";
		//		cin >> x; cin >> y; cin >> updown;
		//		if (!inspector.Move(x, y, updown, field)) 
		//		{ 
		//			goto again2; 
		//		}
		//		break;
		//	}
		//	}
		//	printField();
		//	if (bandit.ISDEAD()) 
		//	{ 
		//		gameOver = true; 
		//		cout << "game over! inspector" << name2 << "wins!\n"; 
		//		system("pause"); 
		//	}
		//}
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
	NClient client1;
	NClient client2;
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