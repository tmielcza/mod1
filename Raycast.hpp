// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Raycast.hpp                                        :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: caupetit <marvin@42.fr>                    +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/01/23 14:59:35 by caupetit          #+#    #+#             //
//   Updated: 2015/01/27 12:55:52 by caupetit         ###   ########.fr       //
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

	enum	coord {X, Y, Z};

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
	Raycast (const Map::point& p, Display& d,
			 const std::vector<std::vector<std::vector<Map::voxel> > >& map);
//	Raycast (const Map::point& p,const int& w,const int& h, const float& zoom);
	~Raycast (void);

	/* Members Functions */

	void		raycastMapVoxels(void);
	void		setZoom(const float& z);

private:
	const std::vector<std::vector<std::vector<Map::voxel> > >& _map;
	Display&	_display;
	Map::point	_camPos;
	Map::point	_camDir;
	Map::point	_dir;
	int			_w;
	int			_h;
	Map::point	_upLeft;
	Map::point	_upDir;
	Map::point	_rightDir;
	float		_zoom;
	float		_shortestDist;
	float		_dist;

	bool		raycast(Map::voxel& vox, Ray ray);
	void		raycastOneCoord(Map::voxel& vox, Ray ray, float& dir, coord C);
	Map::point	getFirstPoint(Ray ray, coord C);
	bool		raycastInMap(Map::point& hitPoint, const Ray& ray,
							 Map::point point);
	void		generateCam(void);

	Raycast (void);
	Raycast(Raycast const & rhs);
	Raycast &	operator=(Raycast const & rhs);
};

#endif
