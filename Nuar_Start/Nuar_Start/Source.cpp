#include <iostream>
#include <vector>
#include <queue>
#include <Windows.h>

class NCharacter
{
public:
//	NPlayer* player = NULL;
	bool isDead = false;
	bool isSuspect = false;
	bool isJustified = false;
	bool haveCoin = false;

	std::string name = "NoName";
};

class NDeck{
public:
	//������ � ����� ���������� 
	//�� ������� � ������ ������� ������ �������� �� ������� ������ (1 ��� 2)
	void read();
	void shakeRand();
	void giveOne();
private:
	std::queue<NCharacter> deck;
	int number;
};


class NField {
public:
	//������� ��������
	void move(int row, int column, char dir);
	//opros
	bool Survey(int row, int column);
	void Kill(int row, int column);
	//tut t�kaut 
	bool Ask(int row, int column);
	void printMe();
	void hide();
	void Justify();
	void Refreash();
private:
	std::vector<std::vector<NCharacter>> field;
	int deadNumber = 0;
};


class NPlayer{
public:
	
	NPlayer() {
	}
	void makeSurvey(int row, int column, NField* field) {
		field->Survey(column, row);
	};
	void killSomeone(int row, int column, NField* field);
	std::string getName() {
		return name;
	}
private:
	NCharacter* self;
	bool isDead = false;
	bool isKiller = false;
	bool isDetective = false;
	std::string name = "EmptyNick";
};

class NGame1 {
	void printField() {
		field.printMe();
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
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 7));
	std::cout << "hello world\n";
	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 2));
	std::cout << "fuck you\n";
	system("pause");
	return 0;
}