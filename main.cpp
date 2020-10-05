#include <SFML\Graphics.hpp>
#include "Logic.h"
#include <iostream>
#include "Project.h"

int main()
{
	bool exit = false;
	bool alreadyclick = false;
	sf::Vertex line[2];
	line[0].position = sf::Vector2f(0, 0);
	line[0].color = sf::Color::Red;
	line[1].position = sf::Vector2f(0, 0);
	line[1].color = sf::Color::Red;
	sf::CircleShape ghost;
	ghost.setFillColor(sf::Color(255, 255, 255,40));
	ghost.setRadius(4);
	ghost.setOrigin(ghost.getRadius(), ghost.getRadius());
	sf::RenderWindow window(sf::VideoMode(Logic::width, Logic::height), "Gravity");
	sf::Event event;
	sf::Clock clock;
	std::size_t framecount;
	std::vector<Dust> masses;
	window.setFramerateLimit(60);
	bool pause = false;;
	bool framemode = false;
	Logic::Options op;
	op.collision = true;
	op.gravity = true;
	op.boundingbox = false;
	bool mouselock = false;

	//3D stuff
	bool mouserotate = false;
	std::vector<Dust3D> objects;
	Project::Camera camera;

	camera.view_x = -Logic::width/2;
	camera.view_y = -Logic::height/2;
	camera.view_z = 10;
	float deltaZ = 0;


	camera.x = 0; camera.y = 0; camera.z = -20;
	camera.yaw = 0; camera.pitch = 0; camera.roll = -0.01;
	bool TDMode = false;
	//start
	while (!exit)
	{
		if (alreadyclick)
		{
			line[1].position = sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
			window.draw(line, 2, sf::Lines);
			window.draw(ghost);

		}
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
				// window closed
			case sf::Event::Closed:
				window.close();
				exit = true;
				break;
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Escape)
				{
					window.close();
					exit = true;
				}
				else if (event.key.code == sf::Keyboard::Space)
				{
					if (framemode)
					{
						framemode = false;
						pause = false;
						op.reverse = false;
					}
					else
					{
						pause = !pause;
					}
				}
				else if (event.key.code == sf::Keyboard::Delete)
				{
					if (!TDMode)
					{
						masses.pop_back();
					}
					else
					{
						objects.pop_back();
					}
				}
				else if (event.key.code == sf::Keyboard::End)
				{
					if (!TDMode)
					{
						masses.clear();
					}
					else
					{
						objects.clear();
					}
				}
				else if (event.key.code == sf::Keyboard::Equal)
				{
					op.reverse = false;
				}
				else if (event.key.code == sf::Keyboard::Dash)
				{
					op.reverse = true;
				}
				else if (event.key.code == sf::Keyboard::Left)
				{
					op.reverse = true;
					if (TDMode)
					{
						Dust3D::update(objects, op);
					}
					else
					{
						Dust::update(masses, op);
					}
					framemode = true;
				}
				else if (event.key.code == sf::Keyboard::Right)
				{
					op.reverse = false;
					if (TDMode)
					{
						Dust3D::update(objects, op);
					}
					else
					{
						Dust::update(masses, op);
					}
					framemode = true;
				}
				else if (event.key.code == sf::Keyboard::Up)
				{
					if (TDMode)
					{
						camera.z-=3;
					}
				}
				else if (event.key.code == sf::Keyboard::Down)
				{
					if (TDMode)
					{
						camera.z+=3;
					}
				}
				else if (event.key.code == sf::Keyboard::G)
				{
					op.gravity = !op.gravity;
				}
				else if (event.key.code == sf::Keyboard::C)
				{
					op.collision = !op.collision;
				}
				else if (event.key.code == sf::Keyboard::B)
				{
					op.boundingbox = !op.boundingbox;
				}
				else if (event.key.code == sf::Keyboard::R)
				{
					mouserotate = !mouserotate;
				}
				else if (event.key.code == sf::Keyboard::T)
				{
					TDMode = !TDMode;
					masses.clear();
					objects.clear();
				}
				else if (event.key.code == sf::Keyboard::L)
				{
					mouselock = !mouselock;
				}
				break;

			case sf::Event::MouseButtonPressed:
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					if(TDMode)
					{ 
						if (!alreadyclick)
						{
							line[0].position = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
							ghost.setPosition(line[0].position.x, line[0].position.y);
							deltaZ = camera.z;
							alreadyclick = true;
						}
						else
						{
							alreadyclick = false;
							objects.push_back(Dust3D(line[0].position.x, line[0].position.y, camera.z+10 , ghost.getRadius()*ghost.getRadius(), //mass and position
								(0.005)*(line[0].position.x - line[1].position.x), (0.005)*(line[0].position.y - line[1].position.y), (0.05)*(camera.z-deltaZ),camera)); // velocity
							std::cout << "Particle created: " << event.mouseButton.x << " " << event.mouseButton.y << camera.z-5 << std::endl;
							deltaZ = 0;
						}
					}
					else
					{
						std::cout << "Particle created: " << event.mouseButton.x << " " << event.mouseButton.y << std::endl;
						if (!alreadyclick)
						{
							line[0].position = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
							ghost.setPosition(line[0].position.x, line[0].position.y);
							alreadyclick = true;
						}
						else
						{
							alreadyclick = false;
							masses.push_back(Dust(line[0].position.x, line[0].position.y, ghost.getRadius()*ghost.getRadius(), //mass and position
								(0.02)*(line[0].position.x - line[1].position.x), (0.02)*(line[0].position.y - line[1].position.y))); // velocity
						}
					}
				}
				break;
			case sf::Event::MouseWheelMoved:
				ghost.setRadius(ghost.getRadius() + event.mouseWheel.delta);
				ghost.setOrigin(ghost.getRadius(), ghost.getRadius());
				ghost.setPosition(line[0].position.x, line[0].position.y);
				break;

			default:
				break;
			}
		}
		if (!pause&&!framemode)
		{
			if (TDMode)
			{
				Dust3D::update(objects, op);
			}
			else
			{
				Dust::update(masses, op);
			}

		}
		if (TDMode)
		{
			for (auto& o : objects)
			{
				for (const auto& lines : Project::Project(o.entity, camera))
				{
					if (o.entity.position().z > camera.z)
					{
						window.draw(&lines[0], lines.size(), sf::Lines);
					}
				}
			}
		}
		else
		{
			Dust::draw(window, masses);
		}
		if (mouselock)
		{
			if (!mouserotate)
			{
				camera.x += (sf::Mouse::getPosition(window).x - ((int)Logic::width / 2))*0.001;
				camera.y += (sf::Mouse::getPosition(window).y - ((int)Logic::height / 2))*0.001;
			}
			else
			{
				camera.pitch += (sf::Mouse::getPosition(window).x - ((int)Logic::width / 2))*0.001;
				camera.roll += (sf::Mouse::getPosition(window).y - ((int)Logic::height / 2))*0.001;
			}
			sf::Mouse::setPosition(sf::Vector2i((int)Logic::width, (int)Logic::height)/2, window);
		}
		window.display();
		window.clear(sf::Color::Black);
	}
	return 0;
}