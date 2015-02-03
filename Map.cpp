// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Map.cpp                                            :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tmielcza <tmielcza@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/01/24 15:59:11 by tmielcza          #+#    #+#             //
//   Updated: 2015/02/03 02:40:40 by tmielcza         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <ctime> 
#include <cstdlib>
#include <algorithm>
#include "Map.hpp"

Map::Map(void) : _vox(CUBE_SIZE / 2, std::vector< std::vector <voxel> >(CUBE_SIZE, std::vector<voxel>(CUBE_SIZE))),
				 _streamOrder(10, std::vector<int>(9))
{
	std::srand(std::time(NULL));
	this->_pts = NULL;
	this->_hMap = new char[CUBE_SIZE * CUBE_SIZE]();
	this->_waterHeight = 1;
	for (unsigned int i = 0; i < this->_streamOrder.size(); i++)
		for (unsigned int j = 0; j < this->_streamOrder[0].size(); j++)
			this->_streamOrder[i][j] = j;
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
	int				miny = 0;
	int				minx = 0;

	unsigned int	maxsize, sizex, sizey;

	float			hRatio;

	for (std::list<point>::const_iterator it = pts->begin(), end = pts->end(); it != end; ++it)
	{
		if (it->z > maxz)
			maxz = it->z;
		if (it->y > maxy)
			maxy = it->y + it->z;
		if (miny == 0 || it->y < miny)
			miny = it->y - it->z;
		if (it->x > maxx)
			maxx = it->x + it->z;
		if (minx == 0 || it->x < minx)
			minx = it->x - it->z;
	}
	maxz = maxz * 2;

	sizex = maxx - minx;
	sizey = maxy - miny;

	maxsize = std::max(maxz, sizex);
	maxsize = std::max(maxsize, sizey);

	hRatio = static_cast<float>(maxsize) / CUBE_SIZE;

	for (std::list<point>::iterator it = pts->begin(), end = pts->end(); it != end; ++it)
	{
		it->x = (it->x - minx) / hRatio;
		it->y = (it->y - miny) / hRatio;
		it->z = it->z / hRatio;
	}
}

Map::voxel::voxel(void) : type(VOID), q(0)
{
}

Map::voxel::voxel(Voxel_Type type, unsigned char q) : type(type), q(q), z(100), u(25)
{
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

void		Map::point::normalize(void)
{
	float	dist = std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z);

	this->x = this->x / dist;
	this->y = this->y / dist;
	this->z = this->z / dist;
}

Map::point&	Map::point::operator=(const point& rhs)
{
	this->x = rhs.x;
	this->y = rhs.y;
	this->z = rhs.z;
	this->dst = rhs.dst;
	return (*this);
}

Map::point	Map::point::operator*(const point& rhs)
{
	return Map::point(this->x * rhs.x, this->y * rhs.y, this->z * rhs.z);
}

Map::point	Map::point::operator*(const float& rhs)
{
	return Map::point(this->x * rhs, this->y * rhs, this->z * rhs);
}

Map::point	Map::point::operator/(const point& rhs)
{
	return Map::point(this->x / rhs.x, this->y / rhs.y, this->z / rhs.z);
}

Map::point	Map::point::operator/(const float& rhs)
{
	return Map::point(this->x / rhs, this->y / rhs, this->z / rhs);
}

Map::point	Map::point::operator+(const Map::point& rhs)
{
	return Map::point(this->x + rhs.x, this->y + rhs.y, this->z + rhs.z);
}

Map::point	Map::point::operator+(const float& rhs)
{
	return Map::point(this->x + rhs, this->y + rhs, this->z + rhs);
}

Map::point	Map::point::operator-(const Map::point& rhs)
{
	return Map::point(this->x - rhs.x, this->y - rhs.y, this->z - rhs.z);
}

Map::point	Map::point::operator-(const float& rhs)
{
	return Map::point(this->x - rhs, this->y - rhs, this->z - rhs);
}

std::ostream&	operator<<(std::ostream& o, const Map::point& rhs)
{
	o << rhs.x << ", " << rhs.y << ", " << rhs.z;
	return o;
}

