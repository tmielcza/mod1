// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Map.cpp                                            :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tmielcza <tmielcza@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/01/24 15:59:11 by tmielcza          #+#    #+#             //
//   Updated: 2015/02/01 21:54:22 by tmielcza         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <cstring>
#include "Map.hpp"

Map::Map(void) : _vox(CUBE_SIZE / 2, std::vector< std::vector <voxel> >(CUBE_SIZE, std::vector<voxel>(CUBE_SIZE)))
{
	this->_pts = NULL;
	this->_hMap = new char[CUBE_SIZE * CUBE_SIZE]();
	this->_tmp = new voxel[CUBE_SIZE * CUBE_SIZE * (CUBE_SIZE / 2)];
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

const int	Map::surroundings::OffsetPos[3][3][3] = {
	{{0, 1, 2},
	 {3, 4, 5},
	 {6, 7, 8}},
	{{9, 10, 11},
	 {12, -1, 13},
	 {14, 15, 16}},
	{{17, 18, 19},
	 {20, 21, 22},
	 {23, 24, 25}}};

Map::surroundings::surroundings(void) : data(0)
{
}

bool	Map::surroundings::Position(int x, int y, int z)
{
	return (data & 1 << OffsetPos[z + 1][y + 1][x + 1]);
}

void	Map::surroundings::Position(int x, int y, int z, bool block)
{
	unsigned int	val = 1 << OffsetPos[z + 1][y + 1][x + 1];

	data &= ~val;
	if (block)
		data |= val;
}

/*
bool	Map::surroundings::Face(FacePosition face)
{
	FacePosition faces[6] = {BOTTOM, UP, FRONT, BACK, LEFT, RIGHT}; 
	int	tab[6][2][3] = {
		{{1, 1, 0}, {-1, -1, -1}},
		{{1, 1, 0}, {-1, -1, 1}},
		{{1, 0, 1}, {-1, -1, -1}},
		{{1, 0, 1}, {-1, 1, -1}},
		{{0, 1, 1}, {-1, -1, -1}},
		{{0, 1, 1}, {1, -1, -1}},
	};

	int theFace;
	for (theFace = 0; theFace < 6; theFace++)
	{
		if (faces[theFace] == face)
			break ;
	}

	int incx = tab[face][0][0];
	int	incy = tab[face][0][1];
	int	incz = tab[face][0][2];

	bool	ret = false;

	for (int i = 0, x = tab[face][1][0], y = tab[face][1][1], z = tab[face][1][2]; i < 9; i++)
	{
		
	}
}
*/

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
	std::memset(this->_tmp, 0, sizeof(voxel) * CUBE_SIZE * CUBE_SIZE * (CUBE_SIZE / 2));

	for (int z = 0; z < CUBE_SIZE / 2; z++)
	{
		for (int y = 0; y < CUBE_SIZE; y++)
		{
			for (int x = 0; x < CUBE_SIZE; x++)
			{
				if (this->_vox[z][y][x].type != voxel::SOIL)
					drainWoxel(x, y, z);
			}
		}
	}

	for (int i = 0; i < CUBE_SIZE / 2; i++)
	{
		for (int j = 0; j < CUBE_SIZE; j++)
		{
			for (int k = 0; k < CUBE_SIZE; k++)
			{
				if (this->_tmp[k + j * CUBE_SIZE + i * CUBE_SIZE * CUBE_SIZE].type == voxel::WATER)
				{
					this->_vox[i][j][k] = this->_tmp[k + j * CUBE_SIZE + i * CUBE_SIZE * CUBE_SIZE];
				}
			}
		}
	}
}

