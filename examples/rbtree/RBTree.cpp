/*
 * Copyright 2022 ©️ vcwild under the GNU Affero General Public License v3.0.
 */
#include "RBTree.hpp"
#include <cassert>
#include <iomanip>

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

RBTree::RedBlackTree() : _root( NULL )
{
    RBNodePtr _nil = new RBNode;

    _nil->color  = BLACK;
    _nil->left   = NULL;
    _nil->right  = NULL;
    _nil->parent = NULL;
    _nil->value  = 0;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

RBTree::~RedBlackTree()
{
    if ( _root != NULL ) {
        _destroy( _root );
    }
    _destroy( _nil );
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

// bool RBTree::operator==( RedBlackTree const &rhs )
// {
//     if ( this == &rhs ) {
//         return true;
//     }
//     return false;
// }

/*
** --------------------------------- METHODS ----------------------------------
*/
RBNodePtr RBTree::grandparent( RBNodePtr node )
{
    if ( node != NULL && node->parent != NULL ) {
        return node->parent->parent;
    } else {
        return NULL;
    }
}

RBNode *RBTree::uncle( RBNodePtr node )
{
    RBNodePtr g = grandparent( node );
    if ( g == NULL ) {
        return NULL;
    }
    if ( node->parent == g->left ) {
        return g->right;
    } else {
        return g->left;
    }
}

RBNode *RBTree::sibling( RBNodePtr node )
{
    if ( node->parent == NULL ) {
        return NULL;
    }
    if ( node == node->parent->left ) {
        return node->parent->right;
    } else {
        return node->parent->left;
    }
}

void RBTree::print() { _print( _root, 0 ); }

RBNodePtr RBTree::search( int value ) { return _search( _root, value ); }

RBNodePtr RBTree::min( RBNodePtr node )
{
    while ( node->left != _nil ) {
        node = node->left;
    }
    return node;
}

RBNodePtr RBTree::max( RBNodePtr node )
{
    while ( node->right != _nil ) {
        node = node->right;
    }
    return node;
}

RBNodePtr RBTree::successor( RBNodePtr node )
{
    if ( node->right != _nil ) {
        return min( node->right );
    }

    RBNodePtr parent = node->parent;

    while ( parent != _nil && node == parent->right ) {
        node   = parent;
        parent = parent->parent;
    }

    return parent;
}

RBNodePtr RBTree::predecessor( RBNodePtr node )
{
    if ( node->left != _nil ) {
        return max( node->left );
    }

    RBNodePtr parent = node->parent;

    while ( parent != _nil && node == parent->left ) {
        node   = parent;
        parent = parent->parent;
    }

    return parent;
}

bool RBTree::insert( int value )
{

    if ( _root == NULL ) {
        _root = _create( BLACK, value );
        return true;
    }

    RBNodePtr node   = _create( RED, value );
    RBNodePtr parent = _root;
    RBNodePtr child  = _root;

    while ( child != _nil ) {
        parent = child;
        if ( node->value < child->value ) {
            child = child->left;
        } else if ( node->value > child->value ) {
            child = child->right;
        } else {
            return false;
        }
    }

    node->parent = parent;

    if ( node->value < parent->value ) {
        parent->left = node;
    } else {
        parent->right = node;
    }

    _insertFixup( node );

    return true;
}

void RBTree::transplant( RBNodePtr u, RBNodePtr v )
{
    if ( u->parent == NULL ) {
        _root = v;
    } else if ( u == u->parent->left ) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }

    if ( v != NULL ) {
        v->parent = u->parent;
    }
}

/*
** ----------------------------- PRIVATE METHODS ----------------------------
*/

RBNodePtr RBTree::_create( t_color color, int value )
{
    RBNodePtr node = new RBNode;
    node->color    = color;
    node->left     = _nil;
    node->right    = _nil;
    node->parent   = _nil;
    node->value    = value;

    return node;
}

RBNodePtr RBTree::_leftRotate( RBNodePtr subRoot )
{
    RBNodePtr right = subRoot->right;

    subRoot->right = right->left;

    if ( right->left != _nil ) {
        right->left->parent = subRoot;
    }

    right->parent = subRoot->parent;

    if ( subRoot->parent == _nil ) {
        _root = right;
    } else if ( subRoot == subRoot->parent->left ) {
        subRoot->parent->left = right;
    } else {
        subRoot->parent->right = right;
    }

    right->left     = subRoot;
    subRoot->parent = right;

    return right;
}
RBNodePtr RBTree::_rightRotate( RBNodePtr subRoot )
{
    RBNodePtr left = subRoot->left;

    subRoot->left = left->right;

    if ( left->right != _nil ) {
        left->right->parent = subRoot;
    }

    left->parent = subRoot->parent;

    if ( subRoot->parent == _nil ) {
        _root = left;
    } else if ( subRoot == subRoot->parent->right ) {
        subRoot->parent->right = left;
    } else {
        subRoot->parent->left = left;
    }

    left->right     = subRoot;
    subRoot->parent = left;

    return left;
}

void RBTree::_insertFixup( RBNodePtr node )
{
    while ( node->parent->color == RED ) {
        if ( node->parent == grandparent( node )->left ) {
            RBNodePtr uncle = grandparent( node )->right;
            if ( uncle->color == RED ) {
                node->parent->color        = BLACK;
                uncle->color               = BLACK;
                grandparent( node )->color = RED;
                node                       = grandparent( node );
            } else {
                if ( node == node->parent->right ) {
                    node = node->parent;
                    _leftRotate( node );
                }
                node->parent->color        = BLACK;
                grandparent( node )->color = RED;
                _rightRotate( grandparent( node ) );
            }
        } else {
            RBNodePtr uncle = grandparent( node )->left;
            if ( uncle->color == RED ) {
                node->parent->color        = BLACK;
                uncle->color               = BLACK;
                grandparent( node )->color = RED;
                node                       = grandparent( node );
            } else {
                if ( node == node->parent->left ) {
                    node = node->parent;
                    _rightRotate( node );
                }
                node->parent->color        = BLACK;
                grandparent( node )->color = RED;
                _leftRotate( grandparent( node ) );
            }
        }
    }
    _root->color = BLACK;
}

void RBTree::_destroy( RBNodePtr node )
{
    if ( node != NULL ) {
        _destroy( node->left );
        _destroy( node->right );
        delete node;
    }
    _destroy( _nil );
}

RBNodePtr RBTree::_search( RBNodePtr node, int value )
{
    if ( node == NULL )
        return NULL;
    if ( value == node->value )
        return node;
    if ( value < node->value )
        return _search( node->left, value );
    return _search( node->right, value );
}

void RBTree::_print( RBNodePtr node, int level )
{
    std::string nodeColor;

    nodeColor = node->color == RED ? "\e[31m" : "\e[33m";

    if ( node->right ) {
        _print( node->right, level + 4 );
    }
    if ( level ) {
        std::cout << std::setw( level ) << ' ';
    }
    if ( node->right )
        std::cout << " /\n" << std::setw( level ) << ' ';
    std::cout << nodeColor << node->value << "\e[0m\n ";
    if ( node->left ) {
        std::cout << std::setw( level ) << ' ' << " \\" << std::endl;
        _print( node->left, level + 4 );
    }
    std::cout << std::endl;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

/* **************************************************************************
 */
