#include "Entity.h"

Entity::Entity() {
	is_alive = false;
}

Entity::~Entity() {}

void Entity::init(int x, int y, int width, int height, int speed) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->speed = speed;
	is_alive = true;
}

void Entity::getRect(int* posx, int* posy, int* w, int* h) {
	*posx = x;
	*posy = y;
	*w = width;
	*h = height;
}

int Entity::getX() {
	return x;
}

int Entity::getY() {
	return y;
}

int Entity::getWidth() {
	return width;
}

int Entity::getHeight() {
	return height;
}

void Entity::shutDown() {
	is_alive = false;
}

bool Entity::isAlive() {
	return is_alive;
}

void Entity::move(int dx, int dy) {
	x += dx * speed;
	y += dy * speed;
}