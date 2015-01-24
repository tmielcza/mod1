// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Raycast.cpp                                        :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: caupetit <marvin@42.fr>                    +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/01/23 14:59:29 by caupetit          #+#    #+#             //
//   Updated: 2015/01/24 15:55:19 by caupetit         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "Raycast.hpp"

/* Constructors / Destructors */
Raycast::Raycast (const Map::point& p,const int& w,const int& h):
	_camPos(p),
	_camDir(Map::point(CUBE_SIZE / 2, CUBE_SIZE / 2, 0)),
	_w(w),
	_h(h),
	_upDir(Map::point(0, 0, 1))
{
	this->_dir = Map::point(_camDir.x - _camPos.x,
							_camDir.y - _camPos.y,
							_camDir.z - _camPos.z);
	this->_dir.normalize();
	this->_rightDir = Map::point::cross(this->_upDir, this->_dir);
	this->_upDir    = Map::point::cross(this->_dir, this->_rightDir);
	std::cout << "updir for init: " << _upDir << std::endl;
	std::cout << "rightDir for init: " << _rightDir << std::endl;
	float	H = this->_h / 2;
	float	W = this->_w / 2;
	_upLeft = Map::point(_camPos.x + (_upDir.x * H) - (_rightDir.x * W),
						 _camPos.y + (_upDir.y * H) - (_rightDir.y * W),
						 _camPos.z + (_upDir.z * H) - (_rightDir.z * W));
	/* Not needed since screen size == plane size */
//	this->_upDir = this->_upDir / this->_h;
//	this->_rightDir = this->_rightDir / this->_w;
}

Raycast::~Raycast (void)
{
}

bool	Raycast::raycast(Map::voxel& vox,
						 const std::vector<std::vector<std::vector<Map::voxel> > >& map,
						 Raycast::Ray ray)
{
	Map::point	firstx;
	float		diffx;
	float		ratio;
	int			size;
	float		x;
	float		y;
	float		z;

//	std::cout << std::endl << "Ray dir: " << ray.dir << std::endl
//			  << "Ray.origin: " << ray.origin << std::endl;

	if (ray.dir.x != 0)
	{
		firstx.x = ray.dir.x < 0 ? std::ceil(ray.origin.x) : std::floor(ray.origin.x);
		diffx = firstx.x - ray.origin.x;
		if (diffx != 0)
		{
			ratio = diffx / ray.dir.x;
			ray.dir = ray.dir * ratio;
			firstx = ray.origin + ray.dir;
		}
		ratio = 1 / ray.dir.x;
		ray.dir = ray.dir * ratio;

		size = std::abs(ray.origin.x) + CUBE_SIZE;
//		std::cout << "Size: " << size << std::endl; 
		for (diffx = 0 ; diffx < size ; diffx++)
		{
			firstx = ray.origin + ray.dir;
			x = firstx.x;
			y = ray.dir.y < 0 ? std::ceil(firstx.y) : std::floor(firstx.y);
			z = ray.dir.z < 0 ? std::ceil(firstx.z) : std::floor(firstx.z);
			(void)x;
			(void)y;
			(void)z;
//			std::cout << "X: " << x << " Y: " << y << " Z: " << z << std::endl;
			if (x > 0 && x < CUBE_SIZE && y > 0 && y < CUBE_SIZE && z > 0 && z < CUBE_SIZE / 2)
				(void)map;
//				std::cout << map[z][y][x].type << std::endl;
		}
//		std::cout << "End boucle: " << diffx << std::endl;
	}


	(void)vox;
	(void)map;
	return true;
}


void	Raycast::raycastMapVoxels(const Map& map, const Display & display)
{
	Map::voxel	current;
	std::vector<std::vector<std::vector<Map::voxel> > >	voxels = map.voxels();
	Map::point	origin = this->_upLeft;
	Map::point	test;

	std::cout << "SCREEN " << this->_w << "  " << _h << std::endl;

	test = origin;
	for (int i = 0 ; i < this->_w ; i++)
	{
		for (int j = 0 ; j < this->_h ; j++)
		{
//			std::cout << "origin: " << origin << std::endl;
			raycast(current, voxels, Raycast::Ray(origin, this->_dir));
			origin = origin + this->_rightDir;
		}
		origin = test - this->_upDir;
//		origin = origin - this->_upDir;
//		std::cout << "One col" << std::endl;
	}
	std::cout << "OUttT" << std::endl;
	(void)display;
}

Raycast::Ray::Ray(Map::point o, Map::point d) : origin(o), dir(d)
{
}

Raycast::Ray::~Ray(void)
{
}

/* Member Functions */
