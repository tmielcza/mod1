// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Map.cpp                                            :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tmielcza <tmielcza@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/01/22 15:20:14 by tmielcza          #+#    #+#             //
//   Updated: 2015/01/22 19:21:16 by tmielcza         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <cstdlib>
#include <cmath>
#include <iostream>
#include "Map.hpp"

Map::Map(void)
{
	this->_pts = NULL;
}

Map::~Map(void)
{
	delete this->_pts;
}

void	Map::setPoints(std::list<point>* pts)
{
	delete this->_pts;
	this->_pts = pts;

	unsigned int	maxz = 0;
	unsigned int	maxy = 0;
	unsigned int	maxx = 0;
	unsigned int	miny = 0;
	unsigned int	minx = 0;

	unsigned int	maxsize, sizex, sizey;

	float			hRatio;

	for (std::list<point>::const_iterator it = pts->begin(), end = pts->end(); it != end; ++it)
	{
		if (it->z > maxz)
			maxz = it->z;
		if (it->y > maxy)
			maxy = it->y;
		else if (miny == 0 || it->y > miny)
			miny = it->y;
		if (it->x > maxx)
			maxx = it->x;
		else if (minx == 0 || it->x > minx)
			minx = it->x;
	}
	maxz = maxz / 2;
	sizex = maxx - minx;
	sizey = maxy - miny;

	maxsize = std::max(maxz, sizex);
	maxsize = std::max(maxsize, sizey);

	hRatio = static_cast<float>(maxsize) / CUBE_SIZE;

	this->_ratio = hRatio;
	this->_offsetx = minx;
	this->_offsety = miny;
}

Map::point::point(void)
{
}

Map::point::point(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
{
}

Map::point::point(const point& src)
{
	*this = src;
}

Map::point::~point(void)
{
}

Map::point&	Map::point::operator=(const point& rhs)
{
	this->x = rhs.x;
	this->y = rhs.y;
	this->z = rhs.z;
	this->dst = rhs.dst;
	return (*this);
}

float	Map::point::getDst(const point& a, const point& b)
{
	float xi, yi;

	xi = a.x - b.x;
	yi = a.y - b.y;
	return (std::sqrt(xi * xi + yi * yi));
}

float	Map::Weight(float d)
{
	if (d == 0)
		return (1);
	return (1.f / (d * d));
}

void	Map::voxelizeMap(void)
{
	point inter;

	for (int j = 0; j < CUBE_SIZE; j++)
	{
		for (int i = 0; i < CUBE_SIZE; i++)
		{
			inter = interPoint(i * this->_ratio,
							   j * this->_ratio);
			
			displayer->addPixel(30 + i * 2, 30 + j * 2 + inter.z / this->_ratio, 0xffffffff);
		 }
	 }
	displayer->draw();
 }

Display* Map::displayer; // A VIRER

 Map::point	Map::interPoint(const float x, const float y) const
 {
	 float	px = x;
	 float	py = y;
	 float	pz = 0;

	 float sumVal = 0;
	 float sum = 0;
	 float distance, weight, value;

	 for (std::list<point>::const_iterator ite = this->_pts->begin(), end = this->_pts->end(); ite != end; ++ite)
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

	return (point(px, py, pz));
}
