/***********************************************************************
 * Header:
 *    NODE
 * Summary:
 *    One node in a linked list (and the functions to support them).
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        Node         : A class representing a Node
 *    Additionally, it will contain a few functions working on Node
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once

#include <cassert>     // for ASSERT
#include <iostream>    // for NULL

/*************************************************
 * NODE
 * the node class.  Since we do not validate any
 * of the setters, there is no point in making them
 * private.  This is the case because only the
 * List class can make validation decisions
 *************************************************/
template <class T>
class Node
{
public:
   //
   // Construct
   //
   //
   // Construct
   //

   Node() : data(T()), pNext(nullptr), pPrev(nullptr) {}

   Node(const T& data) : data(data), pNext(nullptr), pPrev(nullptr){}

   Node(T&& data) : data(std::move(data)), pNext(nullptr), pPrev(nullptr) {}
   //
   // Member variables
   //

   T data;                 // user data
   Node <T> * pNext;       // pointer to next node
   Node <T> * pPrev;       // pointer to previous node
};

/***********************************************
 * COPY
 * Copy the list from the pSource and return
 * the new list
 *   INPUT  : the list to be copied
 *   OUTPUT : return the new list
 *   COST   : O(n)
 **********************************************/
template <class T>
inline Node<T>* copy(const Node<T>* pSource)
{
   if (pSource == nullptr) {
      return nullptr; // Return nullptr if the source is null
   }

   // Create the destination node using the data from the source node
   Node<T>* pDestination = new Node<T>(pSource->data);
   Node<T>* pDesCurrent = pDestination; // Keep track of the current node in the destination

   // Use a for loop to iterate through the source list
   for (auto pSrcCurrent = pSource->pNext; pSrcCurrent; pSrcCurrent = pSrcCurrent->pNext)
   {
      // Create a new node for the current source node
      Node<T>* newNode = new Node<T>(pSrcCurrent->data);

      // Link the new node to the destination list
      pDesCurrent->pNext = newNode; // Link the previous node to the new node
      newNode->pPrev = pDesCurrent;  // Link the new node back to the previous node
      
      // Move the current destination pointer forward
      pDesCurrent = newNode;
   }

   pDesCurrent->pNext = nullptr; // Terminate the new list

   return pDestination; // Return the head of the new list
}



/***********************************************
 * Assign
 * Copy the values from pSource into pDestination
 * reusing the nodes already created in pDestination if possible.
 *   INPUT  : the list to be copied
 *   OUTPUT : return the new list
 *   COST   : O(n)
 **********************************************/
template <class T>
inline void assign(Node <T> * & pDestination, const Node <T> * pSource)
{
   // If the source is null, clear the destination list
   if (pSource == nullptr) {
      while (pDestination != nullptr)
      {
         Node<T>* temp = pDestination;
         pDestination = pDestination->pNext;
         delete temp;  // Free the memory
      }
      return;  // Destination is now empty
   }

   // Use pointers for traversal
   const Node<T>* pSrc = pSource;  // Current source node
   Node<T>* pDes = pDestination;    // Current destination node
   Node<T>* pDesPrevious = nullptr; // To track the last node in destination

   // Step 1: Update values of existing nodes, if they exist
   while (pSrc != nullptr && pDes != nullptr)
   {
      pDes->data = pSrc->data; // Copy data
      pDesPrevious = pDes;      // Update the last node
      pDes = pDes->pNext;       // Move to the next destination node
      pSrc = pSrc->pNext;       // Move to the next source node
   }

   // Step 2: If source is longer, create new nodes for remaining elements
   while (pSrc != nullptr)
   {
      Node<T>* newNode = new Node<T>(pSrc->data); // Create a new node
      if (pDesPrevious != nullptr)
      {
         pDesPrevious->pNext = newNode;   // Link new node
         newNode->pPrev = pDesPrevious;    // Link back
      }
      else
      {
         pDestination = newNode;           // Update head if destination was empty
      }
      pDesPrevious = newNode;               // Update last node
      pSrc = pSrc->pNext;                   // Move to the next source node
   }

   // Step 3: If the destination is longer, delete extra nodes
   while (pDes != nullptr)
   {
      Node<T>* temp = pDes;
      pDes = pDes->pNext; // Move to the next node
      delete temp;        // Free the memory
   }

   // Final linking for new last node
   if (pDesPrevious != nullptr)
   {
      pDesPrevious->pNext = nullptr; // Ensure the last node's next is nullptr
   }
}


