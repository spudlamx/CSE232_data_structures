/***********************************************************************
 * Header:
 *    VECTOR
 * Summary:
 *    Our custom implementation of std::vector
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        vector                 : A class that represents a Vector
 *        vector::iterator       : An iterator through Vector
 * Author
 *    Spencer Lamoreaux, Ashlee Hart
 ************************************************************************/

#pragma once

#include <cassert>  // because I am paranoid
#include <new>      // std::bad_alloc
#include <memory>   // for std::allocator

class TestVector; // forward declaration for unit tests
class TestStack;
class TestPQueue;
class TestHash;

namespace custom
{

/*****************************************
 * VECTOR
 * Just like the std :: vector <T> class
 ****************************************/
template <typename T, typename A = std::allocator<T>>
class vector
{
   friend class ::TestVector; // give unit tests access to the privates
   friend class ::TestStack;
   friend class ::TestPQueue;
   friend class ::TestHash;
public:
   
   //
   // Construct
   //
   vector(const A & a = A());
   vector(size_t numElements,                const A & a = A());
   vector(size_t numElements, const T & t,   const A & a = A());
   vector(const std::initializer_list<T>& l, const A & a = A());
   vector(const vector &  rhs);
   vector(      vector && rhs);
  ~vector();

   //
   // Assign
   //
   void swap(vector& rhs)
   {
      T* tempData = data;
      data = rhs.data;
      rhs.data = tempData;
   
      // Swap the number of elements
      size_t tempElements = numElements;
      numElements = rhs.numElements;
      rhs.numElements = tempElements;
   
      // Swap the capacities
      size_t tempCapacity = numCapacity;
      numCapacity = rhs.numCapacity;
      rhs.numCapacity = tempCapacity;
   }
   vector & operator = (const vector & rhs);
   vector & operator = (vector&& rhs);

   //
   // Iterator
   //
   class iterator;
   iterator begin()
   {
      return iterator();
   }
   iterator end()
   {
      return iterator();
   }

   //
   // Access
   //
         T& operator [] (size_t index);
   const T& operator [] (size_t index) const;
         T& front();
   const T& front() const;
         T& back();
   const T& back() const;

   //
   // Insert
   //
   void push_back(const T& t);
   void push_back(T&& t);
   void reserve(size_t newCapacity);
   void resize(size_t newElements);
   void resize(size_t newElements, const T& t);

   //
   // Remove
   //
   void clear()
   {
      for (size_t i = 0; i < numElements; ++i) {
         alloc.destroy(&data[i]); // Call destructor for each element
      }
      numElements = 0;
   }
   void pop_back()
   {
      if (numElements > 0) {
         alloc.destroy(&data[numElements - 1]);
         numElements--;
      }
      
      
   }
   void shrink_to_fit();

   //
   // Status
   //
   size_t  size()          const { return numElements;}
   size_t  capacity()      const { return numCapacity;}
   bool empty()            const { return size() == 0;}
  
private:
   
   A    alloc;                // use allocator for memory allocation
   T *  data;                 // user data, a dynamically-allocated array
   size_t  numCapacity;       // the capacity of the array
   size_t  numElements;       // the number of items currently used
};

/**************************************************
 * VECTOR ITERATOR
 * An iterator through vector.  You only need to
 * support the following:
 *   1. Constructors (default and copy)
 *   2. Not equals operator
 *   3. Increment (prefix and postfix)
 *   4. Dereference
 * This particular iterator is a bi-directional meaning
 * that ++ and -- both work.  Not all iterators are that way.
 *************************************************/
template <typename T, typename A>
class vector <T, A> ::iterator
{
   friend class ::TestVector; // give unit tests access to the privates
   friend class ::TestStack;
   friend class ::TestPQueue;
   friend class ::TestHash;
public:
   // constructors, destructors, and assignment operator
   iterator()                           {  }
   iterator(T* p)                       {  }
   iterator(const iterator& rhs)        {  }
   iterator(size_t index, vector<T>& v) {  }
   iterator& operator = (const iterator& rhs)
   {
      return *this;
   }

   // equals, not equals operator
   bool operator != (const iterator& rhs) const { return true; }
   bool operator == (const iterator& rhs) const { return true; }

   // dereference operator
   T& operator * ()
   {
      return *(new T);
   }

   // prefix increment
   iterator& operator ++ ()
   {
      return *this;
   }

