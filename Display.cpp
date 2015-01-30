// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Display.cpp                                        :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tmielcza <tmielcza@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/01/21 12:44:56 by tmielcza          #+#    #+#             //
//   Updated: 2015/01/30 20:34:45 by tmielcza         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <SDL.h>
#include <iostream>
#include "Display.hpp"
#include <OpenGL/gl.h>
#include "Utils.hpp"

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

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetSwapInterval(1);

		this->_win = SDL_CreateWindow("mod1",
									  SDL_WINDOWPOS_UNDEFINED,
									  SDL_WINDOWPOS_UNDEFINED,
									  this->_w,
									  this->_h,
									  SDL_WINDOW_OPENGL);
		if (this->_win == NULL)
		{
			throw (std::exception());
		}

		if ((this->_glc = SDL_GL_CreateContext(this->_win)) == NULL)
		{
			throw (std::exception());
		}


		glEnable(GL_TEXTURE_3D);
		glGenTextures(1, &this->_mapTex);
		glBindTexture(GL_TEXTURE_3D, this->_mapTex);
		glTexImage3D(GL_TEXTURE_3D,
					 0,
					 4,
					 128,
					 128,
					 64,
					 0,
					 GL_RGBA,
					 GL_UNSIGNED_BYTE,
					 NULL);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

/*
		this->_ren = SDL_CreateRenderer(this->_win, -1, 0);
		if (this->_ren == NULL)
		{
			throw (std::exception());
		}

		this->_tex = SDL_CreateTexture(this->_ren,
									   SDL_PIXELFORMAT_ARGB8888,
									   SDL_TEXTUREACCESS_STATIC,
									   this->_w,
									   this->_h);

		this->_pix = new Uint32[this->_w * this->_h];
*/
		this->_prog = glCreateProgram();

	}
	catch (std::exception& e)
	{
		std::cerr << "Unable to initialize SDL: " << SDL_GetError() << std::endl;
		throw (std::exception());
	}
}

Display::~Display(void)
{
//	delete [] this->_pix;
//	SDL_DestroyTexture(this->_tex);
//	SDL_DestroyRenderer(this->_ren);
	SDL_DestroyWindow(this->_win);	
	SDL_GL_DeleteContext(this->_glc);
	SDL_Quit();
}

/*
void		Display::addPixel(unsigned int x, unsigned int y, Uint32 col)
{
	if (x >= this->_w || y >= this->_h)
	{
		return ;
	}
	this->_pix[x + y * this->_w] = col;
}
*/

void		Display::initShaderProgram(const std::string name)
{
	std::string data = Utils::getFile(name);
	GLint		isLinked;

	this->_frag = this->compileShader(data, GL_FRAGMENT_SHADER);
	glAttachShader(this->_prog, this->_frag);
	glLinkProgram(this->_prog);
	glUseProgram(this->_prog);

	glGetProgramiv(this->_prog, GL_LINK_STATUS, &isLinked);
	if (isLinked == false)
		std::cout << "DANSLKUL !" << std::endl;
}

GLuint		Display::compileShader(const std::string data, const GLenum flag) const
{
	GLuint			id;
	GLint			stat;
	GLsizei			logsize;
	char*			log;
	const char*		dat = data.c_str();

	id = glCreateShader(flag);
	glShaderSource(id, 1, &dat, NULL);
	glCompileShader(id);
	glGetShaderiv(id, GL_COMPILE_STATUS, &stat);

	if (stat != GL_TRUE)
	{
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logsize);
		log = new char[logsize]();
		glGetShaderInfoLog(id, logsize, &logsize, log);
		std::cout << "CRITICAL: " << log << std::endl;
		delete [] log;
		glDeleteShader(id);
		throw (std::exception());
	}
	return id;
}

void		Display::draw(const void* data, const int x, const int y, const int z)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, this->_mapTex);

	(void)z;
	glTexSubImage3D(GL_TEXTURE_3D,
					0,
					0,
					0,
					0,
					x,
					y,
					z,
					GL_RGBA,
					GL_UNSIGNED_BYTE,
					data);

	GLuint texLoc = glGetUniformLocation(this->_prog, "MapTex");
	glUniform1i(texLoc, 0);

	glBegin(GL_QUADS);
	glVertex3f(-1.0, -1.0, 0.0);
	glVertex3f(-1.0, 1.0, 0.0);
	glVertex3f(1.0, 1.0, 0.0);
	glVertex3f(1.0, -1.0, 0.0);
	glEnd();

	SDL_GL_SwapWindow(this->_win);
//	SDL_UpdateTexture(this->_tex, NULL, this->_pix, this->_w * sizeof(Uint32));
//	SDL_RenderClear(this->_ren);
//	SDL_RenderCopy(this->_ren, this->_tex, NULL, NULL);
//	SDL_RenderPresent(this->_ren);
}
