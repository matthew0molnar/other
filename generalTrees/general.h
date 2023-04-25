// *********************************************************
// File       	general.h
// Author       Matthew Molnar
// Date         4/18/2023
// Assignment   Assignment9
// Description:
//		Header file for general tree ADT.
// Testing:
//		Thoroughly tested and works properly.
// *********************************************************

using namespace std;

struct genNode
{
	int item;
	genNode* firstChild;
	genNode* siblingList;
	genNode* prevSibling;
	genNode* parent;
};

class genTree
{
	public:
		genTree();

		genNode* itemSearch(int itemVal, bool& Success);
		bool insert();
		bool displayChildren(int parentVal);
		bool displaySiblings(int siblingVal);
		bool displayLeftSiblings(int siblingVal);
		bool displayRightSiblings(int siblingVal);
		bool displayParent(int childVal);

	private:
		genNode* itemSearch(genNode* node, int itemVal, bool& Success);

		genNode* root;
};

