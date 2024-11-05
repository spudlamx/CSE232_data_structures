/***********************************************************************
 * Header:
 *    BST
 * Summary:
 *    Our custom implementation of a BST for set and for map
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        BST                 : A class that represents a binary search tree
 *        BST::iterator       : An iterator through BST
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once

#ifdef DEBUG
#define debug(x) x
#else // !DEBUG
#define debug(x)
#endif // !DEBUG

#include <iostream>
#include <cassert>
#include <utility>
#include <memory>     // for std::allocator
#include <functional> // for std::less
#include <utility>    // for std::pair

class TestBST; // forward declaration for unit tests
class TestSet;
class TestMap;

namespace custom
{

   template <typename TT>
   class set;
   template <typename KK, typename VV>
   class map;

/*****************************************************************
 * BINARY SEARCH TREE
 * Create a Binary Search Tree
 *****************************************************************/
template <typename T>
class BST
{
   friend class ::TestBST; // give unit tests access to the privates
   friend class ::TestSet;
   friend class ::TestMap;

   template <class TT>
   friend class custom::set;

   template <class KK, class VV>
   friend class custom::map;
public:
   //
   // Construct
   //

   BST();
   BST(const BST &  rhs);
   BST(      BST && rhs);
   BST(const std::initializer_list<T>& il);
   ~BST();

   //
   // Assign
   //

   BST & operator = (const BST &  rhs);
   BST & operator = (      BST && rhs);
   BST & operator = (const std::initializer_list<T>& il);
   void swap(BST & rhs);

   //
   // Iterator
   //

   class iterator;
   iterator   begin() const noexcept;
   iterator   end()   const noexcept { return iterator(nullptr); }

   //
   // Access
   //

   iterator find(const T& t);

   // 
   // Insert
   //

   std::pair<iterator, bool> insert(const T&  t, bool keepUnique = false);
   std::pair<iterator, bool> insert(      T&& t, bool keepUnique = false);

   //
   // Remove
   // 

   iterator erase(iterator& it);
   void   clear() noexcept;
   

   // 
   // Status
   //

   bool   empty() const noexcept { return size() == 0; }
   size_t size()  const noexcept { return numElements;   }
   
private:

   class BNode;
   BNode * root;              // root node of the binary search tree
   size_t numElements;        // number of elements currently in the tree
   void copyTree(typename BST<T>::BNode* srcNode, typename BST<T>::BNode*& destNode);
   void   clear(BNode* node) noexcept;
 

};


/*****************************************************************
 * BINARY NODE
 * A single node in a binary tree. Note that the node does not know
 * anything about the properties of the tree so no validation can be done.
 *****************************************************************/
template <typename T>
class BST <T> :: BNode
{
public:
   // 
   // Construct
   //
   BNode()
   {
      pLeft = pRight = pParent = nullptr;
      isRed = true;
   }
   BNode(const T &  t) : data(t)
   {
      pLeft = pRight = pParent = nullptr;
      isRed = true;
   }
   BNode(T && t) :data(std::move(t))
   {
      pLeft = pRight = pParent = nullptr;
      isRed = true;
   }

   //
   // Insert
   //
   void addLeft (BNode * pNode);
   void addRight(BNode * pNode);
   void addLeft (const T &  t);
   void addRight(const T &  t);
   void addLeft(       T && t);
   void addRight(      T && t);

   // 
   // Status
   //
   bool isRightChild() const {return pParent && pParent->pRight == this;}
   bool isLeftChild() const {return pParent && pParent->pLeft == this;}

   // balance the tree
   void balance(BNode* root);

#ifdef DEBUG
   //
   // Verify
   //
   std::pair <T,T> verifyBTree() const;
   int findDepth() const;
   bool verifyRedBlack(int depth) const;
   int computeSize() const;
#endif // DEBUG

   //
   // Data
   //
   T data;                  // Actual data stored in the BNode
   BNode* pLeft;          // Left child - smaller
   BNode* pRight;         // Right child - larger
   BNode* pParent;        // Parent
   bool isRed;              // Red-black balancing stuff
   
};

