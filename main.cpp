// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   main.cpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tmielcza <tmielcza@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/01/20 16:07:50 by tmielcza          #+#    #+#             //
//   Updated: 2015/01/31 17:36:37 by tmielcza         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <SDL.h>
#include <OpenGL/gl.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <cmath>
#include <algorithm>
#include "Display.hpp"
#include "Map.hpp"
#include "Raycast.hpp"

#include <ctime>

std::list<Map::point>*	getMod1File(std::string name)
{
	std::ifstream	fs(name.c_str());
	std::string	str;
	std::list<Map::point>* pts = new std::list<Map::point>();

	fs >> str;
	while (!str.empty())
	{
		std::cout << str << std::endl;

		char a, b, c, d;
		unsigned int x, y, z;
		std::stringstream ss(str);

		ss >> a >> x >> b >> y >> c >> z >> d;
		pts->push_back(Map::point(x, y, z));

		std::cout << "pt " << x << "-" << y << "-" << z << std::endl;

		str.clear();
		fs >> str;
	}

	return (pts);
}

int		main(int ac, char **av)
{
	if (ac != 2)
	{
		return (1);
	}

	Display*	dis;

	try
	{
		dis = new Display();
	}
	catch (std::exception& e)
	{
		return (0);
	}


	std::list<Map::point>* pts = getMod1File(av[1]);

	Map	map;

	map.setPoints(pts);
	map.voxelizeMap();

	dis->initShaderProgram("raycast.frag");

	glClearColor(1,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT);

	glViewport(0, 0, dis->getW(), dis->getH());
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 1, 0, 1, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	int *tab = new int[CUBE_SIZE * CUBE_SIZE * (CUBE_SIZE / 2)];
	const std::vector< std::vector< std::vector <Map::voxel> > >&	voxs = map.voxels();
	int frames = 0;
	std::clock_t begin = clock();

	dis->setHeights(map.heights(), CUBE_SIZE, CUBE_SIZE);

	while (1)
	{
		SDL_Event		event;
		SDL_PollEvent(&event);
		switch(event.type)
		{
		case SDL_MOUSEMOTION:
			if (event.motion.state & SDL_BUTTON_LMASK)
			{
				dis->setCamRotation(event.motion.x, event.motion.y);
				std::cout << "BUTTON PRESSED" << std::endl;
				std::cout << "Mouse motion: " << event.motion.x << " " << event.motion.y << std::endl;
			}
			break;
		}

		for (int i = 20; i < 30; i++)
			map.PutWater(i, 100, 40);
		map.drainWoxels();

		for (int i = 0; i < CUBE_SIZE * CUBE_SIZE * (CUBE_SIZE / 2); i++)
		{
			tab[i] = *reinterpret_cast<const int*>(&voxs[i / CUBE_SIZE / CUBE_SIZE][i / CUBE_SIZE % CUBE_SIZE][i % CUBE_SIZE % CUBE_SIZE]);
		}
		dis->rotateCam();
		dis->draw(reinterpret_cast<const void*>(tab), CUBE_SIZE, CUBE_SIZE, CUBE_SIZE / 2);

		frames++;

		if (clock() - begin >= CLOCKS_PER_SEC)
		{
			std::cout << frames << std::endl;
			frames = 0;
			begin = clock();
		}
	}
	SDL_Delay(15000);

	return (0);
}
