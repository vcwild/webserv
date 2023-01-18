/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi_handler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvieira- <mvieira-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 13:00:11 by mvieira-          #+#    #+#             */
/*   Updated: 2023/01/18 13:04:12 by mvieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi_handler.hpp"

Cgi_handler::Cgi_handler() {

}

Cgi_handler::Cgi_handler(Request cgi_request) : cgi_request(cgi_request) {

}

Cgi_handler::~Cgi_handler() {

}