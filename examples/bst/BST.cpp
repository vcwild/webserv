/*
 * Copyright 2022 ©️ vcwild under the GNU Affero General Public License v3.0.
 */
#include "BST.hpp"
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <stdio.h>
/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

BST::BinarySearchTree() : _root( NULL ) {}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

BST::~BinarySearchTree()
{
    if ( _root != NULL ) {
        _destroy( _root );
    }
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

/*
** --------------------------------- METHODS ----------------------------------
*/

bool BST::insert( int value ) { return _insert( &_root, value ); }

bool BST::remove( int value ) { return true; }

void BST::print() { _print( _root, 0 ); }

void BST::_print( TreeNode *node, int level )
{
    if ( node != NULL ) {
        if ( node->right ) {
            _print( node->right, level + 4 );
        }
        if ( level ) {
            std::cout << std::setw( level ) << ' ';
        }
        if ( node->right )
            std::cout << " /\n" << std::setw( level ) << ' ';
        std::cout << node->value << "\n ";
        if ( node->left ) {
            std::cout << std::setw( level ) << ' ' << " \\" << std::endl;
            _print( node->left, level + 4 );
        }
    }
    std::cout << std::endl;
}

TreeNode *BST::search( int value ) { return _search( _root, value ); }

TreeNode *BST::_search( TreeNode *node, int value )
{
    if ( node == NULL )
        return NULL;
    if ( value == node->value )
        return node;
    if ( value < node->value )
        return _search( node->left, value );
    return _search( node->right, value );
}

TreeNode *BST::_create( int value )
{
    TreeNode *node = new TreeNode;
    node->value    = value;
    node->left     = NULL;
    node->right    = NULL;

    return node;
}

bool BST::_insert( TreeNode **node, int value )
{
    if ( *node == NULL )
        return ( *node = _create( value ), true );
    if ( value == ( *node )->value )
        return false;
    if ( value < ( *node )->value )
        return _insert( &( *node )->left, value );
    return _insert( &( *node )->right, value );
}

void BST::_destroy( TreeNode *node )
{
    if ( node != NULL ) {
        _destroy( node->left );
        _destroy( node->right );
        delete node;
    }
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

/* ************************************************************************** */
