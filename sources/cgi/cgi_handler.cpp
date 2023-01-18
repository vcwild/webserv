/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi_handler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvieira- <mvieira-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 13:00:11 by mvieira-          #+#    #+#             */
/*   Updated: 2023/01/18 13:01:32 by mvieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi_handler.hpp"

CgiHandler::CgiHandler() 
{

}

CgiHandler::CgiHandler(Request cgi_request) : cgi_request(cgi_request) {

}

CgiHandler::~CgiHandler() {

}