/**********************************************************
 * BINARY SEARCH TREE ITERATOR
 * Forward and reverse iterator through a BST
 *********************************************************/
template <typename T>
class BST <T> :: iterator
{
   friend class ::TestBST; // give unit tests access to the privates
   friend class ::TestSet;
   friend class ::TestMap;

   template <class KK, class VV>
   friend class custom::map;
public:
   // constructors and assignment
   iterator(BNode * p = nullptr)          
   {
      pNode = p;
   }
   iterator(const iterator & rhs)         
   {
      pNode = rhs.pNode;
   }
   iterator & operator = (const iterator & rhs)
   {
      pNode = rhs.pNode;
      return *this;
   }

   // compare
   bool operator == (const iterator & rhs) const
   {
      return pNode == rhs.pNode;
   }
   bool operator != (const iterator & rhs) const
   {
      return pNode != rhs.pNode;
   }

   // de-reference. Cannot change because it will invalidate the BST
   const T & operator * () const 
   {
      return pNode->data;
   }

   // increment and decrement
   iterator & operator ++ ();
   iterator   operator ++ (int postfix)
   {
      iterator temp = *this;
      ++*this;
      return temp;
   }
   iterator & operator -- ();
   iterator   operator -- (int postfix)
   {
      iterator temp = *this;
      --*this;
      return temp;
   }

   // must give friend status to remove so it can call getNode() from it
   friend BST <T> :: iterator BST <T> :: erase(iterator & it);

private:
   
    // the node
    BNode * pNode;
};


/*********************************************
 *********************************************
 *********************************************
 ******************** BST ********************
 *********************************************
 *********************************************
 *********************************************/


 /*********************************************
  * BST :: DEFAULT CONSTRUCTOR
  ********************************************/
template <typename T>
BST <T> ::BST()
{
   numElements = 0;
   root = nullptr;
}

/*********************************************
 * BST :: COPY CONSTRUCTOR
 * Copy one tree to another
 ********************************************/
template <typename T>
BST <T> :: BST ( const BST<T>& rhs) 
{
   numElements = 0;
   root = nullptr;

   *this = rhs;

}

/*********************************************
 * BST :: MOVE CONSTRUCTOR
 * Move one tree to another
 ********************************************/
template <typename T>
BST <T> :: BST(BST <T> && rhs) 
{
   numElements = rhs.numElements;
   root = rhs.root;
   
   rhs.root = nullptr;
   rhs.numElements = 0;
}

/*********************************************
 * BST :: INITIALIZER LIST CONSTRUCTOR
 * Create a BST from an initializer list
 ********************************************/
template <typename T>
BST <T> ::BST(const std::initializer_list<T>& il)
{
   numElements = 99;
   root = new BNode;
}

/*********************************************
 * BST :: DESTRUCTOR
 ********************************************/
template <typename T>
BST <T> :: ~BST()
{
   clear();
}


/*********************************************
 * BST :: ASSIGNMENT OPERATOR
 * Copy one tree to another
 ********************************************/
template <typename T>
BST<T>& BST<T>::operator=(const BST& rhs)
{
    if (this == &rhs)  // Self-assignment check
        return *this;
   
    // Step 1: Clear the destination tree if the source is empty
    if (rhs.root == nullptr)
    {
        clear();
        return *this;  // No need to do further work if rhs is empty
    }

    // Step 2: Reuse the existing nodes in the destination tree by updating their data
    copyTree(rhs.root, root);

    // Step 3: Copy the number of elements from rhs
    numElements = rhs.numElements;

    return *this;
}

/*********************************************
 * BST :: COPYTREE
 * Reuse nodes or create new ones in the destination tree based on the source
 ********************************************/