float	Map::point::dist(const point& a, const point& b)
{
	float xi, yi, zi;

	xi = a.x - b.x;
	yi = a.y - b.y;
	zi = a.z - b.z;
	return (std::sqrt(xi * xi + yi * yi + zi * zi));
}

float	Map::point::getDst(const point& a, const point& b)
{
	float xi, yi;

	xi = a.x - b.x;
	yi = a.y - b.y;
	return (std::sqrt(xi * xi + yi * yi));
}

Map::point	Map::point::cross(const point& a, const point& b)
{
	Map::point p(a.x * b.z - a.z * b.y,
				 a.z * b.x - a.x * b.z,
				 a.x * b.y - a.y * b.x);
	return p;
}

float	Map::Weight(float d)
{
	if (d == 0)
		return (1);
	return (1.f / (d * d));
}

void	Map::voxelizeMap(void)
{
	point pt;

	for (int y = 0; y < CUBE_SIZE; y++)
	{
		for (int x = 0; x < CUBE_SIZE; x++)
		{
			pt = interPoint(x, y);
			for (int l = 0; l < pt.z; l++)
			{
				this->_vox[l][y][x] = voxel(voxel::SOIL, l);
				this->_hMap[y + x * CUBE_SIZE] = pt.z;
			}
		 }
	 }
}

const std::vector< std::vector< std::vector <Map::voxel> > >& Map::voxels(void) const
{
	return this->_vox;
}

const char*			Map::heights(void) const
{
	return this->_hMap;
}

void		Map::initStreamOrder(void)
{
	for (unsigned int i = 0; i < this->_streamOrder.size(); i++)
	{
		std::vector <int>&	stream = this->_streamOrder[i];
		std::random_shuffle(stream.begin(), stream.end());
	}
}

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
	}

	sum += Weight(std::min(x, CUBE_SIZE - x));
	sum += Weight(std::min(y, CUBE_SIZE - y));

	pz = sumVal/sum;

	return (point(px, py, pz));
}

void				Map::exchangeWater(voxel& src, voxel& dst, const int q)
{
	if (dst.type == voxel::SOIL)
	{
		return ;
	}
	src.q -= q;
	dst.q += q;
	dst.type = voxel::WATER;
	if (src.q == 0)
		src.type = voxel::VOID;
}

void				Map::drainWoxels(void)
{
	this->initStreamOrder();
	for (int z = 0; z < CUBE_SIZE / 2; z++)
	{
		for (int y = 0; y < CUBE_SIZE; y++)
		{
			for (int x = 0; x < CUBE_SIZE; x++)
			{
				if (this->_vox[z][y][x].type == voxel::WATER)
					drainWoxel(x, y, z);
			}
		}
	}
}

void				Map::drainWoxel(const int& x, const int& y, const int& z)
{
	voxel&	vox = this->_vox[z][y][x];
	int		count = 0;
	int		water = 0;

	if (!this->isObstacle(x, y, z - 1) && this->_vox[z - 1][y][x].q != 255)
	{
		voxel&	vox2 = this->_vox[z - 1][y][x];
		exchangeWater(vox, vox2, vox.q + vox2.q > 255 ? 255 - vox2.q : vox.q);
		if (vox.q == 0)
			return ;
	}

	for (int i = 0; i < 9; i++)
	{
		int x2 = x + i % 3 - 1;
		int y2 = y + i / 3 - 1;

		if (i != 4 && !this->isObstacle(x2, y2, z))
		{
			count++;
			water += this->_vox[z][y2][x2].q;
		}
	}
	water += vox.q;
	count++;
	if (water == 255 * count)
		return;

	if (!count)
		return;

	int m = water / count;
	int r = water % count;

	for (int i = 0; i < 9; i++)
	{
		int		pos = this->_streamOrder[0][i];
		int		x2 = x + pos % 3 - 1;
		int		y2 = y + pos / 3 - 1;

		if (pos != 4 && !this->isObstacle(x2, y2, z) && !this->isObstacle(x2, y2, z - 1))
		{
			voxel& vox2 = this->_vox[z][y2][x2];
			vox2.q = m;
			if (r > 0)
				vox2.q += 1;
			if (vox2.q != 0)
				vox2.type = voxel::WATER;
			else
				vox2.type = voxel::VOID;
			r--;
		}
	}

	for (int i = 0; i < 9; i++)
	{
		int		pos = this->_streamOrder[0][i];
		int		x2 = x + pos % 3 - 1;
		int		y2 = y + pos / 3 - 1;

		if (pos != 4 && !this->isObstacle(x2, y2, z) && this->isObstacle(x2, y2, z - 1))
		{
			voxel& vox2 = this->_vox[z][y2][x2];
			vox2.q = m;
			if (r > 0)
				vox2.q += 1;
			if (vox2.q != 0)
				vox2.type = voxel::WATER;
			else
				vox2.type = voxel::VOID;
			r--;
		}
	}
	if (m == 0)
		vox.type = voxel::VOID;
	vox.q = m;

}

