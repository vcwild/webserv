/*
 * Copyright 2022 ©️ vcwild under the GNU Affero General Public License v3.0.
 */
#include "RBTree.hpp"

int main()
{
    RBTree tree;

    tree.insert( 1 );
    tree.insert( 2 );
    tree.insert( 3 );
    tree.insert( 4 );
    tree.insert( 5 );
    // tree.insert( 6 );
    // tree.insert( 7 );
    // tree.insert( 8 );
    // tree.insert( 9 );
    // tree.insert( 10 );

    tree.print();
}