template <typename T>
void BST<T>::copyTree(typename BST<T>::BNode* srcNode, typename BST<T>::BNode*& destNode)
{
    if (srcNode == nullptr) {
       clear(destNode);
        return;  // If the source node is null, do nothing
    }

    // Case 1: If destination node is null, create a new node in the destination tree
    if (destNode == nullptr) {
        destNode = new BNode(srcNode->data);
        destNode->isRed = srcNode->isRed;  // Copy the color (isRed) for Red-Black Tree
    }
    // Case 2: If destination node exists, update its data
    else {
        destNode->data = srcNode->data;
        destNode->isRed = srcNode->isRed;  // Update color to match source node's color
    }

    // Recursively copy the left and right children
    copyTree(srcNode->pLeft, destNode->pLeft);
    copyTree(srcNode->pRight, destNode->pRight);

    // Ensure parent pointers are updated after changing the subtrees
    if (destNode->pLeft) destNode->pLeft->pParent = destNode;
    if (destNode->pRight) destNode->pRight->pParent = destNode;
}


/*********************************************
 * BST :: ASSIGNMENT OPERATOR with INITIALIZATION LIST
 * Copy nodes onto a BTree
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (const std::initializer_list<T>& il)
{
   for (const T& t : il) {
      insert(t);
   }
   return *this;
}

/*********************************************
 * BST :: ASSIGN-MOVE OPERATOR
 * Move one tree to another
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (BST <T> && rhs)
{
    if (this == &rhs)  // Self-assignment check
        return *this;

    clear();

   swap(rhs);

    return *this;
}

/*********************************************
 * BST :: SWAP
 * Swap two trees
 ********************************************/
template <typename T>
void BST<T>::swap(BST<T>& rhs)
{
    // Swap the root nodes of the two trees
    std::swap(this->root, rhs.root);

    // Swap the number of elements (size) of the two trees
    std::swap(this->numElements, rhs.numElements);
}


/*****************************************************
 * BST :: INSERT
 * Insert a node at a given location in the tree
 ****************************************************/
template <typename T>
std::pair<typename BST<T>::iterator, bool> BST<T>::insert(const T& t, bool keepUnique) {
    // If the tree is empty, insert the first node
    if (!root) {
        root = new BNode(t);
        root->isRed = false; // The root should always be black
        numElements++;
        return { iterator(root), true };
    }

    BNode* pCurrent = root;
   

    // Traverse the tree to find the correct insertion point
   while (pCurrent) {
      
      // Compare using operator< and operator==
      if (keepUnique and t == pCurrent->data) {
         return { iterator(pCurrent), false }; // Duplicate found, no insertion
      }
      else
      {
         
          if (t < pCurrent->data) {
            if (pCurrent->pLeft) {
               pCurrent = pCurrent->pLeft;
            } else {
               // Create new node if there's no left child
               BNode* newNode = new BNode(t);
               pCurrent->addLeft(newNode); // This should invoke the copy constructor
               newNode->balance(root);
               numElements++;
               return { iterator(pCurrent->pLeft), true }; // New node inserted
            }
         }
         else
         { // Using the inverted comparison here
            
            if (pCurrent->pRight)
            {
               pCurrent = pCurrent->pRight;
            }
            else
            {
               // Create new node if there's no right child
               BNode* newNode = new BNode(t);
               pCurrent->addRight(newNode); // This should invoke the copy constructor
               newNode->balance(root);
               numElements++;
               return { iterator(pCurrent->pRight), true }; // New node inserted
               
            }
         }
      }
   }

    // Control should never reach here
    return { iterator(nullptr), false };
}







template <typename T>
std::pair<typename BST <T> ::iterator, bool> BST <T> ::insert(T && t, bool keepUnique)

