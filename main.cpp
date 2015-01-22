// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   main.cpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tmielcza <tmielcza@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/01/20 16:07:50 by tmielcza          #+#    #+#             //
//   Updated: 2015/01/22 14:48:35 by tmielcza         ###   ########.fr       //
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

struct point
{
	point(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {};
	point(void) {};

	float x, y, z, dst;

	static float	getDst(point a, point b)
		{
			float xi, yi;

			xi = a.x - b.x;
			yi = a.y - b.y;
			return (std::sqrt(xi * xi + yi * yi));
		};
};

static std::list<point> _list;

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

float		Weight(float d)
{
	if (d == 0)
		return (1);
	return (1.f / (d * d));
}

void		interPoint(Display& d, const float x, const float y)
{
	float	px = x;
	float	py = y;
	float	pz = 0;

//	float	a = 0.3f;
//
	float sumVal = 0;
	float sum = 0;
	float distance, weight, value;
	for (std::list<point>::const_iterator ite = _list.begin(), end = _list.end(); ite != end; ++ite)
	{
		distance = point::getDst(point(x, y, 0), *ite);
		weight = distance != 0 ? 1/ (distance * distance) : 1;
		value = weight * ite->z;
		sumVal += value;
		sum += weight;
		if (ite->z == 0)
			std::cout << value << " " << weight << std::endl;
	}

	sum += Weight(std::min(x, 100 - x));
	sum += Weight(std::min(y, 100 - y));

	pz = sumVal/sum;

	d.addPixel(px * 2 + 100, py * 2 - pz + 100, 0xffffffff);
}

void		getFile(std::string name)
{
	std::ifstream	fs(name.c_str());
	std::string	str;

	fs >> str;
	while (!str.empty())
	{
		std::cout << str << std::endl;

		char a, b, c, d;
		unsigned int x, y, z;
		std::stringstream ss(str);

		ss >> a >> x >> b >> y >> c >> z >> d;
		_list.push_back(point(x, y, z));

		std::cout << x << "-" << y << "-" << z << std::endl;

		str.clear();
		fs >> str;
	}
}

void	displayMap(Display& d)
{
	for (float j = 0; j < 100.f; j += 2.f)
	{
		for (float i = 0; i < 100.f; i += 2.f)
		{
			interPoint(d, i, j);
		}
	}
}

int main(int ac, char **av)
{
	Display*	dis;

	if (ac != 2)
	{
		return (1);
	}

	try
	{
		dis = new Display();
	}
	catch (std::exception& e)
	{
		return (0);
	}
//	(void)dis;

	_list.push_back(point(0.f, 0.f, 0.f));
	_list.push_back(point(0.f, 100.f, 0.f));
	_list.push_back(point(100.f, 100.f, 0.f));
	_list.push_back(point(100.f, 0.f, 0.f));

	getFile(av[1]);

	displayMap(*dis);
	dis->draw();

	SDL_Delay(5000);

	return (0);
}
