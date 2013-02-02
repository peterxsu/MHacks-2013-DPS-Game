
class Enemy
{
	int x, y, health, attack;

public:
	Enemy(int, int, int, int);
	~Enemy();
	void move(int, int);
	void fire();
	void takeHit(int);
};