{
    if (!root) {
        root = new BNode(std::move(t));  // Move the value into the node
        root->isRed = false;  // The root should always be black
        numElements++;
        return { iterator(root), true };
    }

    BNode* pCurrent = root;

    // Traverse the tree to find the correct insertion point
    while (pCurrent) {
        if (keepUnique && t == pCurrent->data) {
            return { iterator(pCurrent), false };  // Duplicate found, no insertion
        }
        else {
            if (t < pCurrent->data) {
                if (pCurrent->pLeft) {
                    pCurrent = pCurrent->pLeft;
                } else {
                    // Create new node using move semantics
                    BNode* newNode = new BNode(std::move(t));
                    pCurrent->addLeft(newNode);
                    newNode->balance(root);
                    numElements++;
                    return { iterator(pCurrent->pLeft), true };  // New node inserted
                }
            }
            else {  // t > pCurrent->data
                if (pCurrent->pRight) {
                    pCurrent = pCurrent->pRight;
                } else {
                    // Create new node using move semantics
                    BNode* newNode = new BNode(std::move(t));
                    pCurrent->addRight(newNode);
                    newNode->balance(root);
                    numElements++;
                    return { iterator(pCurrent->pRight), true };  // New node inserted
                }
            }
        }
    }

    // Control should never reach here
    return { iterator(nullptr), false };
}


/*************************************************
 * BST :: ERASE
 * Remove a given node as specified by the iterator
 ************************************************/
template <typename T>
typename BST<T>::iterator BST<T>::erase(iterator & it) {
    if (!it.pNode) {
        return end(); // Return end iterator if the node is null
    }

    BNode* nodeToDelete = it.pNode; // Node to be deleted
    iterator nextIterator = ++it; // Move iterator to the next node

    --numElements; // Decrement the number of elements before deletion

    // Case 1: Node is the root
    if (nodeToDelete == root) {
        root = nullptr; // Reset root to null since we deleted it
        delete nodeToDelete;
        return end(); // Return end as there's no next node
    }

    // Case 2: Node has no children (leaf node)
    if (!nodeToDelete->pLeft && !nodeToDelete->pRight) {
        if (nodeToDelete->pParent->pLeft == nodeToDelete) {
            nodeToDelete->pParent->pLeft = nullptr; // Disconnect from parent
        } else {
            nodeToDelete->pParent->pRight = nullptr; // Disconnect from parent
        }
    }
    // Case 3: Node has two children
    else if (nodeToDelete->pLeft && nodeToDelete->pRight) {
        BNode* successor = nextIterator.pNode; // Use the successor found by the iterator

        // Disconnect the successor from its parent
        if (successor->pParent) {
           if (successor->isLeftChild()) {
              successor->pParent->addLeft(successor->pRight); // Link the right child
            } else {
                successor->pParent->addRight(successor->pRight); // Link the right child
            }
        }

       // Replace nodeToDelete with the successor
        if (nodeToDelete->isLeftChild()) {
            nodeToDelete->pParent->addLeft(successor);
        } else {
            nodeToDelete->pParent->addRight(successor);
        }

        // Now link the children of nodeToDelete to the successor
        successor->addLeft(nodeToDelete->pLeft);
        successor->addRight(nodeToDelete->pRight);

    }
    // Case 4: Node has one child (left or right)
    else {
        BNode* child = nodeToDelete->pLeft ? nodeToDelete->pLeft : nodeToDelete->pRight;
        child->pParent = nodeToDelete->pParent; // Connect child to parent

        if (nodeToDelete->pParent) {
            if (nodeToDelete->pParent->pLeft == nodeToDelete) {
                nodeToDelete->pParent->pLeft = child; // Link child to parent
            } else {
                nodeToDelete->pParent->pRight = child; // Link child to parent
            }
        } else {
            root = child; // Update root if necessary
        }
    }

    // Delete the node
    delete nodeToDelete;
   

    // Return the iterator to the next node after deletion
    return nextIterator;
}




/*****************************************************
 * BST :: CLEAR
 * Removes all the BNodes from a tree
 ****************************************************/
template <typename T>
void BST<T>::clear() noexcept
{
    // Start clearing from the root node if it's not already null
    if (root != nullptr) {
        clear(root);
        root = nullptr;  // After clearing, ensure root is nullptr.
    }
}

