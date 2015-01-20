// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   main.cpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: caupetit <marvin@42.fr>                    +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/01/20 18:01:21 by caupetit          #+#    #+#             //
//   Updated: 2015/01/20 19:59:03 by caupetit         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <SDL.h>

#include <iostream>

int main(void)
{
	/* Initialisation simple */
	if (SDL_Init(SDL_INIT_VIDEO) != 0 )
	{
		std::cout << "Échec de l'initialisation de la SDL "
				  << SDL_GetError() << std::endl;
		return -1;
	}

	{
        /* Création de la fenêtre */
		SDL_Window* pWindow = NULL;
		pWindow = SDL_CreateWindow("Ma première application SDL2",SDL_WINDOWPOS_UNDEFINED,
								   SDL_WINDOWPOS_UNDEFINED,
								   640,
								   480,
								   SDL_WINDOW_SHOWN);

		if( pWindow )
		{
			SDL_Delay(3000); /* Attendre trois secondes, que l'utilisateur voie la fenêtre */

			SDL_DestroyWindow(pWindow);
		}
		else
			std::cout << "Erreur de création de la fenêtre: "
					  << SDL_GetError() << std::endl;
	}
	
	SDL_Quit();

	return 0;
}
