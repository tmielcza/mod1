// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Raycast.cpp                                        :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: caupetit <marvin@42.fr>                    +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/01/23 14:59:29 by caupetit          #+#    #+#             //
//   Updated: 2015/01/25 21:28:51 by tmielcza         ###   ########.fr       //
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
	_camDir(Map::point(CUBE_SIZE / 2, CUBE_SIZE / 2, 0)),
	_w(d.getW()),
	_h(d.getH()),
	_upDir(Map::point(0, 0, 1)),
	_zoom(0.5),
	_shortestDist(0),
	_dist(0)
{
	this->_dir = _camDir - _camPos;
	this->_dir.normalize();
	this->_rightDir = Map::point::cross(this->_upDir, this->_dir);
	this->_upDir    = Map::point::cross(this->_dir, this->_rightDir);
	this->_rightDir = this->_rightDir * _zoom;
	this->_upDir = this->_upDir * _zoom;
	float H = this->_h / 2;
	float W = this->_w / 2;
	_upLeft = _camPos + _upDir * H - _rightDir * W;
//	_upLeft = _camPos + (_upDir * (this->_h / 2)) - (_rightDir * (this->_w / 2));
	std::cout << "INIT dir" << _dir << std::endl; 
	std::cout << "INIT: right: " << _rightDir << " up: " << _upDir << std::endl; 
	std::cout << "INIT: upleft " << _upLeft  << std::endl; 
}

void	Raycast::raycastMapVoxels1(void)
{
	Map::voxel	current;
	Map::point	origin = this->_upLeft;
	Map::point	tmp;

	for (int i = 0 ; i < this->_w ; i++)
	{
		tmp = origin;
		for (int j = 0 ; j < this->_h ; j++)
		{
			if (raycast1(current, Raycast::Ray(origin, this->_dir)))
			{
				if (current.type == Map::voxel::WATER)
					this->_display.addPixel(i, j, 0x000000FF + 0x00FFFF00 * static_cast<float>(255 - current.q) / 255);
//					this->_display.addPixel(i, j, 0x000000FF);
				else
					this->_display.addPixel(i, j, 0xFFFFFF00 + current.q * 2);
				current.type = Map::voxel::VOID;
			}
			origin = origin - this->_upDir;
		}
		origin = tmp + this->_rightDir;
	}
}

bool		Raycast::raycast1(Map::voxel& vox, Ray ray)
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
		if (raycastInMap(hitPoint, ray, firstPoint))
		{
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
								  Map::point& point)
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
			hitPoint = point;
//			exit(0);
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
		first.x = ray.dir.x < 0 ? CUBE_SIZE - 1 : 0;
		diff = first.x - ray.origin.x;
		if (diff != 0)
			ratio = std::abs(diff / ray.dir.x);
	}
	if (C == Y)
	{
		first.y = ray.dir.y < 0 ? CUBE_SIZE - 1 : 0;
		diff = first.y - ray.origin.y;
		if (diff != 0)
			ratio = std::abs(diff / ray.dir.y);
	}
	if (C == Z)
	{
		first.z = ray.dir.z > 0 ? 0  : (CUBE_SIZE / 2) - 1;
		diff = first.z - ray.origin.z;
		if (diff != 0)
			ratio = std::abs(diff / ray.dir.z);
	}
	if (diff == 0)
	{
		first = ray.origin;
		return first;
	}
//	std::cout << "Origin: " << ray.origin << std::endl;
//	std::cout << "dir avant: " << ray.dir << std::endl;
	ray.dir = ray.dir * ratio;
	first = ray.origin + ray.dir;
//	std::cout << "dir apres: " << ray.dir << std::endl;
//	std::cout << "First Point: " << first << std::endl;
	return first;
}

void	Raycast::setZoom(const float& z)
{
	this->_zoom = z;
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
