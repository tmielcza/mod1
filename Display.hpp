// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Display.hpp                                        :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tmielcza <tmielcza@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/01/21 12:37:22 by tmielcza          #+#    #+#             //
//   Updated: 2016/08/10 18:01:56 by tmielcza         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef DISPLAY_HPP
# define DISPLAY_HPP

# include <SDL.h>
# include <OpenGL/gl.h>
# include "Map.hpp"

class Display
{
public:
	Display(void);
	~Display(void);
	Display(const Display& src);

//	void		addPixel(unsigned int x, unsigned int y, Uint32 col);
	void			draw(const void* data, const int x, const int y, const int z);
	void			setHeights(const char* hs, const int x, const int y);
	void			rotateCam(void);
	void			setCamRotation(int x, int y);
	void			setZoom(const float x);

	unsigned int	getH(void) const;
	unsigned int	getW(void) const;

	void			initShaderProgram(std::string name);

	Display&	operator=(const Display& rhs);

private:
	SDL_Window*		_win;
	SDL_GLContext	_glc;
//	SDL_Renderer*	_ren;
//	SDL_Texture*	_tex;
//	Uint32*			_pix;
	unsigned int	_w;
	unsigned int	_h;
	GLuint			_prog;
	GLuint			_frag;
	GLuint			_mapTex;
	GLuint			_hTex;
	Map::point		_camDir;
	float			_rotv;
	float			_roth;
	float			_zoom;

	GLuint			compileShader(const std::string name, const GLenum flag) const;

};

#endif // DISPLAY_HPP
