#include <iostream>
#include <vector>

class NCharacter
{
public:


};


class NField {
public:
	void move(int column, int row);
private:
	std::vector<std::vector<NCharacter>> field;
};

class NDetective
{
	virtual void makeSurvey();
};
__interface NKiller
{
	void killSomeone();
};
class NPlayer sealed : NDetective, NKiller{
public:
	void NDetective::makeSurvey();
	void killSomeone();
private:
	bool isDead = false;
};

using namespace std;

class PlayField {												//����
	public:
		//...

	private:
		//...

};
class Person {												//�������� (����, �������������, ��������)
	public:
		//...
	
	private:
		//...

};
class Player {												//�����
	public:
		//...

	private:
		//...

};
class R_Inspector {												//���� ����������(������)
	public:
		//...

	private:
		//...

};
class R_Bandit {												//���� �������
	public:
		//...

	private:
		//...

};


int main() {
<<<<<<< HEAD
	NDetective A = new NPlayer;
=======
	std::cout << "redone\n";
	std::cout << "Is it works?\n";
	std::cout << "You did not answer me...\n";
>>>>>>> 76b00dccd0971ec6878f839b118d8e1303dc30fb
	return 0;
}