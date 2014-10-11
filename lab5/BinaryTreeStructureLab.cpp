#include <iostream>

using namespace std;

struct TreeNode {

        // An object of type TreeNode represents one node
        // in a binary tree.
    
   //  A variable in the main program of type pointer-to-TreeNode points to the binary tree that
   //  is used by the program:

   //  TreeNode *root;  // Pointer to the root node in the tree.
   //  root = NULL;  // Start with an empty tree.
     

   int item;    // The data in this node.
   TreeNode *left;  // Pointer to left subtree.
   TreeNode *right;   // Pointer to right subtree.
   
};  // end struct TreeNode

void nodeInsert(TreeNode *&root, int newItem) {
     
    if(root == NULL)
      root = new TreeNode;

    root->left = NULL;
    root->right = NULL;
    root->item = newItem;

}  // end treeInsert()

// traverses the tree and finds where to insert the item so that it's in alphabetical order
void treeInsert(TreeNode *&root, int item){

  if(root == NULL)
    nodeInsert(root, item);

  if(item < root->item)
    treeInsert(root->left, item);
  else if(item == root->item)
    return;
  else if(item > root->item)
    treeInsert(root->right, item);
  else if(root->left == NULL)
    nodeInsert(root->left, item);
  else if(root->right == NULL)
    nodeInsert(root->right, item);
  else return;


}
    //A recursive function named treeContains is used to search for a given item in the tree.
    
void treeListin(TreeNode *node) {
       // Print the items in the tree in-order
  if(node != NULL){
    treeListin(node->left);
    cout << node->item << " ";
    treeListin(node->right);
  }
} // end treeList()

void treeListpre(TreeNode *node) {
       // Print the items in the tree in preorder
  if(node != NULL){
    cout << node->item << " ";
    treeListpre(node->left);
    treeListpre(node->right);
  }
} // end treeList()



int main() {
 
   TreeNode *root;  // Pointer to the root node in a binary tree.  This
                    // tree is used in this program as a binary sort tree.
                    // The tree is not allowed to contain duplicate
                    // items.  When the tree is empty, root is null.
 
   root = NULL;  // Start with an empty tree.
 
   cout << "This program stores integers that you enter in a binary tree.\n";

   cout << "After each items is inserted, the contents of the tree\n";

   cout << "are displayed. \n";
   
   cout << "Duplicate entries are ignored.\n";

   while (true) {

           // Get one int from the user, insert it into the tree,
           // and print some information about the tree.
       cout << ("\n\nEnter an int to be inserted, or press return to end.\n");
       int item;  // The user's input.
       cin >> item;
       
       
          treeInsert(root,item);  // Add user's input to the tree.
          cout << "\nContents of tree:\n\n";
          treeListin(root);
          cout <<  "" << endl;
          treeListpre(root);

   }  // end while
   cout << "\n\nExiting program.\n\n";
}  // end main()
