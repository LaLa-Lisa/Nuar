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

int main() {
	NDetective A = new NPlayer;
	return 0;
}