/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vwildner <vwildner@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 09:44:34 by mvieira-          #+#    #+#             */
/*   Updated: 2023/01/24 23:13:48 by vwildner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./server/Server.hpp"
#include "parsers.hpp"
#include "webserv.hpp"
#include <iostream>

int main( int argc, char **argv )
{
    if ( argc > 5 )
        std::cout << "Error" << std::endl;

    std::ifstream test( argv[1] );
    if ( !test ) {
        std::cout << "Error: invalid path" << std::endl;
        return 1;
    }

    vector<Config> conf = parseConfig( argv[1] );
    Server         server( conf );
    return 0;
}
