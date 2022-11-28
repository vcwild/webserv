/*
 * Copyright 2022 ©️ vcwild under the GNU Affero General Public License v3.0.
 */
#include "BST.hpp"

void printNode( TreeNode *node )
{
    if ( node == NULL ) {
        std::cout << "not found!" << std::endl;
    } else {
        std::cout << "value: " << node->value << std::endl;
    }
}

int main()
{
    BST *tree = new BST();

    tree->insert( 5 );
    tree->insert( 2 );
    tree->insert( 1 );
    tree->insert( 3 );
    tree->insert( 7 );
    tree->insert( 2 );
    tree->insert( 8 );
    tree->insert( 10 );
    tree->insert( 6 );
    tree->insert( 4 );

    tree->print();

    printNode( tree->search( 4 ) );
    printNode( tree->search( 2 ) );
    printNode( tree->search( 13 ) );
    printNode( tree->search( 14 ) );
    printNode( tree->search( 8 ) );
    printNode( tree->search( 7 ) );

    delete tree;
    return 0;
}