   // postfix increment
   iterator operator ++ (int postfix)
   {
      return *this;
   }

   // prefix decrement
   iterator& operator -- ()
   {
      return *this;
   }

   // postfix decrement
   iterator operator -- (int postfix)
   {
      return *this;
   }

private:
   T* p;
};


/*****************************************
 * VECTOR :: NON-DEFAULT constructors
 * non-default constructor: set the number of elements,
 * construct each element, and copy the values over
 ****************************************/
template <typename T, typename A>
vector <T, A> :: vector(const A & a)
{
   data = nullptr;
   numElements = 0;
   numCapacity = 0;
}


/*****************************************
 * VECTOR :: NON-DEFAULT constructors
 * non-default constructor: set the number of elements,
 * construct each element, and copy the values over
 ****************************************/
template <typename T, typename A>
vector <T, A> :: vector(size_t num, const T & t, const A & a)
{
   alloc = a;
   data = (num == 0 ? nullptr : alloc.allocate(num));
   numElements = num;
   numCapacity = num;
   for (size_t i = 0; i < num; ++i)
   {
      new (&data[i]) T(t);  // Construct elements with the given value
   }
}

/*****************************************
 * VECTOR :: INITIALIZATION LIST constructors
 * Create a vector with an initialization list.
 ****************************************/
template <typename T, typename A>
vector <T, A> :: vector(const std::initializer_list<T> & l, const A & a)
{
   alloc = a;
   data = alloc.allocate(l.size());
   numElements = l.size();
   numCapacity = l.size();

   size_t index = 0;
   for (auto it = l.begin(); it != l.end(); ++it)
   {
      alloc.construct(data + index, *it); // Construct the item
      index++; // Move to the next index
   }

}

/*****************************************
 * VECTOR :: NON-DEFAULT constructors
 * non-default constructor: set the number of elements,
 * construct each element, and copy the values over
 ****************************************/
template <typename T, typename A>
vector <T, A> :: vector(size_t num, const A & a)
{
   alloc = a;
   data = (num == 0 ? nullptr : alloc.allocate(num));
   numElements = num;
   numCapacity = num;
   for (size_t i = 0; i < num; ++i)
   {
      new (&data[i]) T;  // Construct elements with the given value
   }

}

/*****************************************
 * VECTOR :: COPY CONSTRUCTOR
 * Allocate the space for numElements and
 * call the copy constructor on each element
 ****************************************/
template <typename T, typename A>
vector <T, A> :: vector (const vector & rhs)
{
   if (!rhs.empty()) {
      data = alloc.allocate(rhs.numElements);
      numElements = rhs.numElements;
      numCapacity = rhs.numElements;

      for (size_t i = 0; i < numElements; ++i)
      {
            alloc.construct(&data[i], rhs.data[i]); // Use allocator to construct each element
      }
   }
   else
   {
      data = nullptr;
      numElements = 0;
      numCapacity = 0;
   }

}
   
/*****************************************
 * VECTOR :: MOVE CONSTRUCTOR
 * Steal the values from the RHS and set it to zero.
 ****************************************/
template <typename T, typename A>
vector <T, A> :: vector (vector && rhs)
{
   data = rhs.data;
   rhs.data = NULL;

   numElements = rhs.numElements;
   rhs.numElements = NULL;

   numCapacity = rhs.numCapacity;
   rhs.numCapacity = NULL;
}

/*****************************************
 * VECTOR :: DESTRUCTOR
 * Call the destructor for each element from 0..numElements
 * and then free the memory
 ****************************************/
template <typename T, typename A>
vector <T, A> :: ~vector()
{
   for (size_t i = 0; i < numElements; ++i)
   {
      alloc.destroy(&data[i]);
   }
   alloc.deallocate(data, numCapacity);
}

/***************************************
 * VECTOR :: RESIZE
 * This method will adjust the size to newElements.
 * This will either grow or shrink newElements.
 *     INPUT  : newCapacity the size of the new buffer
 *     OUTPUT :
 **************************************/
template <typename T, typename A>
void vector <T, A> :: resize(size_t newElements)
{
   if (newElements < numElements)
   {
      for (size_t i = newElements; i < numElements; i++)
      {
         alloc.destroy(&data[i]);
      }
   }
   else if (newElements > numElements)
   {
      // Increase size, reserve space if necessary
      if (newElements > numCapacity)
      {
         reserve(newElements);
      }
      // Construct new elements
      for (size_t i = numElements; i < newElements; i++)
      {
         alloc.construct(&data[i]); // Default construct
      }
   }
    // Update the size
    numElements = newElements;
}

template <typename T, typename A>
void vector <T, A> :: resize(size_t newElements, const T & t)
{
   if (newElements < numElements)
   {
      for (size_t i = newElements; i < numElements; i++)
      {
         alloc.destroy(&data[i]);
      }
   }
   else if (newElements > numElements)
   {
      // Increase size, reserve space if necessary
      if (newElements > numCapacity)
      {
         reserve(newElements);
      }
      // Construct new elements
      for (size_t i = numElements; i < newElements; i++)
      {
         alloc.construct(&data[i], t); // Default construct
      }
   }
    // Update the size
    numElements = newElements;
}

/***************************************
 * VECTOR :: RESERVE
 * This method will grow the current buffer
 * to newCapacity.  It will also copy all
 * the data from the old buffer into the new
 *     INPUT  : newCapacity the size of the new buffer
 *     OUTPUT :
 **************************************/
template <typename T, typename A>
void vector<T, A>::reserve(size_t newCapacity)
{
   if (newCapacity <= numCapacity) {
      return; // No need to reserve if the new capacity is less than or equal to current capacity
   }

   // Allocate new memory
   T* dataNew = alloc.allocate(newCapacity);

   for (auto i = 0; i < numElements; i++)
   {
      new ((void*)(dataNew + i)) T(std::move(data[i]));
   }
   
   for (auto i = 0; i < numElements; i++) {
      alloc.destroy(&data[i]);
   }

   alloc.deallocate(data, numCapacity);
   data = dataNew;
   numCapacity = newCapacity;
}


/***************************************
 * VECTOR :: SHRINK TO FIT
 * Get rid of any extra capacity
 *     INPUT  :
 *     OUTPUT :
 **************************************/
template <typename T, typename A>
void vector <T, A> :: shrink_to_fit()
{
   if (numElements == 0) {
      data = nullptr;
      numCapacity = 0;
      return;
   }
   if (numElements == numCapacity)
   {
      return;
   }

   // Allocate new memory
   T* dataNew = alloc.allocate(numElements);

   for (size_t i = 0; i < numElements; i++)
   {
      alloc.construct(&dataNew[i], data[i]);
   }
   
   for (auto i = 0; i < numElements; i++)
   {
      alloc.destroy(&data[i]);
   }

   alloc.deallocate(data, numCapacity);
   data = dataNew;
   numCapacity = numElements;
}



/*****************************************
 * VECTOR :: SUBSCRIPT
 * Read-Write access
 ****************************************/
template <typename T, typename A>
T & vector <T, A> :: operator [] (size_t index)
{
   return data[index];
    
}

/******************************************
 * VECTOR :: SUBSCRIPT
 * Read-Write access
 *****************************************/
template <typename T, typename A>
const T & vector <T, A> :: operator [] (size_t index) const
{
   return data[index];
}

/*****************************************
 * VECTOR :: FRONT
 * Read-Write access
 ****************************************/
template <typename T, typename A>
T & vector <T, A> :: front ()
{
   return data[0];
}

/******************************************
 * VECTOR :: FRONT
 * Read-Write access
 *****************************************/
template <typename T, typename A>
const T & vector <T, A> :: front () const
{
   return data[0];
}

/*****************************************
 * VECTOR :: FRONT
 * Read-Write access
 ****************************************/
template <typename T, typename A>
T & vector <T, A> :: back()
{
   return data[numElements - 1];
}

/******************************************
 * VECTOR :: FRONT
 * Read-Write access
 *****************************************/
template <typename T, typename A>
const T & vector <T, A> :: back() const
{
   return data[numElements - 1];
}

/***************************************
 * VECTOR :: PUSH BACK
 * This method will add the element 't' to the
 * end of the current buffer.  It will also grow
 * the buffer as needed to accomodate the new element
 *     INPUT  : 't' the new element to be added
 *     OUTPUT : *this
 **************************************/
template <typename T, typename A>
void vector <T, A> :: push_back (const T & t)
{
   if (numElements >= numCapacity)
      {
         size_t newCapacity = numCapacity == 0 ? 1 : numCapacity * 2;
         reserve(newCapacity); // Allocate new memory
      }
   new ((void *)(&data[numElements++])) T(t);
}

template <typename T, typename A>
void vector <T, A> ::push_back(T && t)
{
   if (numElements >= numCapacity)
      {
         size_t newCapacity = numCapacity == 0 ? 1 : numCapacity * 2;
         reserve(newCapacity); // Allocate new memory
      }
   new ((void *)(&data[numElements++])) T(std::move(t));
}

/***************************************
 * VECTOR :: ASSIGNMENT
 * This operator will copy the contents of the
 * rhs onto *this, growing the buffer as needed
 *     INPUT  : rhs the vector to copy from
 *     OUTPUT : *this
 **************************************/
template <typename T, typename A>
vector<T, A>& vector<T, A>::operator=(const vector& rhs)
{
   // Handle the three main cases
   if (rhs.size() == size())
   {
      // Case 1: Same size
      for (size_t i = 0; i < size(); ++i)
      {
         data[i] = rhs.data[i];
      }
   }
   else if (rhs.size() > size())
   {
      // Case 2: RHS larger
      if (rhs.size() <= capacity())
      {
         // Enough capacity, just copy and construct new elements
         for (size_t i = 0; i < size(); ++i)
         {
            data[i] = rhs.data[i];
         }
         for (size_t i = size(); i < rhs.size(); ++i)
         {
            alloc.construct(&data[i], rhs.data[i]); // Construct new elements
         }
      }
      else
      {
         // Not enough capacity, allocate new memory
         T* dataNew = alloc.allocate(rhs.size());
         for (size_t i = 0; i < rhs.size(); ++i)
         {
            alloc.construct(&dataNew[i], rhs.data[i]); // Construct elements in new memory
         }
         clear(); // Clear old elements
         alloc.deallocate(data, numCapacity); // Deallocate old memory
         data = dataNew; // Point to new memory
         numCapacity = rhs.size(); // Update capacity
      }
   }
   else
   {
      // Case 3: LHS larger
      // Copy elements
      for (size_t i = 0; i < rhs.size(); ++i)
      {
         data[i] = rhs.data[i];
      }
      // Destroy the extra elements
      for (size_t i = rhs.size(); i < size(); ++i)
      {
         alloc.destroy(&data[i]);
      }
   }

   // Update the number of elements
   numElements = rhs.size();
   return *this;
}

template <typename T, typename A>
vector<T, A>& vector<T, A>::operator=(vector&& rhs)
{
   if (this != &rhs) { // Check for self-assignment
      // Release current resources
      clear();
      alloc.deallocate(data, numCapacity);
      
      // Steal data from rhs
      data = rhs.data;
      numElements = rhs.numElements;
      numCapacity = rhs.numCapacity;

      // Reset rhs
      rhs.data = nullptr;
      rhs.numElements = 0;
      rhs.numCapacity = 0;
   }
   return *this;
}



} // namespace custom






