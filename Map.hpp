// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Map.hpp                                            :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tmielcza <tmielcza@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/01/22 15:07:14 by tmielcza          #+#    #+#             //
//   Updated: 2015/01/29 20:35:41 by caupetit         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef MAP_HPP
# define MAP_HPP

# define CUBE_SIZE 128

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

		voxel(Voxel_Type& type, unsigned char& q);
//		voxel(void);

		Voxel_Type&		type;
		unsigned char&	q;
	};

	Map(void);
	~Map(void);

	void	setPoints(std::list<point>* pts);
	void	voxelizeMap(void);
	
//	const std::vector< std::vector< std::vector <voxel> > >& voxels(void) const;

	void	drainWoxel(const unsigned int x, const unsigned int y, const unsigned int z);
	void	drainWoxels(void);
	void	setVoxel(int x, int y, int z, voxel::Voxel_Type type, unsigned char info);
	voxel	getVoxel(int x, int y, int z) const;
//	void	pushWater(const unsigned int x, const unsigned int y, const unsigned int z, const unsigned int q);

	const std::list<point>&	Points(void);

//	void	PutWater(int x, int y, int z) {_vox[z][y][x] = voxel(voxel::WATER, 255);} // A VIRER

private:
	struct surroundings
	{
		surroundings(void);

		static const int	OffsetPos[3][3][3];

		enum FacePosition {ALL, BOTTOM, UP, FRONT, BACK, LEFT, RIGHT};

		bool		Position(int x, int y, int z);
		void		Position(int x, int y, int z, bool block);
//		bool		Face(FacePosition face);

		int			data;
	};

	surroundings	woxelSurroundings(const unsigned int x, const unsigned int y, const unsigned int z) const;

	std::list<point>*	_pts;
//	std::vector< std::vector< std::vector <voxel> > >	_vox;

	voxel::Voxel_Type*	_vtype;
	unsigned char*		_vinfo;

	Map(const Map& src);

	Map&	operator=(const Map& rhs);

	void	exchangeWater(voxel& src, voxel& dst, const int q);
	point	interPoint(const float x, const float y) const;
	static float	Weight(float d);
};

std::ostream& operator<<(std::ostream& o, const Map::point& rhs);


#endif // MAP_HPP
