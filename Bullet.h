
class Bullet
{
private:
	int x, y;
	int strength;
	int direction;

public:
	Bullet();
	Bullet(int, int, int, int);
	~Bullet();
	int getStrength() {return strength;}
	void move();
};

