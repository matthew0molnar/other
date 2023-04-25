// *********************************************************
// File         assign9.cpp
// Author       Matthew Molnar
// Date         4/18/2023
// Assignment   Assignment9
// Description:
//		Main program for Assignment 9, used to test
//		the functionality of the general tree ADT.
// Testing:
//		Thoroughly tested and works properly.
// *********************************************************

#include <fstream>
#include <cassert>
#include <iostream>
#include <string>
#include "general.h"

int main()
{
	// Command List:
	// 1: Search for item
	// 2: Display children of a node
	// 3: Display all siblings of a node
	// 4: Display left siblings of a node
	// 5: Display right siblings of a node
	// 6: Display parent of a node
	// 7: Add a node to the tree
	// 8: Exit loop

	int cmnd = 0;

	genTree myTree;

        while(cmnd != 8)
        {
                cout<<"Enter a command: "<<endl;
                cout<<"1: Search for an item"<<endl;
                cout<<"2: Display children of a node"<<endl;
                cout<<"3: Display all siblings of a node"<<endl;
                cout<<"4: Display left siblings of a node"<<endl;
                cout<<"5: Display right siblings of a node"<<endl;
                cout<<"6: Display parent of a node"<<endl;
		cout<<"7: Add a node to the tree"<<endl;
		cout<<"8: Exit loop"<<endl;

                for(;;)
                {
                        std::cin >> cmnd;
                        if(cin.good())
                        {
                                break;
                        }
                        cin.clear();
                        cin.ignore(120, '\n');
                }
                cout<<"User inputted "<<cmnd<<endl;
                switch(cmnd)
                {
                        case 1:
                        {
                                int ele;
				cout<<"What element would you like to look for?"<<endl;
				std::cin >> ele;
				cout<<"User inputted element "<<ele<<" to search for"<<endl;
				bool Success = false;
				myTree.itemSearch(ele,Success);
				cout<<endl;
				break;
			}
			case 2:
			{
				int ele;
				cout<<"What element would you like to see the children of?"<<endl;
				std::cin >> ele;
				cout<<"User inputted value " <<ele<<endl;
				myTree.displayChildren(ele);
				cout<<endl;
				break;
			}
			case 3:
			{
				int ele;
				cout<<"What element would you like to see the siblings of?"<<endl;
				std::cin >> ele;
				cout<<"User inputted value "<<ele<<endl;
				bool Success = myTree.displaySiblings(ele);
				if(!Success)
				{
					cout<<"Item does not exist in tree"<<endl;
				}
				cout<<endl;
				break;
			}
			case 4:
			{
				int ele;
				cout<<"What element would you like to see the left siblings of?"<<endl;
				std::cin >> ele;
				cout<<"User inputted value "<<ele<<endl;
				bool Success = myTree.displayLeftSiblings(ele);
				if(!Success)
				{
					cout<<"Item does not exist in tree"<<endl;
				}
				cout<<endl;
				break;
			}
			case 5:
			{
				int ele;
				cout<<"What element would you like to see the right siblings of?"<<endl;
				std::cin >> ele;
				cout<<"User inputted value"<<ele<<endl;
				bool Success = myTree.displayRightSiblings(ele);
				if(!Success)
				{
					cout<<"Item does not exist in tree"<<endl;
				}
				cout<<endl;
				break;
			}
			case 6:
			{
				int ele;
				cout<<"What element would you like to see the parent of?"<<endl;
				std::cin >> ele;
				cout<<"User inputted value"<<ele<<endl;
				bool Success = myTree.displayParent(ele);
				if(!Success)
				{
					cout<<"Item does not exist in tree"<<endl;
				}
				cout<<endl;
				break;
			}
                        case 7:
                        {
                                cout<<"User requested element insert"<<endl;
                                myTree.insert();
                                cout<<endl;
                                break;
			}
			case 8:
			{
				cout<<"Exiting menu"<<endl<<endl;
				break;
			}
			default:
			{
				cout<<"Invalid option"<<endl<<endl;
				break;
			}
		}
	}
}
