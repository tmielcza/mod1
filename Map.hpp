// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Map.hpp                                            :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tmielcza <tmielcza@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/01/22 15:07:14 by tmielcza          #+#    #+#             //
//   Updated: 2015/01/23 12:33:47 by tmielcza         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef MAP_HPP
# define MAP_HPP

# define CUBE_SIZE 128

# include <list>

# include "Display.hpp" // A VIRER

class Map
{
public:
	struct point
	{
		point(void);
		point(float _x, float _y, float _z);
		point(const point& src);
		~point(void);

		point&	operator=(const point& rhs);

		float x, y, z, dst;

		static float	getDst(const point& a, const point& b);
	};

	struct voxel
	{
		enum Voxel_Type {VOID, SOIL, WATER};
		voxel(Voxel_Type);
		voxel(void);

		char			type;
		char			q;
	};

	Map(void);
	~Map(void);

	void	setPoints(std::list<point>* pts);
	void	voxelizeMap(void);

	const std::list<point>&	Points(void);

	static Display* displayer; // A VIRER

private:
	std::list<point>*			_pts;
	voxel						_vox[CUBE_SIZE][CUBE_SIZE / 2][CUBE_SIZE];
/*
	float						_ratio;
	unsigned int				_offsetx;
	unsigned int				_offsety;
*/

	Map(const Map& src);

	Map&	operator=(const Map& rhs);

	point	interPoint(const float x, const float y) const;
	static float	Weight(float d);
};

#endif // MAP_HPP
