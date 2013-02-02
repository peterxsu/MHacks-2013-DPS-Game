
class Player
{
private:
	int x, y;
	int health;
	int strength;
	int direction; //0 = North, 1 = East, 2 = South, 3 = West
	int cooldown; //cooldown time between shots in milliseconds

public:
	Player();
	Player(int, int, int, int, int, int);
	~Player();
	int getStrength() {return strength;}
	int getDirection() {return direction;}
	int getCooldown() {return cooldown;}
	int getHealth() {return health;}
	int getX() {return x;}
	int getY() {return y;}
	void takeHit(int damage) {health -= damage;}
	void move(int, int);
	void turn(int d) {direction = d;}
	void attack(); //calls a function in the world class that creates a bullet traveling in the right direction
};

