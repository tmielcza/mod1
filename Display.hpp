// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Display.hpp                                        :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tmielcza <tmielcza@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/01/21 12:37:22 by tmielcza          #+#    #+#             //
//   Updated: 2015/01/30 15:07:59 by caupetit         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef DISPLAY_HPP
# define DISPLAY_HPP

# include <SDL.h>
# include <OpenGL/gl.h>

class Display
{
public:
	Display(void);
	~Display(void);
	Display(const Display& src);

//	void		addPixel(unsigned int x, unsigned int y, Uint32 col);
	void			draw(const int* vtypes, const int*vinfos);

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

	GLuint			compileShader(const std::string name, const GLenum flag) const;

};

#endif // DISPLAY_HPP