/***********************************************
 * SWAP
 * Swap the list from LHS to RHS
 *   COST   : O(1)
 **********************************************/
template <class T>
inline void swap(Node <T>* &pLHS, Node <T>* &pRHS)
{
   std::swap(pLHS, pRHS);
}

/***********************************************
 * REMOVE
 * Remove the node pSource in the linked list
 *   INPUT  : the node to be removed
 *   OUTPUT : the pointer to the parent node
 *   COST   : O(1)
 **********************************************/
template <class T>
inline Node <T> * remove(const Node <T> * pRemove) 
{
   Node<T>* pReturn = nullptr;
   if (pRemove != nullptr)
   {
      if(pRemove->pNext != nullptr)
      {
         pRemove->pNext->pPrev = pRemove->pPrev;
         pReturn = pRemove->pNext;
      }
      if(pRemove->pPrev != nullptr)
      {
         pRemove->pPrev->pNext = pRemove->pNext;
         pReturn = pRemove->pPrev;
      }
      
      delete pRemove;
   }
   return pReturn;
}


/**********************************************
 * INSERT 
 * Insert a new node the the value in "t" into a linked
 * list immediately before the current position.
 *   INPUT   : t - the value to be used for the new node
 *             pCurrent - a pointer to the node before which
 *                we will be inserting the new node
 *             after - whether we will be inserting after
 *   OUTPUT  : return the newly inserted item
 *   COST    : O(1)
 **********************************************/
template <class T>
inline Node <T> * insert(Node <T> * pCurrent,
                  const T & t,
                  bool after = false)
{
   // Create a new node with the provided value
   Node<T>* pNew = new Node<T>(t);
       
   if (pCurrent == nullptr)
   {
      return pNew;
   }

   if (after)
   {
      // Insert after the current node
      pNew->pPrev = pCurrent;
      pNew->pNext = pCurrent->pNext;
           
      if (pCurrent->pNext != nullptr)
      {
         pCurrent->pNext->pPrev = pNew;
      }
           
      pCurrent->pNext = pNew;
   }
   else
   {
      // Insert before the current node
      pNew->pNext = pCurrent;
      pNew->pPrev = pCurrent->pPrev;
           
      if (pCurrent->pPrev != nullptr)
      {
         pCurrent->pPrev->pNext = pNew;
      }
      
      pCurrent->pPrev = pNew;
   }
   return pNew;
}

/******************************************************
 * SIZE
 * Find the size an unsorted linked list.  
 *  INPUT   : a pointer to the head of the linked list
 *            the value to be found
 *  OUTPUT  : number of nodes
 *  COST    : O(n)
 ********************************************************/
template <class T>
inline size_t size(const Node <T> * pHead)
{
   size_t size = 0;
   for (auto p = pHead; p; p = p->pNext)
      size++;
   return size;
}

/***********************************************
 * DISPLAY
 * Display all the items in the linked list from here on back
 *    INPUT  : the output stream
 *             pointer to the linked list
 *    OUTPUT : the data from the linked list on the screen
 *    COST   : O(n)
 **********************************************/
template <class T>
inline std::ostream & operator << (std::ostream & out, const Node <T> * pHead)

{
   const Node<T>* pCurrent = pHead;
   out << "[";
   while (pCurrent != nullptr)
   {
      out << pCurrent->data;
      pCurrent = pCurrent->pNext;
      if (pCurrent != nullptr) // Prevents trailing separator
      {
         out << ", "; // Use your preferred separator
      }
   }
   out << "]";
   return out;
}

/*****************************************************
 * FREE DATA
 * Free all the data currently in the linked list
 *   INPUT   : pointer to the head of the linked list
 *   OUTPUT  : pHead set to NULL
 *   COST    : O(n)
 ****************************************************/
template <class T>
inline void clear(Node <T> * & pHead)
{
   Node<T>* pDelete;
   while (pHead != nullptr) {
      pDelete = pHead;
      pHead = pHead->pNext;
      delete pDelete;
   }
}


