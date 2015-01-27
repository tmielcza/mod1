// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Raycast.cpp                                        :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: caupetit <marvin@42.fr>                    +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/01/23 14:59:29 by caupetit          #+#    #+#             //
//   Updated: 2015/01/27 14:11:43 by caupetit         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "Raycast.hpp"

//Map::point	Raycast::getFirstPoint(Ray ray, coord c);
//Map::point	Raycast::raycastInMap(vox, map, ray, firstpoint, size);
//bool			isInMap(Map::point);

/* Constructors / Destructors */
Raycast::Raycast (const Map::point& p, Display& d,
				  const std::vector<std::vector<std::vector<Map::voxel> > >& m):
	_map(m),
	_display(d),
	_camPos(p),
	_camDir(CUBE_SIZE / 2, CUBE_SIZE / 2, 0),
	_w(d.getW()),
	_h(d.getH()),
	_upDir(Map::point(0, 0, 1)),
	_zoom(0.5),
	_shortestDist(0),
	_dist(0)
{
	this->generateCam();
}

void	Raycast::raycastMapVoxels(void)
{
	Map::voxel	current;
	Map::point	origin = this->_upLeft;
	Map::point	tmp;

	for (int i = 0 ; i < this->_w ; i++)
	{
		tmp = origin;
		for (int j = 0 ; j < this->_h ; j++)
		{
			current.type = Map::voxel::VOID;
			if (raycast(current, Raycast::Ray(origin, this->_dir)))
			{
				if (current.type == Map::voxel::WATER)
					this->_display.addPixel(i, j, 0x000000FF);
				else
					this->_display.addPixel(i, j, 0xFFFFFF00 + current.q * 4);
			}
			origin = origin - this->_upDir;
		}
		origin = tmp + this->_rightDir;
	}
}

bool		Raycast::raycast(Map::voxel& vox, Ray ray)
{
//	std::cout << " ------------- " << std::endl;	
	this->_shortestDist = 0;
	this->_dist = 0;
	raycastOneCoord(vox, ray, ray.dir.x, Raycast::X);
	raycastOneCoord(vox, ray, ray.dir.y, Raycast::Y);
	raycastOneCoord(vox, ray, ray.dir.z, Raycast::Z);

	if (vox.type == Map::voxel::VOID)
		return false;
//	std::cout << "true found" << std::endl;
	return true;
}

void		Raycast::raycastOneCoord(Map::voxel& vox, Ray ray,
									 float& dir, coord C)
{
	Map::point	firstPoint;
	Map::point	hitPoint;

//	std::cout << " ---- " << std::endl;
	if (dir != 0)
	{
//		std::cout << "dir 0 avant: " << ray.dir << std::endl;
		firstPoint = getFirstPoint(ray, C);
//		std::cout << "dir 1 avant: " << ray.dir << std::endl;
		ray.dir = ray.dir * std::abs(1 / dir);
//		std::cout << "dir 1 apres: " << ray.dir << std::endl;
//		std::cout << "FirstPoint: (" << firstPoint << ")" << std::endl;
		if (raycastInMap(hitPoint, ray, firstPoint))
		{
//			std::cout << "found" << std::endl;
			this->_dist = Map::point::dist(hitPoint, ray.origin);
			if (_shortestDist == 0 || _dist < _shortestDist)
			{
				_shortestDist = _dist;
				vox = this->_map[hitPoint.z][hitPoint.y][hitPoint.x];
			}
		}
	}
}

bool		Raycast::raycastInMap(Map::point& hitPoint, const Ray& ray,
								  Map::point point)
{
	int		x;
	int		y;
	int		z;

	x = std::floor(point.x);
	y = std::floor(point.y);
	z = std::floor(point.z);
	while(x >= 0 && x < CUBE_SIZE
		&& y >= 0 && y < CUBE_SIZE
		&& z >= 0 && z < CUBE_SIZE / 2)
	{
//		std::cout << "In Map !: x:" << x << " y: " << y << " z: " << z << std::endl;
		if (this->_map[z][y][x].type != Map::voxel::VOID)
		{
//			std::cout << "Found ! BLock OK !!" << std::endl;
			hitPoint = point;
			return true;
		}
		point = point + ray.dir;
		x = std::floor(point.x);
		y = std::floor(point.y);
		z = std::floor(point.z);
	}
	return false;
}

Map::point	Raycast::getFirstPoint(Ray ray, coord C)
{
	Map::point	first;
	float		diff, ratio;

	if (C == X)
	{
		first.x = ray.dir.x < 0 ? CUBE_SIZE - 1: 0;
		diff = first.x - ray.origin.x;
		if (diff != 0)
			ratio = diff / ray.dir.x;
	}
	if (C == Y)
	{
		first.y = ray.dir.y < 0 ? CUBE_SIZE - 1: 0;
		diff = first.y - ray.origin.y;
		if (diff != 0)
			ratio = diff / ray.dir.y;
	}
	if (C == Z)
	{
		first.z = ray.dir.z < 0 ? (CUBE_SIZE / 2) - 1 : 0;
		diff = first.z - ray.origin.z;
		if (diff != 0)
			ratio = diff / ray.dir.z;
	}
	if (diff == 0)
	{
		first = ray.origin;
		return first;
	}
	ray.dir = ray.dir * ratio;
	first = ray.origin + ray.dir;
	return first;
}

void	Raycast::generateCam(void)
{
	this->_upDir = Map::point(0, 0, 1);
	this->_dir = _camPos - _camDir;
	this->_dir.normalize();
	this->_rightDir = Map::point::cross(this->_upDir, this->_dir);
	this->_upDir    = Map::point::cross(this->_dir, this->_rightDir);
	this->_rightDir = this->_rightDir * _zoom;
	this->_upDir = this->_upDir * _zoom;
	_upLeft = _camPos + (_upDir * (this->_h / 2)) - (_rightDir * (this->_w / 2));
	std::cout << "DIR: (" << this->_dir << ")" << std::endl;
	std::cout << "UPLEFT: (" << this->_upLeft << ")" << std::endl;
	std::cout << "UPDIR: (" << this->_upDir << ")" << std::endl;
	std::cout << "RIGHT: (" << this->_rightDir << ")" << std::endl;
}

void	Raycast::setZoom(const float& z)
{
	this->_zoom = z;
	this->generateCam();
}

Raycast::~Raycast (void)
{
}

Raycast::Ray::Ray(Map::point o, Map::point d) : origin(o), dir(d)
{
}

Raycast::Ray::~Ray(void)
{
}
