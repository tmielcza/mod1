// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   main.cpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tmielcza <tmielcza@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/01/20 16:07:50 by tmielcza          #+#    #+#             //
//   Updated: 2015/01/25 15:12:29 by tmielcza         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <SDL.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <cmath>
#include <algorithm>
#include "Display.hpp"
#include "Map.hpp"
#include "Raycast.hpp"

/*
point*		getNeighbours(float x, float y)
{
	point*	nei = new point[3];
	point	tmp(x, y, 0);

	for (std::list<point>::const_iterator ite = _list.begin(), end = _list.end(); ite != end; ++ite)
	{
		float dst = point::getDst(tmp, *ite);
		for (int i = 0; i < 3; i++)
		{
			if (nei[i].dst == 0 || nei[i].dst > dst)
			{
				nei[i] = *ite;
				nei[i].dst = dst;
			}
		}
	}
	return (nei);
}
*/

std::list<Map::point>*	getFile(std::string name)
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


	std::list<Map::point>* pts = getFile(av[1]);

	Map	map;

	map.setPoints(pts);
	map.voxelizeMap();

	Raycast		raycast(Map::point(100, 0, 50), 640, 480, 0.2f);

/*
	for (int i = 0; i < 100; i++)
	{
		for (int j = 10; j < 12; j++)
			map.PutWater(i, 1, j);
	}
*/

	for (int i = 0; i < 300; i++)
	{
		for (int j = 40; j < 80; j++)
			map.PutWater(j, 1, 3);
		for (int j = 0; j < 10; j++)
			map.drainWoxels();
		raycast.raycastMapVoxels(map, *dis);
		dis->draw();
	}

	SDL_Delay(14000);

	return (0);
}
