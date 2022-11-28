/*
 * Copyright 2022 ©️ vcwild under the GNU Affero General Public License v3.0.
 */
#ifndef BST_HPP
#define BST_HPP

#include <iostream>

typedef struct TreeNode
{
    int       value;
    TreeNode *left;
    TreeNode *right;
} TreeNode;

typedef class BinarySearchTree {
public:
    BinarySearchTree();
    ~BinarySearchTree();

    bool      insert( int value );
    bool      remove( int value );
    void      print();
    TreeNode *search( int value );

private:
    TreeNode *_root;
    TreeNode *_search( TreeNode *node, int value );
    TreeNode *_create( int value );
    bool      _insert( TreeNode **node, int value );
    void      _print( TreeNode *node, int level );
    void      _destroy( TreeNode *node );
} BST;

#endif /* ************************************************** BINARYTREE_H  */
