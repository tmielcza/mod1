// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   main.cpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tmielcza <tmielcza@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/01/20 16:07:50 by tmielcza          #+#    #+#             //
//   Updated: 2015/02/03 20:28:22 by tmielcza         ###   ########.fr       //
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

	int *tab = new int[CUBE_SIZE * CUBE_SIZE * (CUBE_SIZE / 2)];
	const std::vector< std::vector< std::vector <Map::voxel> > >&	voxs = map.voxels();
	int frames = 0;
	std::clock_t begin = clock();

	dis->setHeights(map.heights(), CUBE_SIZE, CUBE_SIZE);

	typedef void (Map::* waterModes)(void);
	waterModes wtm[5] = {&Map::rain, &Map::plane, &Map::wave, &Map::column, &Map::drain};

	int		waterMode = -1;
	int		quit = false;
	while (!quit)
	{
		SDL_Event		event;
		while (SDL_PollEvent(&event))
		{
			switch(event.type)
			{
			case SDL_MOUSEMOTION:
				if (event.motion.state & SDL_BUTTON_LMASK)
					dis->setCamRotation(event.motion.x, event.motion.y);
				break;
			case SDL_MOUSEWHEEL:
				dis->setZoom(event.motion.x * 0.005);
				break;
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym)
				{
				case SDLK_KP_PLUS:
					dis->setZoom(0.005);
					break;
				case SDLK_KP_MINUS:
					dis->setZoom(-0.005);
					break;
				case SDLK_ESCAPE:
					quit = true;
					break;
				case SDLK_1:
					waterMode = 0;
					break;
				case SDLK_2:
					waterMode = 1;
					break;
				case SDLK_3:
					waterMode = 2;
					break;
				case SDLK_4:
					waterMode = 3;
					break;
				case SDLK_5:
					waterMode = 4;
					break;
				case SDLK_SPACE:
					waterMode = -1;
					break;
				}
				break;
			}
		}
		
		map.setWaterHeight();
		if (waterMode >= 0)
			(map.*wtm[waterMode])();
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
//	SDL_Delay(500);
	}

	return (0);
}
