// *********************************************************
// File         general.cpp
// Author       Matthew Molnar
// Date         4/18/2023
// Assignment   Assignment9
// Description:
//		ADT of a general tree. Contains methods
//		used to insert, and display certain nodes.
// Testing:
//		Thoroughly tested and works properly.
// *********************************************************

#include "general.h"
#include <stddef.h>
#include <iostream>

genTree::genTree()
{
	root = new genNode;

	cout<<"Constructing tree"<<endl;
	int ele;
	cout<<"Enter a value for the root"<<endl;
	std::cin >> ele;
	cout<<"User inputted "<<ele<<endl;
	root->item = ele;

	char cmnd = '0';
	while(cmnd != 'n')
	{
		cout<<"Would you like to add another element?"<<endl;

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

		if(cmnd == 'y')
		{
			bool Success = insert();
			if(!Success)
			{
				cout<<"Insert failed"<<endl;
			}
		}
	}
}

genNode* genTree::itemSearch(int itemVal, bool& Success)
{
	genNode* tmp = root;
	if(root == NULL)
	{
		return tmp;
	}
	genNode* searchedNode = itemSearch(tmp, itemVal, Success);
	if(Success)
	{
		cout<<"Item "<<itemVal<<" found in tree"<<endl;
	}
	else
	{
		cout<<"Item "<<itemVal<<" not found in tree"<<endl;
	}
	return searchedNode;
}

genNode* genTree::itemSearch(genNode* node, int itemVal, bool& Success)
{
	if(node->item == itemVal)
	{
		Success = true;
		return node;
	}
	if(node->firstChild != NULL)
	{
		genNode* tmp = node;
		tmp = itemSearch(node->firstChild, itemVal, Success);
		if(Success == true)
		{
			return tmp;
		}

		if(node->siblingList != NULL)
		{
			return itemSearch(node->siblingList,itemVal,Success);
		}
	}
	if(node->siblingList != NULL)
	{
		return itemSearch(node->siblingList,itemVal,Success);
	}
	return node;
}

bool genTree::insert()
{
	cout<<"Enter a parent node"<<endl;
	int parentVal;
	for(;;)
	{
		std::cin >> parentVal;
		if(cin.good())
		{
			break;
		}
		cin.clear();
		cin.ignore(120, '\n');
	}

	cout<<"User inputted parent value of "<<parentVal<<endl;
	bool parentSuccess = false;
	genNode* parentNode = itemSearch(parentVal,parentSuccess);

	if(parentSuccess == false)
	{
		cout<<"Parent not in tree"<<endl;
		return false;
	}

	cout<<"Enter a child to insert"<<endl;
	int childVal;
	for(;;)
	{
		std::cin >> childVal;
		if(cin.good())
		{
			break;
		}
		cin.clear();
		cin.ignore(120, '\n');
	}

	cout<<"User inputted child value of "<<childVal<<endl;
	bool childSuccess = false;
	itemSearch(childVal,childSuccess);
	if(childSuccess == true)
	{
		cout<<"Child already in tree"<<endl;
		return false;
	}

	genNode* newNode = new genNode;
	newNode->item = childVal;
	newNode->parent = parentNode;

	if(parentNode->firstChild == NULL)
	{
		parentNode->firstChild = newNode;
		return true;
	}

	genNode* tmp = parentNode->firstChild;
	while(tmp->siblingList != NULL)
	{
		tmp = tmp->siblingList;
	}
	tmp->siblingList = newNode;
	newNode->prevSibling = tmp;
	return true;
}

bool genTree::displayChildren(int parentVal)
{
	bool Success = false;
	genNode* parentNode = itemSearch(parentVal,Success);

	if(Success == false)
	{
		cout<<"Item does not exist in tree"<<endl;
		return false;
	}
	else if(parentNode->firstChild == NULL)
	{
		cout<<parentVal<<" has no children"<<endl;
		return true;
	}

	cout<<parentNode->firstChild->item<<endl;
	genNode* tmp = parentNode->firstChild;

	while(tmp->siblingList != NULL)
	{
		cout<<tmp->siblingList->item<<endl;
		tmp = tmp->siblingList;
	}
	return true;
}

bool genTree::displaySiblings(int siblingVal)
{
	bool Success = false;
	genNode* siblingNode = itemSearch(siblingVal,Success);

	if(Success == false)
	{
		return false;
	}
	else if(siblingNode->prevSibling == NULL && siblingNode->siblingList == NULL)
	{
		cout<<siblingVal<<" has no siblings"<<endl;
		return true;
	}

	displayLeftSiblings(siblingVal);
	displayRightSiblings(siblingVal);
	return true;
}

bool genTree::displayLeftSiblings(int siblingVal)
{
	bool Success = false;
	genNode* siblingNode = itemSearch(siblingVal, Success);

	if(Success == false)
	{
		return false;
	}
	else if(siblingNode->prevSibling == NULL)
	{
		cout<<siblingVal<<" has no left siblings"<<endl;
		return true;
	}

	genNode* tmp = siblingNode;
	genNode* tmp2 = siblingNode;
	while(tmp->prevSibling != NULL)
	{
		tmp = tmp->prevSibling;
	}
	while(tmp != tmp2)
	{
		cout<<tmp->item<<endl;
		tmp = tmp->siblingList;
	}

	return true;
}

bool genTree::displayRightSiblings(int siblingVal)
{
	bool Success = false;
	genNode* siblingNode = itemSearch(siblingVal, Success);

	if(Success == false)
	{
		return false;
	}
	else if(siblingNode->siblingList == NULL)
	{
		cout<<siblingVal<<" has no right siblings"<<endl;
		return true;
	}

	genNode* tmp = siblingNode->siblingList;
	cout<<tmp->item<<endl;
	while(tmp->siblingList != NULL)
	{
		cout<<tmp->siblingList->item<<endl;
		tmp = tmp->siblingList;
	}
	return true;
}

bool genTree::displayParent(int childVal)
{
	bool Success = false;
	genNode* childNode = itemSearch(childVal, Success);

	if(Success == false)
	{
		return false;
	}
	else if(childNode->parent == NULL)
	{
		cout<<childVal<<" has no parent"<<endl;
		return true;
	}
	cout<<childNode->parent->item<<endl;
	return true;
}

