// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Raycast.hpp                                        :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: caupetit <marvin@42.fr>                    +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/01/23 14:59:35 by caupetit          #+#    #+#             //
//   Updated: 2015/01/25 14:57:25 by tmielcza         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef RAYCAST_HPP
# define RAYCAST_HPP

# include <iostream>
# include <cmath>
# include "Map.hpp"
# include "Display.hpp"

class		Raycast
{
public:

	struct	Ray
	{
		Ray(Map::point origin, Map::point direction);
		~Ray(void);

		Map::point	origin;
		Map::point	dir;

	private:
		Ray(void);
	};

	/* Constructors / Destructors */
	Raycast (const Map::point& p,const int& w,const int& h, const float& zoom);
	~Raycast (void);

	/* Members Functions */

	bool	raycast(Map::voxel& vox,
					const std::vector<std::vector<std::vector<Map::voxel> > >& map,
					Ray ray);
	void	raycastMapVoxels(const Map& map, Display & display);

private:
	Map::point	_camPos;
	Map::point	_camDir;
	Map::point	_dir;
	int			_w;
	int			_h;
	Map::point	_upLeft;
	Map::point	_upDir;
	Map::point	_rightDir;

	Raycast (void);
	Raycast(Raycast const & rhs);
	Raycast &	operator=(Raycast const & rhs);
};

#endif
