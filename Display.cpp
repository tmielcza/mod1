// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Display.cpp                                        :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tmielcza <tmielcza@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/01/21 12:44:56 by tmielcza          #+#    #+#             //
//   Updated: 2015/01/25 17:31:14 by caupetit         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <SDL.h>
#include <iostream>
#include "Display.hpp"

unsigned int	Display::getH(void) const
{
	return this->_h;
}

unsigned int	Display::getW(void) const
{
	return this->_w;
}

Display::Display(void) : _w(640), _h(480)
{
	try
	{
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			throw (std::exception());
		}

		this->_win = SDL_CreateWindow("mod1",
									  SDL_WINDOWPOS_UNDEFINED,
									  SDL_WINDOWPOS_UNDEFINED,
									  this->_w,
									  this->_h,
									  SDL_WINDOW_SHOWN);
		if (this->_win == NULL)
		{
			throw (std::exception());
		}

		this->_ren = SDL_CreateRenderer(this->_win, -1, 0);
		if(this->_ren == NULL)
		{
			throw (std::exception());
		}

		this->_tex = SDL_CreateTexture(this->_ren,
									   SDL_PIXELFORMAT_ARGB8888,
									   SDL_TEXTUREACCESS_STATIC,
									   this->_w,
									   this->_h);

		this->_pix = new Uint32[this->_w * this->_h];
	}
	catch (std::exception& e)
	{
		std::cerr << "Unable to initialize SDL: " << SDL_GetError() << std::endl;
		throw (std::exception());
	}
}

Display::~Display(void)
{
	delete [] this->_pix;
	SDL_DestroyTexture(this->_tex);
	SDL_DestroyRenderer(this->_ren);
	SDL_DestroyWindow(this->_win);
	SDL_Quit();
}

void		Display::addPixel(unsigned int x, unsigned int y, Uint32 col)
{
	if (x >= this->_w || y >= this->_h)
	{
		return ;
	}
	this->_pix[x + y * this->_w] = col;
}

void		Display::draw(void)
{
	SDL_UpdateTexture(this->_tex, NULL, this->_pix, this->_w * sizeof(Uint32));
	SDL_RenderClear(this->_ren);
	SDL_RenderCopy(this->_ren, this->_tex, NULL, NULL);
	SDL_RenderPresent(this->_ren);
}