template <typename T>
void BST<T>::clear(BNode* node) noexcept
{
    if (node == nullptr) {
        return;  // Base case: if the node is nullptr, do nothing.
    }

    // Recursively clear the left and right subtrees first
    if (node->pLeft) {
        clear(node->pLeft);  // Clear left child
    }
    if (node->pRight) {
        clear(node->pRight);  // Clear right child
    }

    // After children are cleared, erase the current node
    // If you're using a more complex erase method, you can keep it.
   iterator it = iterator(node);
    erase(it);  // Or simply delete node if not using erase

    // Optionally: After erasing the node, it should no longer be accessed.
    // But since it's passed by reference, it's not required to nullify the pointer.
}



/*****************************************************
 * BST :: BEGIN
 * Return the first node (left-most) in a binary search tree
 ****************************************************/
template <typename T>
typename BST<T>::iterator BST<T>::begin() const noexcept {
   if (empty())
   {
      return end(); // Return end() if the tree is empty
   }
   else
   {
      BNode* p = root; // Start from the root
      while (p->pLeft) { // Traverse to the leftmost node
         p = p->pLeft;
      }
      return iterator(p); // Return an iterator pointing to the leftmost node
   }
}



/****************************************************
 * BST :: FIND
 * Return the node corresponding to a given value
 ****************************************************/
template <typename T>
typename BST <T> :: iterator BST<T> :: find(const T & t)
{
   BNode* pCurrent = root;

   while (pCurrent)
   {
      if (t == pCurrent->data)
      {
         // Value found, return the iterator to this node
         return iterator(pCurrent);
         
      }
      else if (t < pCurrent->data)
      {
         pCurrent = pCurrent->pLeft; // Go left
      }
      else
      {
         pCurrent = pCurrent->pRight; // Go right
      }
   }

   // Value not found, return end iterator
   return end();
}


/******************************************************
 ******************************************************
 ******************************************************
 *********************** B NODE ***********************
 ******************************************************
 ******************************************************
 ******************************************************/

 
/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST<T>::BNode::addLeft(BNode *pNode) {
    // Ensure pNode is not null and points to a valid object
    if (pNode == nullptr) {
        this->pLeft = nullptr; // Handle null case
        return;
    }

    this->pLeft = pNode;    // Assign the left child
    pNode->pParent = this;  // Set parent to this node
}


/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addRight (BNode * pNode)
{
   if (pNode == nullptr)
   {
      this->pRight = nullptr; // Check for null node
      return;
   }
   pNode->pParent = this;
   this->pRight = pNode;
}

/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST<T> :: BNode :: addLeft (const T & t)
{
   this->addLeft(new BNode(t));
}

