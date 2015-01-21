// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Display.cpp                                        :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tmielcza <tmielcza@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/01/21 12:44:56 by tmielcza          #+#    #+#             //
//   Updated: 2015/01/21 15:04:44 by tmielcza         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <SDL.h>
#include <iostream>
#include "Display.hpp"

Display::Display(void)
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
									  640,
									  480,
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
									   640,
									   480);

		this->_pix = new Uint32[640 * 480];
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
	SDL_UpdateTexture(this->_tex, NULL, this->_pix, 640 * sizeof(Uint32));
	SDL_RenderClear(this->_ren);
	SDL_RenderCopy(this->_ren, this->_tex, NULL, NULL);
	SDL_RenderPresent(this->_ren);
}