///***********************************************************************
// * Header:
// *    VECTOR
// * Summary:
// *    Our custom implementation of std::vector
// *      __      __     _______        __
// *     /  |    /  |   |  _____|   _  / /
// *     `| |    `| |   | |____    (_)/ /
// *      | |     | |   '_.____''.   / / _
// *     _| |_   _| |_  | \____) |  / / (_)
// *    |_____| |_____|  \______.' /_/
// *
// *    This will contain the class definition of:
// *        vector                 : A class that represents a Vector
// *        vector::iterator       : An iterator through Vector
// * Author
// *    <your names here>
// ************************************************************************/
//
//#pragma once
//
//#include <cassert>  // because I am paranoid
//#include <new>      // std::bad_alloc
//#include <memory>   // for std::allocator
//
//class TestVector; // forward declaration for unit tests
//class TestStack;
//class TestPQueue;
//class TestHash;
//
//namespace custom
//{
//
///*****************************************
// * VECTOR
// * Just like the std :: vector <T> class
// ****************************************/
//template <typename T, typename A = std::allocator<T>>
//class vector
//{
//   friend class ::TestVector; // give unit tests access to the privates
//   friend class ::TestStack;
//   friend class ::TestPQueue;
//   friend class ::TestHash;
//public:
//   
//   //
//   // Construct
//   //
//   vector(const A & a = A());
//   vector(size_t numElements,                const A & a = A());
//   vector(size_t numElements, const T & t,   const A & a = A());
//   vector(const std::initializer_list<T>& l, const A & a = A());
//   vector(const vector &  rhs);
//   vector(      vector && rhs);
//  ~vector();
//
//   //
//   // Assign
//   //
//   void swap(vector& rhs)
//   {
//      T* tempData = data;
//      data = rhs.data;
//      rhs.data = tempData;
//
//      // Swap the number of elements
//      size_t tempElements = numElements;
//      numElements = rhs.numElements;
//      rhs.numElements = tempElements;
//
//      // Swap the capacities
//      size_t tempCapacity = numCapacity;
//      numCapacity = rhs.numCapacity;
//      rhs.numCapacity = tempCapacity;
//   }
//   vector & operator = (const vector & rhs);
//   vector & operator = (vector&& rhs);
//
//   //
//   // Iterator
//   //
//   class iterator;
//   iterator begin() 
//   { 
//      return iterator(); 
//   }
//   iterator end() 
//   { 
//      return iterator(); 
//   }
//
//   //
//   // Access
//   //
//         T& operator [] (size_t index);
//   const T& operator [] (size_t index) const;
//         T& front();
//   const T& front() const;
//         T& back();
//   const T& back() const;
//
//   //
//   // Insert
//   //
//   void push_back(const T& t);
//   void push_back(T&& t);
//   void reserve(size_t newCapacity);
//   void resize(size_t newElements);
//   void resize(size_t newElements, const T& t);
//
//   //
//   // Remove
//   //
//   void clear()
//   {
//   }
//   void pop_back()
//   {
//   }
//   void shrink_to_fit();
//
//   //
//   // Status
//   //
//   size_t  size()          const { return numElements;}
//   size_t  capacity()      const { return numCapacity;}
//   bool empty()            const { return size() == 0;}
//  
//private:
//   
//   A    alloc;                // use allocator for memory allocation
//   T *  data;                 // user data, a dynamically-allocated array
//   size_t  numCapacity;       // the capacity of the array
//   size_t  numElements;       // the number of items currently used
//};
//
///**************************************************
// * VECTOR ITERATOR
// * An iterator through vector.  You only need to
// * support the following:
// *   1. Constructors (default and copy)
// *   2. Not equals operator
// *   3. Increment (prefix and postfix)
// *   4. Dereference
// * This particular iterator is a bi-directional meaning
// * that ++ and -- both work.  Not all iterators are that way.
// *************************************************/
//template <typename T, typename A>
//class vector <T, A> ::iterator
//{
//   friend class ::TestVector; // give unit tests access to the privates
//   friend class ::TestStack;
//   friend class ::TestPQueue;
//   friend class ::TestHash;
//public:
//   // constructors, destructors, and assignment operator
//   iterator()                           {  }
//   iterator(T* p)                       {  }
//   iterator(const iterator& rhs)        {  }
//   iterator(size_t index, vector<T>& v) {  }
//   iterator& operator = (const iterator& rhs)
//   {
//      return *this;
//   }
//
//   // equals, not equals operator
//   bool operator != (const iterator& rhs) const { return true; }
//   bool operator == (const iterator& rhs) const { return true; }
//
//   // dereference operator
//   T& operator * ()
//   {
//      return *(new T);
//   }
//
//   // prefix increment
//   iterator& operator ++ ()
//   {
//      return *this;
//   }
//
//   // postfix increment
//   iterator operator ++ (int postfix)
//   {
//      return *this;
//   }
//
//   // prefix decrement
//   iterator& operator -- ()
//   {
//      return *this;
//   }
//
//   // postfix decrement
//   iterator operator -- (int postfix)
//   {
//      return *this;
//   }
//
//private:
//   T* p;
//};
//
//
///*****************************************
// * VECTOR :: NON-DEFAULT constructors
// * non-default constructor: set the number of elements,
// * construct each element, and copy the values over
// ****************************************/
//template <typename T, typename A>
//vector <T, A> :: vector(const A & a)
//{
////   data = nullptr;
////   numElements = 0;
////   numCapacity = 0;
//}
//
//
///*****************************************
// * VECTOR :: NON-DEFAULT constructors
// * non-default constructor: set the number of elements,
// * construct each element, and copy the values over
// ****************************************/
//template <typename T, typename A>
//vector <T, A> :: vector(size_t num, const T & t, const A & a) 
//{
//   alloc = a; // Assign the allocator
//   data = alloc.allocate(num);
//   numElements = num; // Set number of elements
//   numCapacity = num; // Set capacity to the number of elements
//
//   // Construct each element with the value t
//   for (size_t i = 0; i < numElements; ++i) {
//      alloc.construct(data + i, t); // Construct T at data[i]
//      }
//}
//
///*****************************************
// * VECTOR :: INITIALIZATION LIST constructors
// * Create a vector with an initialization list.
// ****************************************/
//template <typename T, typename A>
//vector <T, A> :: vector(const std::initializer_list<T> & l, const A & a) 
//{
//   alloc = a;
//   data = alloc.allocate(l.size());
//   numElements = l.size();
//   numCapacity = l.size();
//   
//   size_t index = 0;
//   for (auto it = l.begin(); it != l.end(); ++it) {
//           alloc.construct(data + index, *it); // Construct the item
//           index++; // Move to the next index
//       }
//
//}
//
///*****************************************
// * VECTOR :: NON-DEFAULT constructors
// * non-default constructor: set the number of elements,
// * construct each element, and copy the values over
// ****************************************/
//template <typename T, typename A>
//vector <T, A> :: vector(size_t num, const A & a) 
//{
//   alloc = a; // Assign the allocator
//   data = alloc.allocate(num);
//   numElements = num; // Set number of elements
//   numCapacity = num; // Set capacity to the number of elements
//   
//   for (size_t i = 0; i < numElements; ++i) {
//      alloc.construct(data + i); // Use the allocator to construct each element
//   }
//}
//
///*****************************************
// * VECTOR :: COPY CONSTRUCTOR
// * Allocate the space for numElements and
// * call the copy constructor on each element
// ****************************************/
//template <typename T, typename A>
//vector <T, A> :: vector (const vector & rhs) 
//{
//   if (!rhs.empty()) {
//      data = alloc.allocate(rhs.numElements);
//      numElements = rhs.numElements;
//      numCapacity = rhs.numElements;
//      
//      for (size_t i = 0; i < numElements; ++i)
//      {
//            alloc.construct(data + i, rhs.data[i]); // Use allocator to construct each element
//      }
//   }
//   else
//   {
//      data = nullptr;
//      numElements = 0;
//      numCapacity = 0;
//   }
//   
//}
//   
///*****************************************
// * VECTOR :: MOVE CONSTRUCTOR
// * Steal the values from the RHS and set it to zero.
// ****************************************/
//template <typename T, typename A>
//vector <T, A> :: vector (vector && rhs) 
//{
//   data = rhs.data;
//   rhs.data = NULL;
//   
//   numElements = rhs.numElements;
//   rhs.numElements = NULL;
//   
//   numCapacity = rhs.numCapacity;
//   rhs.numCapacity = NULL;
//}
//
///*****************************************
// * VECTOR :: DESTRUCTOR
// * Call the destructor for each element from 0..numElements
// * and then free the memory
// ****************************************/
//template <typename T, typename A>
//vector<T, A>::~vector()
//{
//   for (size_t i = 0; i < numElements; ++i)
//   {
//      alloc.destroy(&data[i]);
//   }
//   alloc.deallocate(data, numCapacity);
//}
//
//
///***************************************
// * VECTOR :: RESIZE
// * This method will adjust the size to newElements.
// * This will either grow or shrink newElements.
// *     INPUT  : newCapacity the size of the new buffer
// *     OUTPUT :
// **************************************/
//template <typename T, typename A>
//void vector<T, A>::resize(size_t newElements) {
//   if (newElements < numElements)
//   {
//      for (size_t i = newElements; i < numElements; i++)
//      {
//         alloc.destroy(&data[i]);
//      }
//   }
//   else if (newElements > numElements)
//   {
//      // Increase size, reserve space if necessary
//      if (newElements > numCapacity)
//      {
//         reserve(newElements);
//      }
//      // Construct new elements
//      for (size_t i = numElements; i < newElements; ++i)
//      {
//         alloc.construct(&data[i]); // Default construct
//      }
//   }
//    // Update the size
//    numElements = newElements;
//}
//
//template <typename T, typename A>
//void vector <T, A> :: resize(size_t newElements, const T & t)
//{
//   if (newElements < numElements)
//   {
//      for (size_t i = newElements; i < numElements; i++)
//      {
//         alloc.destroy(&data[i]);
//      }
//   }
//   else if (newElements > numElements)
//   {
//      // Increase size, reserve space if necessary
//      if (newElements > numCapacity)
//      {
//         reserve(newElements);
//      }
//      // Construct new elements
//      for (size_t i = numElements; i < newElements; i++)
//      {
//         alloc.construct(&data[i]); // Default construct
//      }
//   }
//    // Update the size
//    numElements = newElements;
//}
//
///***************************************
// * VECTOR :: RESERVE
// * This method will grow the current buffer
// * to newCapacity.  It will also copy all
// * the data from the old buffer into the new
// *     INPUT  : newCapacity the size of the new buffer
// *     OUTPUT :
// **************************************/
//template <typename T, typename A>
//void vector <T, A> :: reserve(size_t newCapacity)
//{
//   if (newCapacity <= numCapacity)
//   {
//      return;
//   }
//   
//   T* dataNew = alloc.allocate(newCapacity);
//   for (size_t i = 0; i < numElements; i++)
//      
//   {
//      dataNew[i] = data[i];
//   }
//   
//   alloc.deallocate(data, numCapacity);
//   data = dataNew;
//   numCapacity = newCapacity;
//   
//}
//
///***************************************
// * VECTOR :: SHRINK TO FIT
// * Get rid of any extra capacity
// *     INPUT  :
// *     OUTPUT :
// **************************************/
//template <typename T, typename A>
//void vector <T, A> :: shrink_to_fit()
//{
////   if (numElements == numCapacity)
////   {
////      return;
////   }
////   
////   T* dataNew = alloc.allocate(numElements);
////   
////   for (size_t i = 0; i < numElements; ++i)
////   {
////      dataNew[i] = data[i];
////   }
////      
////   alloc.deallocate(data, numCapacity);
////
////   data = dataNew;
////   numCapacity = numElements;
//   
//    
//}
//
//
//
///*****************************************
// * VECTOR :: SUBSCRIPT
// * Read-Write access
// ****************************************/
//template <typename T, typename A>
//T & vector <T, A> :: operator [] (size_t index)
//{
//   return data[index];
//    
//}
//
///******************************************
// * VECTOR :: SUBSCRIPT
// * Read-Write access
// *****************************************/
//template <typename T, typename A>
//const T & vector <T, A> :: operator [] (size_t index) const
//{
//   return data[index];
//}
//
///*****************************************
// * VECTOR :: FRONT
// * Read-Write access
// ****************************************/
//template <typename T, typename A>
//T & vector <T, A> :: front ()
//{
//   return data[0];
//}
//
///******************************************
// * VECTOR :: FRONT
// * Read-Write access
// *****************************************/
//template <typename T, typename A>
//const T & vector <T, A> :: front () const
//{
//   return data[0];
//}
//
///*****************************************
// * VECTOR :: FRONT
// * Read-Write access
// ****************************************/
//template <typename T, typename A>
//T & vector <T, A> :: back()
//{
//   return data[numElements - 1];
//}
//
///******************************************
// * VECTOR :: FRONT
// * Read-Write access
// *****************************************/
//template <typename T, typename A>
//const T & vector <T, A> :: back() const
//{
//   return data[numElements - 1];
//}
//
///***************************************
// * VECTOR :: PUSH BACK
// * This method will add the element 't' to the
// * end of the current buffer.  It will also grow
// * the buffer as needed to accomodate the new element
// *     INPUT  : 't' the new element to be added
// *     OUTPUT : *this
// **************************************/
//template <typename T, typename A>
//void vector <T, A> :: push_back (const T & t)
//{
////   new ((void *)(&data[numElements++])) T(t); 
//}
//
//template <typename T, typename A>
//void vector <T, A> ::push_back(T && t)
//{
//    
//
//}
//
///***************************************
// * VECTOR :: ASSIGNMENT
// * This operator will copy the contents of the
// * rhs onto *this, growing the buffer as needed
// *     INPUT  : rhs the vector to copy from
// *     OUTPUT : *this
// **************************************/
//template <typename T, typename A>
//vector <T, A> & vector <T, A> :: operator = (const vector & rhs)
//{
//   return *this;
//}
//template <typename T, typename A>
//vector <T, A>& vector <T, A> :: operator = (vector&& rhs)
//{
//
//   return *this;
//}
//
//
//
//
//} // namespace custom
//
