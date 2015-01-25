// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Raycast.cpp                                        :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: caupetit <marvin@42.fr>                    +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/01/23 14:59:29 by caupetit          #+#    #+#             //
//   Updated: 2015/01/25 15:08:09 by tmielcza         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "Raycast.hpp"

/* Constructors / Destructors */
Raycast::Raycast (const Map::point& p,const int& w,const int& h, const float& zoom):
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

	this->_rightDir = this->_rightDir * zoom;
	this->_upDir = this->_upDir * zoom;
	std::cout << "updir for init: " << _upDir << std::endl;
	std::cout << "rightDir for init: " << _rightDir << std::endl;
	float	H = this->_h / 2;
	float	W = this->_w / 2;
	_upLeft = _camPos + _upDir * H - _rightDir * W;
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
	Map::point	p;
	float		dist = 0;
	float		shortestDist = 0;

	if (ray.dir.x != 0)
	{
		firstx.x = ray.dir.x < 0 ? std::floor(ray.origin.x) : std::ceil(ray.origin.x);
		diffx = firstx.x - ray.origin.x;
		if (diffx != 0)
		{
			ratio = diffx / ray.dir.x;
			ray.dir = ray.dir * ratio;
			firstx = ray.origin + ray.dir;
		}
		ratio = std::abs(1 / ray.dir.x);
		ray.dir = ray.dir * ratio;
		size = std::abs(ray.origin.x) + CUBE_SIZE;
		for (diffx = 0 ; diffx < size ; diffx++)
		{
			firstx = firstx + ray.dir;
			p.x = firstx.x;
			p.y = std::floor(firstx.y);
			p.z = std::floor(firstx.z);
			if (p.x >= 0 && p.x < CUBE_SIZE
				&& p.y >= 0 && p.y < CUBE_SIZE
				&& p.z >= 0 && p.z < CUBE_SIZE / 2)
			{
				if (map[p.z][p.y][p.x].type != Map::voxel::VOID)
				{
					dist = std::sqrt((p.x - ray.origin.x) * (p.x - ray.origin.x) +
									 (p.y - ray.origin.y) * (p.y - ray.origin.y) +
									 (p.z - ray.origin.z) * (p.x - ray.origin.z));
					shortestDist = dist;
					vox = map[p.z][p.y][p.x];
					break;
				}
			}
		}
	}

	if (ray.dir.y != 0)
	{
		firstx.y = ray.dir.y < 0 ? std::floor(ray.origin.y) : std::ceil(ray.origin.y);
		diffx = firstx.y - ray.origin.y;
		if (diffx != 0)
		{
			ratio = diffx / ray.dir.y;
			ray.dir = ray.dir * ratio;
			firstx = ray.origin + ray.dir;
		}
		ratio = std::abs(1 / ray.dir.y);
		ray.dir = ray.dir * ratio;
		size = std::abs(ray.origin.y) + CUBE_SIZE;
		for (diffx = 0 ; diffx < size ; diffx++)
		{
			firstx = firstx + ray.dir;
			p.x = std::floor(firstx.x);
			p.y = std::floor(firstx.y);
			p.z = std::floor(firstx.z);
			if (p.x >= 0 && p.x < CUBE_SIZE
				&& p.y >= 0 && p.y < CUBE_SIZE
				&& p.z >= 0 && p.z < CUBE_SIZE / 2)
			{
				if (map[p.z][p.y][p.x].type != Map::voxel::VOID)
				{
					dist = std::sqrt((p.x - ray.origin.x) * (p.x - ray.origin.x) +
									 (p.y - ray.origin.y) * (p.y - ray.origin.y) +
									 (p.z - ray.origin.z) * (p.x - ray.origin.z));
					if (shortestDist == 0 || dist < shortestDist)
					{
						shortestDist = dist;
						vox = map[p.z][p.y][p.x];
					}
					break;
				}
			}
		}
	}

if (ray.dir.z != 0)
	{
		firstx.z = ray.dir.z < 0 ? std::floor(ray.origin.z) : std::ceil(ray.origin.z);
		diffx = firstx.z - ray.origin.z;
		if (diffx != 0)
		{
			ratio = diffx / ray.dir.z;
			ray.dir = ray.dir * ratio;
			firstx = ray.origin + ray.dir;
		}
		ratio = std::abs(1 / ray.dir.z);
		ray.dir = ray.dir * ratio;
		size = std::abs(ray.origin.z) + CUBE_SIZE;
		for (diffx = 0 ; diffx < size ; diffx++)
		{
			firstx = firstx + ray.dir;
			p.x = std::floor(firstx.x);
			p.y = std::floor(firstx.y);
			p.z = std::floor(firstx.z);
			if (p.x >= 0 && p.x < CUBE_SIZE
				&& p.y >= 0 && p.y < CUBE_SIZE
				&& p.z >= 0 && p.z < CUBE_SIZE / 2)
			{
				if (map[p.z][p.y][p.x].type != Map::voxel::VOID)
				{
					dist = std::sqrt((p.x - ray.origin.x) * (p.x - ray.origin.x) +
									 (p.y - ray.origin.y) * (p.y - ray.origin.y) +
									 (p.z - ray.origin.z) * (p.x - ray.origin.z));
					if (shortestDist == 0 || dist < shortestDist)
					{
						shortestDist = dist;
						vox = map[p.z][p.y][p.x];
					}
					break;
				}
			}
		}
	}
	if (shortestDist)
		return true;
	else
		return false;
}

void	Raycast::raycastMapVoxels(const Map& map, Display & display)
{
	Map::voxel	current;
	std::vector<std::vector<std::vector<Map::voxel> > >	voxels = map.voxels();
	Map::point	origin = this->_upLeft;
	Map::point	test;

	for (int i = 0 ; i < this->_w ; i++)
	{
		test = origin;
		for (int j = 0 ; j < this->_h ; j++)
		{
			if (raycast(current, voxels, Raycast::Ray(origin, this->_dir)))
			{
				if (current.type == Map::voxel::WATER)
					display.addPixel(i, j, 0x000000FF);
				else
					display.addPixel(i, j, 0xFFFFFF00 + current.q * 2);
			}
			origin = origin - this->_upDir;
		}
		origin = test + this->_rightDir;
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
