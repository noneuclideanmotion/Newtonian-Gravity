#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

struct Coordinate
{
	Coordinate() : x(0), y(0) {}
	Coordinate(double x_a, double y_a) : x(x_a), y(y_a) {}
	double x;
	double y;
};

struct Coordinate3D
{
	Coordinate3D() : x(0), y(0), z(0) {}
	Coordinate3D(double x_a, double y_a, double z_a) : x(x_a), y(y_a), z(z_a) {}
	double x;
	double y;
	double z;
};

namespace Logic
{
	const double G = 0.005;
	const int width = 1200;
	const int height = 800;
	double distance(Coordinate A, Coordinate B);
	double distance(Coordinate3D A, Coordinate3D B);
	struct Options
	{
		bool reverse = false;
		bool collision = false;
		bool gravity = false;
		bool boundingbox = false;
	};

}

struct Dust
{
	Dust(double x_a, double y_a, double m, double v_x, double v_y) : mass(m)
	{
		entity = sf::CircleShape(sqrt(m));
		entity.setOrigin(entity.getRadius(), entity.getRadius());
		position = Coordinate(x_a, y_a);
		velocity = Coordinate(v_x, v_y);
		
		entity.setFillColor(sf::Color(255, 255, 255));
	}
	double mass;
	Coordinate position;
	Coordinate velocity;
	sf::CircleShape entity;
	static void update(std::vector<Dust> &array, Logic::Options o)
	{
		int i = 0;
		int j = 0;
		for (auto &dust_a : array)
		{
			for (auto &dust_b : array)
			{
				if (!(&dust_a == &dust_b))
				{
					double distance = Logic::distance(dust_a.position, dust_b.position);
					if (o.gravity)
					{
						dust_a.velocity.x += (!o.reverse * 2 - 1)*Logic::G*((dust_b.position.x - dust_a.position.x)*dust_b.mass) / (distance*distance);
						dust_a.velocity.y += (!o.reverse * 2 - 1)*Logic::G*((dust_b.position.y - dust_a.position.y)*dust_b.mass) / (distance*distance);
					}
					if (o.collision)
					{
						if (distance <= dust_a.entity.getRadius() + dust_b.entity.getRadius())
						{
							if (i < j)
							{
								double velax = dust_a.velocity.x;
								double velbx = dust_b.velocity.x;
								double velay = dust_a.velocity.y;
								double velby = dust_b.velocity.y;
								dust_a.velocity.x = (velbx - velax)*(dust_b.mass / (dust_a.mass + dust_b.mass));
								dust_a.velocity.y = (velby - velay)*(dust_b.mass / (dust_a.mass + dust_b.mass));
								dust_b.velocity.x = (velax - velbx)*(dust_a.mass / (dust_a.mass + dust_b.mass));
								dust_b.velocity.y = (velay - velby)*(dust_a.mass / (dust_a.mass + dust_b.mass));
							}
						}
					}
				}
				++j;
			}
			if (o.boundingbox)
			{
				if (dust_a.position.x < 0 || dust_a.position.x > Logic::width)
				{
					dust_a.velocity.x = -dust_a.velocity.x;
				}
				if (dust_a.position.y < 0 || dust_a.position.y > Logic::height)
				{
					dust_a.velocity.y = -dust_a.velocity.y;
				}
			}
			j = 0;
			++i;
		}
		for (auto &dust_a : array)
		{
			dust_a.position.x += (!o.reverse*2-1)*dust_a.velocity.x;
			dust_a.position.y += (!o.reverse*2-1)*dust_a.velocity.y;
		}
	}
	static void draw(sf::RenderWindow &window, std::vector<Dust> array)
	{
		for (auto &dust : array)
		{
			dust.entity.setPosition(dust.position.x, dust.position.y);
			window.draw(dust.entity);
		}
	}
};

#include "Project.h"

struct Dust3D
{
	Dust3D(double x_a, double y_a, double z_a, double m, double v_x, double v_y, double v_z, Project::Camera c) : mass(m)
	{
		 
		
		position = Coordinate3D(x_a, y_a, z_a);
		velocity = Coordinate3D(v_x, v_y, v_z);
		entity.connections = Project::Shape::Cube::connections;
		entity.points = Project::Shape::Cube::points;
		entity.move(sf::Vector3f(-0.008*(x_a+c.x-Logic::width/2)-1.5, -0.008*(y_a-+c.y-Logic::height/2)-1.5, z_a));
	}
	double mass;
	Coordinate3D position;
	Coordinate3D velocity;
	Project::Object entity;
	static void update(std::vector<Dust3D> &array, Logic::Options o)
	{
		int i = 0;
		int j = 0;
		for (auto &dust_a : array)
		{
			for (auto &dust_b : array)
			{
				if (!(&dust_a == &dust_b))
				{
					double distance = Logic::distance(dust_a.position, dust_b.position);
					if (o.gravity)
					{
						dust_a.velocity.x += (!o.reverse * 2 - 1)*Logic::G*((dust_b.position.x - dust_a.position.x)*dust_b.mass) / (distance*distance);
						dust_a.velocity.y += (!o.reverse * 2 - 1)*Logic::G*((dust_b.position.y - dust_a.position.y)*dust_b.mass) / (distance*distance);
						dust_a.velocity.z += (!o.reverse * 2 - 1)*Logic::G*((dust_b.position.z - dust_a.position.z)*dust_b.mass) / (distance*distance);
					}
					if (o.collision)
					{
						//3D collision TBA
					}
				}
				++j;
			}
			if (o.boundingbox)
			{
				//3D BB tba
			}
			j = 0;
			++i;
		}
		for (auto &dust_a : array)
		{
			dust_a.position.x += (!o.reverse * 2 - 1)*dust_a.velocity.x;
			dust_a.position.y += (!o.reverse * 2 - 1)*dust_a.velocity.y;
			dust_a.position.z += (!o.reverse * 2 - 1)*dust_a.velocity.z;
			dust_a.entity.move(sf::Vector3f(dust_a.velocity.x, dust_a.velocity.y, dust_a.velocity.z));
		}
	}
};
