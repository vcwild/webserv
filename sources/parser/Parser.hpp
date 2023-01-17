/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvieira- <mvieira-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 09:28:15 by mvieira-          #+#    #+#             */
/*   Updated: 2023/01/03 09:37:13 by mvieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP

#include "webserv.hpp"

class Parser {

  private:
    char *file_name;
    std::vector<Config> servers_conf;

  public:
    Parser();
    Parser(char  *file_name);
    ~Parser();

    int parse();
    std::vector<Config> getServersConf() const;

};

#endif