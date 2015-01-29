// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Utils.cpp                                          :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tmielcza <tmielcza@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/01/29 16:45:14 by tmielcza          #+#    #+#             //
//   Updated: 2015/01/29 16:58:20 by tmielcza         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "Utils.hpp"
#include <fstream>
#include <sstream>

std::string		Utils::getFile(const std::string name)
{
	std::ifstream   fs(name.c_str());
	std::stringstream   buf;
    buf << fs.rdbuf();
    return (buf.str());
}
