#include "Project.h"
#include <vector>
#include <SFML\Graphics.hpp>
#include <map>
#include <iostream>

namespace Project
{

	std::vector<std::vector<sf::Vertex>> Project(Project::Object o, Project::Camera c)
	{
		//Vector to return
		std::vector<std::vector<sf::Vertex>> linesToDraw;

		//Variables defined sto make the calculations easier to understand

		for (const auto& connection : o.connections)
		{
			sf::Vertex lineCoord[2];
			auto iterator1 = o.points.find(connection.first);
			auto iterator2 = o.points.find(connection.second);

			//if both points on the connection are valid
			if (iterator1 != o.points.end() && iterator2 != o.points.end())
			{
				lineCoord[0].position = convert3Dto2D(iterator1->second, c);
				lineCoord[1].position = convert3Dto2D(iterator2->second, c);
				//std::cout << lineCoord[0].position.x << " " << lineCoord[0].position.y << std::endl;
			}
			linesToDraw.push_back(std::vector<sf::Vertex>());
			linesToDraw.back().push_back(lineCoord[0]);
			linesToDraw.back().push_back(lineCoord[1]);
		}
		return linesToDraw;
	}



	sf::Vector2f convert3Dto2D(const sf::Vector3f& point, const Project::Camera& c)
	{
		const float view_x = c.view_x;
		const float view_y = c.view_y;
		const float view_z = c.view_z;

		const float cos_y = cos(c.pitch);
		const float cos_x = cos(c.roll);
		const float cos_z = cos(c.yaw);

		const float sin_y = sin(c.pitch);
		const float sin_x = sin(c.roll);
		const float sin_z = sin(c.yaw);

		const float x = point.x - c.x;
		const float y = point.y - c.y;
		const float z = point.z - c.z;
		const float d_x = cos_y*(sin_z*y + cos_z*x) - sin_y*z;
		const float d_y = sin_x*(cos_y*z + sin_y*(sin_z*y + cos_z*x)) + cos_x*(cos_z*y - sin_z*x);
		const float d_z = cos_x*(cos_y*z + sin_y*(sin_z*y + cos_z*x)) - sin_x*(cos_z*y - sin_z*x);

		float final_x = view_z*d_x / d_z - view_x;
		float final_y = view_z*d_y / d_z - view_y;
		return sf::Vector2f(final_x, final_y);
	}
	void Object::move(sf::Vector3f relativeMovement)
	{
		for (auto&point : points)
		{
			point.second += relativeMovement;
		}
	}
	sf::Vector3f Object::position()
	{
		return sf::Vector3f(points[0].x, points[0].y, points[0].z);
	}
}