/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST<T> ::BNode::addLeft(T && t)
{
   this->addLeft(new BNode(std::move(t)));
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addRight (const T & t)
{
   this->addRight(new BNode(t));
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> ::BNode::addRight(T && t)
{
   this->addRight(new BNode(std::move(t)));
}

#ifdef DEBUG
/****************************************************
 * BINARY NODE :: FIND DEPTH
 * Find the depth of the black nodes. This is useful for
 * verifying that a given red-black tree is valid
 ****************************************************/
template <typename T>
int BST <T> :: BNode :: findDepth() const
{
   // if there are no children, the depth is ourselves
   if (pRight == nullptr && pLeft == nullptr)
      return (isRed ? 0 : 1);

   // if there is a right child, go that way
   if (pRight != nullptr)
      return (isRed ? 0 : 1) + pRight->findDepth();
   else
      return (isRed ? 0 : 1) + pLeft->findDepth();
}

/****************************************************
 * BINARY NODE :: VERIFY RED BLACK
 * Do all four red-black rules work here?
 ***************************************************/
template <typename T>
bool BST <T> :: BNode :: verifyRedBlack(int depth) const
{
   bool fReturn = true;
   depth -= (isRed == false) ? 1 : 0;

   // Rule a) Every node is either red or black
   assert(isRed == true || isRed == false); // this feels silly

   // Rule b) The root is black
   if (pParent == nullptr)
      if (isRed == true)
         fReturn = false;

   // Rule c) Red nodes have black children
   if (isRed == true)
   {
      if (pLeft != nullptr)
         if (pLeft->isRed == true)
            fReturn = false;

      if (pRight != nullptr)
         if (pRight->isRed == true)
            fReturn = false;
   }

   // Rule d) Every path from a leaf to the root has the same # of black nodes
   if (pLeft == nullptr && pRight && nullptr)
      if (depth != 0)
         fReturn = false;
   if (pLeft != nullptr)
      if (!pLeft->verifyRedBlack(depth))
         fReturn = false;
   if (pRight != nullptr)
      if (!pRight->verifyRedBlack(depth))
         fReturn = false;

   return fReturn;
}


/******************************************************
 * VERIFY B TREE
 * Verify that the tree is correctly formed
 ******************************************************/
template <typename T>
std::pair <T, T> BST <T> :: BNode :: verifyBTree() const
{
   // largest and smallest values
   std::pair <T, T> extremes;
   extremes.first = data;
   extremes.second = data;

   // check parent
   if (pParent)
      assert(pParent->pLeft == this || pParent->pRight == this);

   // check left, the smaller sub-tree
   if (pLeft)
   {
      assert(!(data < pLeft->data));
      assert(pLeft->pParent == this);
      pLeft->verifyBTree();
      std::pair <T, T> p = pLeft->verifyBTree();
      assert(!(data < p.second));
      extremes.first = p.first;

   }

   // check right
   if (pRight)
   {
      assert(!(pRight->data < data));
      assert(pRight->pParent == this);
      pRight->verifyBTree();

      std::pair <T, T> p = pRight->verifyBTree();
      assert(!(p.first < data));
      extremes.second = p.second;
   }

   // return answer
   return extremes;
}

/*********************************************
 * COMPUTE SIZE
 * Verify that the BST is as large as we think it is
 ********************************************/
template <typename T>
int BST <T> :: BNode :: computeSize() const
{
   return 1 +
      (pLeft  == nullptr ? 0 : pLeft->computeSize()) +
      (pRight == nullptr ? 0 : pRight->computeSize());
}
#endif // DEBUG

/******************************************************
 * BINARY NODE :: BALANCE
 * Balance the tree from a given location
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: balance(BNode* root)
{
   BNode* pGranny = pParent ? pParent->pParent : nullptr;
   BNode* pSibling = pParent ? (pParent->pLeft == this ? pParent->pRight : pParent->pLeft) : nullptr;
   BNode* pGreatGranny = pGranny ? pGranny->pParent : nullptr;
   BNode* pAunt = pGranny ? (pGranny->pLeft == pParent ? pGranny->pRight : pGranny->pLeft) : nullptr;

   // Case 1: if we are the root, then color ourselves black and call it a day.
   if (pParent == nullptr)
   {
      isRed = false;
      return;
   }

   // Case 2: if the parent is black, then there is nothing left to do
   if (!pParent->isRed)
   {
      return;
   }
   // Case 3: if the aunt is red, then just recolor
   if (pAunt and pParent->isRed and pAunt->isRed and !pGranny->isRed)
   {
      pParent->isRed = false;
      pAunt->isRed = false;
      pGranny->isRed = true;
      pGranny->balance(root);
      return;
   }

   // Case 4: if the aunt is black or non-existant, then we need to rotate
   if (pParent->isRed and (!pAunt or !pAunt->isRed) and !pGranny->isRed)
   {
      // Case 4a: We are mom's left and mom is granny's left
      if (this->isLeftChild() && pParent->isLeftChild())
      {
         pGranny->isRed = true;
         pParent->isRed = false;
         pParent->addRight(pGranny);
         pGranny->addLeft(pSibling);
         if (pGreatGranny)
         {
            pGranny->isLeftChild() ? pGreatGranny->addLeft(pParent): pGreatGranny->addRight(pParent);
         }
         else
         {
            pParent->pParent = nullptr;
         }

      }
      // case 4b: We are mom's right and mom is granny's right
      else if (this->isRightChild() && pParent->isRightChild())
      {
         pGranny->isRed = true;
         pParent->isRed = false;
         pParent->addLeft(pGranny);
         pGranny->addRight(pSibling);
         if (pGreatGranny)
         {
            pGranny->isLeftChild() ? pGreatGranny->addLeft(pParent): pGreatGranny->addRight(pParent);
         }
         else
         {
            pParent->pParent = nullptr;
            *root  = *pParent;
         }

      }
      // Case 4c: We are mom's right and mom is granny's left
      else if (this->isRightChild() && pParent->isLeftChild())
      {
         isRed = false;
         pGranny->isRed = true;
         BNode* pTempL = this->pLeft;
         BNode* pTempR = this->pRight;
         
         pParent->addRight(pTempL);
         pGranny->addLeft(pTempR);
         addLeft(pParent);
         addRight(pGranny);
         if (pGreatGranny)
         {
            pGranny->isLeftChild() ? pGreatGranny->addLeft(this): pGreatGranny->addRight(this);
         }
         else
         {
            this->pParent = nullptr;
            *root = *this;
         }
      }
      // case 4d: we are mom's left and mom is granny's right
      else if (this->isLeftChild() && pParent->isRightChild())
      {
         isRed = false;
         pGranny->isRed = true;
         BNode* pTempL = this->pLeft;
         BNode* pTempR = this->pRight;
         
         pGranny->addRight(pTempL);
         pParent->addLeft(pTempR);
         addLeft(pGranny);
         addRight(pParent);
         if (pGreatGranny)
         {
            pGranny->isLeftChild() ? pGreatGranny->addLeft(this): pGreatGranny->addRight(this);
         }
         else
         {
            this->pParent = nullptr;
            *root = *this;
         }
      }
   }
}

/*************************************************
 *************************************************
 *************************************************
 ****************** ITERATOR *********************
 *************************************************
 *************************************************
 *************************************************/     

/**************************************************
 * BST ITERATOR :: INCREMENT PREFIX
 * advance by one
 *************************************************/
template <typename T>
typename BST <T> :: iterator & BST <T> :: iterator :: operator ++ ()
{
    if (pNode == nullptr) return *this;  // End of traversal
    
    // Case 1: If there is a right child, go to the leftmost node in the right subtree
    if (pNode->pRight != nullptr)
    {
        pNode = pNode->pRight;
        while (pNode->pLeft != nullptr)
        {
            pNode = pNode->pLeft;
        }
        return *this;
    }

    // Case 2: If there is no right child and we are the left child of the parent, move up to the parent
    if (pNode->pRight == nullptr && pNode->pParent != nullptr && pNode->pParent->pLeft == pNode)
    {
        pNode = pNode->pParent;
        return *this;
    }

    // Case 3: If there is no right child and we are the right child of the parent, move up to the parent
    // and continue moving up until we are the left child of a parent
    if (pNode->pRight == nullptr && pNode->pParent != nullptr && pNode->pParent->pRight == pNode)
    {
        while (pNode->pParent != nullptr && pNode->pParent->pRight == pNode)
        {
            pNode = pNode->pParent;
        }
        pNode = pNode->pParent;  // Move to the parent
        return *this;
    }

    return *this;
}


/**************************************************
 * BST ITERATOR :: DECREMENT PREFIX
 * advance by one
 *************************************************/
template <typename T>
typename BST <T> :: iterator & BST <T> :: iterator :: operator -- ()
{
   if (pNode == nullptr) return *this;
   
   if (pNode->pLeft)
   {
      pNode = pNode->pLeft;
      while (pNode->pRight)
      {
         pNode = pNode->pRight;
      }
      return *this;
   }
   if (pNode->pLeft == nullptr and pNode->pParent->pRight == pNode)
   {
      pNode = pNode->pParent;
      return *this;
   }
   if (pNode->pLeft == nullptr and pNode->pParent->pLeft == pNode)
   {
      while (pNode->pParent and pNode->pParent->pLeft == pNode)
      {
         pNode = pNode->pParent;

      }
      pNode = pNode->pParent;
      return *this;
   }
   return *this;

}


} // namespace custom


