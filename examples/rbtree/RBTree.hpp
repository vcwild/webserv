/*
 * Copyright 2022 ©️ vcwild under the GNU Affero General Public License v3.0.
 */
#ifndef RBTREE_HPP
#define RBTREE_HPP

#include <iostream>
#include <string>

/**
 * @brief The color to set the Red-Black tree leave.
 *
 */
typedef enum tree_color { BLACK, RED } t_color;

/**
 * @brief The rotation direction of the Red-Black tree
 *
 */
typedef enum rotation_direction { LEFT, RIGHT } t_rot_dir;

typedef struct RBNode RBNode;

typedef RBNode *RBNodePtr;

struct RBNode
{
    RBNodePtr parent;
    RBNodePtr left;
    RBNodePtr right;
    int       value;
    t_color   color;
};

typedef class RedBlackTree {

public:
    RedBlackTree();
    ~RedBlackTree();

    RBNodePtr parent( RBNodePtr node );
    RBNodePtr grandparent( RBNodePtr node );
    RBNodePtr sibling( RBNodePtr node );
    RBNodePtr uncle( RBNodePtr node );

    bool      insert( int value );
    RBNodePtr search( int value );
    bool      remove( int value );

    RBNodePtr min( RBNodePtr node );
    RBNodePtr max( RBNodePtr node );
    RBNodePtr successor( RBNodePtr node );
    RBNodePtr predecessor( RBNodePtr node );

    /**
     * @brief  In order to move subtrees around within the binary search tree,
     * we define a subroutine TRANSPLANT, which replaces one subtree as a child
     * of its parent with another subtree. When TRANSPLANT replaces the subtree
     * rooted at node u with the subtree rooted at node v, node u's parent
     * becomes node v’s parent, and u's parent ends up having as its appropriate
     * child.
     *
     * @param u The node to be replaced.
     * @param v The node to replace u.
     */
    void transplant( RBNodePtr u, RBNodePtr v );

    void print();

private:
    RBNodePtr _root;
    RBNodePtr _nil;

    RBNodePtr _create( t_color color, int value );

    /**
     * @brief Rotates red black tree leaves. Requires an existing pointer to a
     * subtree node. Otherwise raises an exception.
     * G denotes the subtree grandparent node.
     *
     * @param subRoot The root of the subtree
     * @param dir The direction to rotate, 0 for left rotation, 1 for right
     * rotation
     * @return Returns the node for the root of the subtree
     */
    RBNodePtr _leftRotate( RBNodePtr subRoot );
    RBNodePtr _rightRotate( RBNodePtr subRoot );

    RBNodePtr _search( RBNodePtr node, int value );
    void      _destroy( RBNodePtr node );
    bool      _insert( int value );
    void      _insertFixup( RBNodePtr node );
    void      _print( RBNodePtr node, int level );
} RBTree;

#endif /* ********************************************************** RBTREE_H  \
        */
