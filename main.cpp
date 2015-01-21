// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   main.cpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tmielcza <tmielcza@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/01/20 16:07:50 by tmielcza          #+#    #+#             //
//   Updated: 2015/01/21 20:51:14 by caupetit         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <SDL.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <cmath>
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

float		getHi(point a)
{
	float	teta = 0.5f;

	return (1.f / (a.dst + teta));
}

void		interPoint(Display& d, const float x, const float y)
{
	float	px = x;
	float	py = y;
	float	pz = 0;
	float	his[3];
	float	shi = 0;

//	float	a = 0.3f;

	point*	neighbours = getNeighbours(x, y);

	for (int i = 0 ; i < 3 ; i++)
	{
		his[i] = getHi(neighbours[i]);
		shi += his[i];
	}

	for (int i = 0 ; i < 3 ; i++)
	{
		pz += neighbours[i].z * (his[i] / shi);
	}

	d.addPixel(px, py + pz, 0xffffffff);
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
	for (int i = 0; i < 100 * 100; i += 2)
	{
		interPoint(d, i % 100, i / 100);
	}
}

int main(void)
{
	Display*	dis;

	try
	{
		dis = new Display();
	}
	catch (std::exception& e)
	{
		return (0);
	}
	(void)dis;

	_list.push_back(point(0.f, 0.f, 0.f));
	_list.push_back(point(0.f, 100.f, 0.f));
	_list.push_back(point(100.f, 100.f, 0.f));
	_list.push_back(point(100.f, 0.f, 0.f));
	getFile("test.mod1");

	displayMap(*dis);
	dis->draw();

	while (0);

	return (0);
}
