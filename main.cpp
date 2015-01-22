// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   main.cpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tmielcza <tmielcza@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/01/20 16:07:50 by tmielcza          #+#    #+#             //
//   Updated: 2015/01/22 19:27:58 by tmielcza         ###   ########.fr       //
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

		std::cout << x << "-" << y << "-" << z << std::endl;

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

	Map::displayer = dis;

//	displayMap(*dis);
//	dis->draw();

	SDL_Delay(5000);

	return (0);
}
