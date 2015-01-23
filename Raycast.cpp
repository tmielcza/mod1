// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Raycast.cpp                                        :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: caupetit <marvin@42.fr>                    +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/01/23 14:59:29 by caupetit          #+#    #+#             //
//   Updated: 2015/01/23 21:56:24 by caupetit         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "Raycast.hpp"

/* Constructors / Destructors */
Raycast::Raycast (const Map::point& p,const int& w,const int& h):
	_camPos(p),
	_camDir(Map::point(0, 0, 0)),
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
	float	H = this->_h / 2;
	float	W = this->_w / 2;
	_upLeft = Map::point(_camPos.x + (_upDir.x * H) - (_rightDir.x * W),
						 _camPos.y + (_upDir.y * H) - (_rightDir.y * W),
						 _camPos.z + (_upDir.z * H) - (_rightDir.z * W));
	this->_upDir = this->_upDir / this->_h;
	this->_rightDir = this->_rightDir / this->_w;
}

Raycast::~Raycast (void)
{
}

bool	Raycast::raycast(Map::voxel& vox,
						 const std::vector<std::vector<std::vector<Map::voxel> > >& map,
						 Raycast::Ray ray)
{
	Map::point	firstx;
	Map::point	firsty;
	Map::point	firstz;

	firstx.x = ray.dir.x < 0 ? std::floor(ray.origin.x) : std::ceil(ray.origin.x);
//	float ratio = std::abs(ray.origin.x - firstx.x);

//	ray.dir.x = 

//	firsty.y = ray.dir.y < 0 = std::floor(ray.origin.y) : std::ceil(ray.origin.y);
//	firstz.y = ray.dir.z < 0 = std::floor(ray.origin.z) : std::ceil(ray.origin.z);

	(void)vox;
	(void)map;
	return true;
}


void	Raycast::raycastMapVoxels(const Map& map, const Display & display)
{
	Map::voxel	current;
	std::vector<std::vector<std::vector<Map::voxel> > >	voxels = map.voxels();
	Map::point	origin = this->_upLeft;

	for (int i = 0 ; i < this->_w ; i++)
	{
		for (int j = 0 ; j < this->_h ; i++)
		{
			raycast(current, voxels, Raycast::Ray(origin, this->_dir));
			origin = origin + this->_rightDir;
		}
		origin = origin - this->_upDir;
	}
	(void)display;
}

Raycast::Ray::Ray(Map::point o, Map::point d) : origin(o), dir(d)
{
}

Raycast::Ray::~Ray(void)
{
}

/* Member Functions */
