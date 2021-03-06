// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Display.cpp                                        :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tmielcza <tmielcza@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/01/21 12:44:56 by tmielcza          #+#    #+#             //
//   Updated: 2016/08/10 18:08:23 by tmielcza         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <iostream>
#include <OpenGL/gl.h>

#include "Display.hpp"
#include "Utils.hpp"
#include "Map.hpp"

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
		glEnable(GL_TEXTURE_2D);

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
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

		glGenTextures(1, &this->_hTex);
		glBindTexture(GL_TEXTURE_2D, this->_hTex);
		glTexImage2D(GL_TEXTURE_2D,
					 0,
					 4,
					 128,
					 128,
					 0,
					 GL_RGBA,
					 GL_UNSIGNED_BYTE,
					 NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

		this->_prog = glCreateProgram();

		this->_camDir = Map::point(CUBE_SIZE / 2, CUBE_SIZE / 2, CUBE_SIZE / 4);
		this->_zoom = 0.3;

		this->setCamRotation(45.0, 60.0);
		this->rotateCam();

		glClearColor(1,0,0,1);
		glClear(GL_COLOR_BUFFER_BIT);

		glViewport(0, 0, this->getW(), this->getH());
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, 1, 0, 1, 0, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	catch (std::exception& e)
	{
		std::cerr << "Unable to initialize SDL: " << SDL_GetError() << std::endl;
		throw (std::exception());
	}
}

Display::~Display(void)
{
	SDL_DestroyWindow(this->_win);
	SDL_GL_DeleteContext(this->_glc);
	SDL_Quit();
}

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
	{
		throw (std::exception());
	}
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

void		Display::setHeights(const char* hs, const int x, const int y)
{
	int*	tmp = new int[x * y];

	for (int i = 0; i < x * y; i++)
	{
		tmp[i] = hs[i];
	}

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->_hTex);

	glTexSubImage2D(GL_TEXTURE_2D,
					0,
					0,
					0,
					x,
					y,
					GL_RGBA,
					GL_UNSIGNED_BYTE,
					tmp);

	GLuint texLoc = glGetUniformLocation(this->_prog, "HTex");
	glUniform1i(texLoc, 1);
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

	GLuint zoomLoc = glGetUniformLocation(this->_prog, "Zoom");
	glUniform1f(zoomLoc, this->_zoom);

	glBegin(GL_QUADS);
	glVertex3f(-1.0, -1.0, 0.0);
	glVertex3f(-1.0, 1.0, 0.0);
	glVertex3f(1.0, 1.0, 0.0);
	glVertex3f(1.0, -1.0, 0.0);
	glEnd();

	SDL_GL_SwapWindow(this->_win);
}

void		Display::setCamRotation(int x, int y)
{
	this->_roth = x;
	this->_rotv = y;
}

const double g_Pi = 3.14159265358979323846;

void		Display::setZoom(const float x)
{
	float	res = this->_zoom - x;
	if (res >= 0.03 && res <= 1.0)
		this->_zoom = res;
}

void		Display::rotateCam()
{
	float Rotv = this->_rotv / 180. * g_Pi;
	float Roth = this->_roth / 180. * g_Pi;
	const float d = 300.;
	Map::point campos;

	campos.x = d * sin(Rotv) * cos(Roth);
	campos.y = d * sin(Rotv) * sin(Roth);
	campos.z = d * cos(Rotv);

	campos = campos + this->_camDir;

	GLuint camPosLoc = glGetUniformLocation(this->_prog, "CamPos");
	glUniform3f(camPosLoc, campos.x, campos.y, campos.z);
	GLuint camDirLoc = glGetUniformLocation(this->_prog, "CamDir");
	glUniform3f(camDirLoc, this->_camDir.x, this->_camDir.y, this->_camDir.z);
}
