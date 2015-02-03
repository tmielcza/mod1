// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Map.hpp                                            :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tmielcza <tmielcza@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/01/22 15:07:14 by tmielcza          #+#    #+#             //
//   Updated: 2015/02/03 20:27:45 by tmielcza         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef MAP_HPP
# define MAP_HPP

static const int CUBE_SIZE = 128;
static const int HALF_CUBE_SIZE = 64;

# include <list>
# include <vector>
# include <cmath>

class Map
{
public:
	struct point
	{
		point(void);
		point(float _x, float _y, float _z);
		point(const point& src);
		~point(void);

		void	normalize(void);
		point&	operator=(const point& rhs);
		point	operator*(const point& rhs);
		point	operator*(const float& rhs);
		point	operator/(const point& rhs);
		point	operator/(const float& rhs);
		point	operator+(const point& rhs);
		point	operator+(const float& rhs);
		point	operator-(const point& rhs);
		point	operator-(const float& rhs);

		float x, y, z, dst;

		static float	dist(const point & a, const point& b);
		static float	getDst(const point& a, const point& b);
		static point	cross(const point& a, const point& b);
	};

	struct voxel
	{
		enum Voxel_Type {VOID = 0, SOIL = 1, WATER = 2};

		voxel(Voxel_Type type, unsigned char q);
		voxel(void);

		unsigned char	type;
		unsigned char	q;
		unsigned char	z;
		unsigned char	u;
	};

	Map(void);
	~Map(void);

	void	setPoints(std::list<point>* pts);
	void	voxelizeMap(void);
	
	const std::vector< std::vector< std::vector <voxel> > >& voxels(void) const;
	const char*			heights(void) const;

	void	drainWoxel(const int& x, const int& y, const int& z);
	void	drainWoxels(void);
//	void	pushWater(const unsigned int x, const unsigned int y, const unsigned int z, const unsigned int q);

	const std::list<point>&	Points(void);

	void	rain(void);
	void	plane(void);
	void	wave(void);
	void	column(void);
	void	drain(void);
	void	setWaterHeight(void);
	void	PutWater(int x, int y, int z, int q) {_vox[z][y][x] = voxel(voxel::WATER, q);} // A VIRER

private:

	std::list<point>*	_pts;
	std::vector< std::vector< std::vector <voxel> > >	_vox;
	char*				_hMap;
	int					_waterHeight;
	std::vector< std::vector <int> >	_streamOrder;

	void	initStreamOrder(void);

	Map(const Map& src);

	Map&	operator=(const Map& rhs);

	void	exchangeWater(voxel& src, voxel& dst, const int q);
	point	interPoint(const float x, const float y) const;
	static float	Weight(float d);

	bool	isInMap(const int& x, const int& y, const int& z) const;
	bool	isSoil(const int& x, const int& y, const int& z) const;
	bool	isWater(const int& x, const int& y, const int& z) const;
	bool	isAir(const int& x, const int& y, const int& z) const;
	bool	isObstacle(const int& x, const int& y, const int& z) const;

	bool	isSoil(const voxel& vox) const;
	bool	isWater(const voxel& vox) const;
	bool	isAir(const voxel& vox) const;
};

std::ostream& operator<<(std::ostream& o, const Map::point& rhs);


#endif // MAP_HPP
