// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Display.hpp                                        :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tmielcza <tmielcza@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/01/21 12:37:22 by tmielcza          #+#    #+#             //
//   Updated: 2015/01/25 17:30:15 by caupetit         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef DISPLAY_HPP
# define DISPLAY_HPP

# include <SDL.h>

class Display
{
public:
	Display(void);
	~Display(void);
	Display(const Display& src);

	void		addPixel(unsigned int x, unsigned int y, Uint32 col);
	void		draw(void);

	unsigned int	getH(void) const;
	unsigned int	getW(void) const;

	Display&	operator=(const Display& rhs);

private:
	SDL_Window*		_win;
	SDL_Renderer*	_ren;
	SDL_Texture*	_tex;
	Uint32*			_pix;
	unsigned int	_w;
	unsigned int	_h;
};

#endif // DISPLAY_HPP
