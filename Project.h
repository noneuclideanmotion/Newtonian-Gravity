
#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

namespace Project
{
	namespace Shape
	{
		namespace Cube
		{
			const std::map<int, sf::Vector3f> points =
			{
				{ 0, sf::Vector3f(0,0,0) },
				{ 1, sf::Vector3f(1,0,0) },
				{ 2, sf::Vector3f(1,1,0) },
				{ 3, sf::Vector3f(0,1,0) },
				{ 4, sf::Vector3f(0,0,1) },
				{ 5, sf::Vector3f(1,0,1) },
				{ 6, sf::Vector3f(1,1,1) },
				{ 7, sf::Vector3f(0,1,1) },
			};

			const std::vector<std::pair<int, int>> connections =
			{
				{ 0,1 },{ 0,3 },{ 0,4 },
				{ 1,2 },{ 1,5 },
				{ 2,3 },{ 2,6 },
				{ 7,4 },{ 7,3 },{ 7,6 },
				{ 5,4 },{ 5,6 }
			};

		}
	}

	struct Camera
	{
		float x; float y; float z;
		float view_x; float view_y; float view_z;
		float yaw; float pitch; float roll;
	};

	struct Object
	{
		//Vector of point coordinates and their ID's(Should be incremental)
		std::map<int, sf::Vector3f> points;

		//Pairs of point IDs which should have a line drawn between them
		std::vector<std::pair<int, int>> connections;

		//Move, with P1 going 
		void move(sf::Vector3f p1Pos);
		sf::Vector3f position();
	};

	std::vector<std::vector<sf::Vertex>> Project(Project::Object o, Project::Camera c);

	sf::Vector2f convert3Dto2D(const sf::Vector3f& v, const Project::Camera& c);
}