void				Map::drainWoxel(const unsigned int x, const unsigned int y, const unsigned int z)
{
	int		count = 0;
	int		water = 0;
	int		fq = 0;

	if (!this->isBlock(x, y, z + 1) && this->_vox[z + 1][y][x].q > 0)
	{
		int		q = this->_vox[z + 1][y][x].q;
		fq += (fq + q <= 255) ? q : 255 - fq;
//		exchangeWater(vox, this->_tmp[x + y * CUBE_SIZE + (z - 1) * CUBE_SIZE * CUBE_SIZE], vox.q + q > 255 ? 255 - q : vox.q);
	}

	bool	blockCanFill = false;

	if (!this->isBlock(x, y, z - 1))
	{
		voxel&	vox = this->_vox[z - 1][y][x];

//		if (vox.type == voxel::WATER)
//			blockCanFill = true;
		if (vox.type == voxel::VOID)
		{
			for (int i = 0 ; i < 9 ; i++)
			{
				int x2 = x + i % 3 - 1;
				int y2 = y + i / 3 - 1;

				if (i != 4 && this->isBlock(x2, y2, z - 1))
					blockCanFill = true;
			}
		}
	}
	else
		blockCanFill = true;

	if (fq < 255 && blockCanFill)
	{
		for (int i = 0; i < 9; i++)
		{
			int x2 = x + i % 3 - 1;
			int y2 = y + i / 3 - 1;

			if (i != 4 && !this->isBlock(x2, y2, z))
			{
				count++;
				water += this->_vox[z][y2][x2].q;
			}
		}

		if (count)
		{
			int m = water / count;

			if (m != 0)
			{
				fq = (m + fq > 255) ? 255 : fq + m;
				//fq = 255;
			}
		}
	}

	if (fq == 0)
		return;

	voxel& vox2 = this->_tmp[x + y * CUBE_SIZE + z * CUBE_SIZE * CUBE_SIZE];

	vox2.type = voxel::WATER;
	vox2.q = fq;
/*
	if (m < vox.q && (vox.q - m) / count != 0)
	{
		int gift = (vox.q - m) / count;

		for (int i = 0; i < 9; i++)
		{
			int x2 = x + i % 3 - 1;
			int y2 = y + i / 3 - 1;

			if (i != 4 && !this->isBlock(x2, y2, z) && vox.q > this->_vox[z][y2 + y][x2 + x].q)
			{
				voxel& vox2 = this->_vox[z][y2 + y][x2 + x];
				std::cout << "vox " << x << "," << y << "," << z << " (" << (int)vox.q << ") puts "
						  << gift << " in " << x2 << "," << y2 << "," << z << " (" << (int)vox2.q << ")"
						  << std::endl;
				exchangeWater(vox, this->_tmp[x2 + y2 * CUBE_SIZE + z * CUBE_SIZE * CUBE_SIZE], gift + vox2.q <= 255 ? gift : 255 - vox2.q);
				std::cout << "Then vox.q : " << (int)vox.q << " ,vox2(copy).q: " << (int)this->_tmp[x2 + y2 * CUBE_SIZE + z * CUBE_SIZE * CUBE_SIZE].q << std::endl;
			}
		}
		if (vox.q != 0)
		{
			voxel& vox3 = this->_tmp[x + y * CUBE_SIZE + z * CUBE_SIZE * CUBE_SIZE];
			vox3.q += vox.q;
			vox3.type = voxel::WATER;
		}
	}
*/
}

bool			Map::isBlock(const int x, const int y, const int z) const
{
	if (x >= CUBE_SIZE || y >= CUBE_SIZE || z >= CUBE_SIZE / 2)
		return (true);
	else if (x < 0 || y < 0 || z < 0)
		return (true);
	else
		return (this->_vox[z][y][x].type == voxel::SOIL);
}

Map::surroundings	Map::woxelSurroundings(const unsigned int x, const unsigned int y, const unsigned int z) const
{
	surroundings	ret;

	for (int zi = -1; zi <= 1; zi++)
	{
		for (int yi = -1; yi <= 1; yi++)
		{
			for (int xi = -1; xi <= 1; xi++)
			{
				bool	full = true;

				if (xi == 0 && yi == 0 && zi == 0)
					continue ;
				if ((x == 0 && xi < 0) || (y == 0 && yi < 0) || (z == 0 && zi < 0))
					full = true;
				else if (x + xi >= CUBE_SIZE || y + yi >= CUBE_SIZE || z + zi >= CUBE_SIZE / 2)
					full = true;
				else
				{
					full = _vox[z + zi][y + yi][x + xi].type == voxel::SOIL;
				}
				ret.Position(xi, yi, zi, full);
			}
		}
	}
	return (ret);
}
