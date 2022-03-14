#pragma once

class Entity {
public:
	Entity();
	~Entity();

	void init(int x, int y, int width, int height, int speed);

	//Getters
	void getRect(int* posx, int* posy, int* w, int* h);
	int getX();
	int getY();
	int getWidth();
	int getHeight();

	//Existence
	void shutDown();
	bool isAlive();

	//Movement
	void move(int dx, int dy);
	void updateHeight(int dy);
	
private:
	int x, y;
	int width, height;
	int speed;
	bool is_alive;
};