bool				Map::isInMap(const int& x, const int& y, const int& z) const
{
	return (x >= 0 && y >= 0 && z >= 0 && x < CUBE_SIZE && y < CUBE_SIZE && z < HALF_CUBE_SIZE);
}

bool				Map::isSoil(const int& x, const int& y, const int& z) const
{
	return (this->_vox[z][y][x].type == voxel::SOIL);
}

bool				Map::isWater(const int& x, const int& y, const int& z) const
{
	return (this->_vox[z][y][x].type == voxel::WATER);
}

bool				Map::isAir(const int& x, const int& y, const int& z) const
{
	return (this->_vox[z][y][x].type == voxel::VOID);
}

bool				Map::isObstacle(const int& x, const int& y, const int& z) const
{
	return (!isInMap(x, y, z) || isSoil(x, y, z));
}

bool				Map::isSoil(const voxel& vox) const
{
	return (vox.type == voxel::SOIL);
}

bool				Map::isWater(const voxel& vox) const
{
	return (vox.type == voxel::WATER);
}

bool				Map::isAir(const voxel& vox) const
{
	return (vox.type == voxel::VOID);
}

void				Map::setWaterHeight(void)
{
	for (int i = 2 ; i < HALF_CUBE_SIZE - 2; i++)
	{
		if (this->_vox[i][0][0].type !=	voxel::WATER)
		{
			this->_waterHeight = i + 1;
			break;
		}
		if (this->_vox[i][CUBE_SIZE - 1][CUBE_SIZE - 1].type != voxel::WATER)
		{
			this->_waterHeight = i + 1;
			break;
		}
	}
}

void				Map::rain(void)
{
	int	z = HALF_CUBE_SIZE - 1;

	for (int i = 0 ; i < 4 ; i++)
	{
		int x = rand() % CUBE_SIZE;
		int	y = rand() % CUBE_SIZE;

		this->PutWater(x, y, z, 200);
	}
}

void				Map::plane(void)
{
	for (int i = 0 ; i < CUBE_SIZE ; i++)
	{
		this->PutWater(i, 0, this->_waterHeight, 50);
		this->PutWater(i, CUBE_SIZE - 1, this->_waterHeight, 50);
	}
	for (int j = 0 ; j < CUBE_SIZE ; j++)
	{
		this->PutWater(0, j, this->_waterHeight, 100);
		this->PutWater(CUBE_SIZE - 1, j, this->_waterHeight, 50);
	}
}

void				Map::wave(void)
{
	for (int i = 0 ; i < CUBE_SIZE ; i++)
		this->PutWater(i, 0, this->_waterHeight, 255);
	this->drainWoxels();
	for (int i = 0 ; i < CUBE_SIZE ; i++)
		this->PutWater(i, 0, this->_waterHeight, 255);
}

void				Map::column(void)
{
	for (int j = 50; j < 55; j++)
		for (int k = 95; k < 100; k++)
			this->PutWater(j, k, 40, 180);
}
