

In the Library ContainerElements you will find the implementation of some very popular datastructures like a dynamically allocated array, stack, queue, hash table and heap.

1. Object Pool
   	This implementation of an object pool is based on a very simple sequentiell 
	allocation and deallocation strategy. It is meant as replacement to the 
	naive solution i've implemented so far. Where even the reserved memory 
	is fully initialized and size values are used for the actual size and the 
	true capacity. 

	Thia memory pool implementation holds four pointers to the 
	container objects.
    The first pointer points to the beginning of the allocated memory.
	The second one points to the beginning of the pool and is the data pointer.
    The third on points to the end of the pool elements.     
	The fourth one points to the end of the capacity of the container objects.    

	1. Allocation strategy:
	The allocation strategy is given by the second template parameter 
	TAllocType. This parameter has to be a class corresponding to the 
	std::allocator.
	The decision to use a std conforming allocator was made to keep the 
	implementation as testable and because of that STL comform as possible. 

	2. Growing strategy:
	The third template parameter gives the object pool growing strategy. 
	The pool of objects can only grow sequentielly at the end of the pool. 
	This is probably the most useful functionality of the object pool, 
	because the user can decide if and how the pool is growing. 
	The default object pool growing strategy is given by the struct 
	TPoolGStrat. 
	
	HINT: 
	What i came up with is basically a vector without overhead like iterators, 
	error safty (exception handling), thread safty and so on ... ! 
	Therefore iterators are defined as pointers.
	Furthermore the used method specific pointer types could be replaced by 
	iterator types. 
	BUT this class is meant to be a memory pool for array objects and 
	not a vector replacement with the capability to modify the growing strategy. 
    Especially the growing strategy enables the implementation of algorithms which
    lets the elements grow to a point. This might be interesting for an external
    memory algorithm where memory management is crussial.
    Another example is if you want to always grow to the next power of 2 based on
    the last pool size. This is especially useful if you want to be memory alligned.
	(Anyway this class can be used as vector replacement, because it provides 
	the same functionality plus the complete control of the capacity growing.) 
    The fact that there are four pointers instead of three allows the allocation
    of elements in the beginning and at the end of the pool (see queue and stack).

2. StringPool
    The string pool is using an object pool of characters and implements a simple stl string replacement.

3. TFIFOPool
    The fifo (first in first out) pool datastructure is basically a queue where the elements are all stored in a simple pool.
    Especially the pop operation shows the true power of using 4 instead of 3 pointers in a pool implementation.
    Because the pop operation advances the second pointer one element in the direction of the third and fourth pointer.

4. TLIFOPool
    The lifo (last in first out) pool datastructure is basically a stack where the elements are all stored in a simple pool.

5. HashPool
    The hash pool is following a very simple straight forward hash table strategy and is very similar to the dictionary in c# or the dictionary in java.

6. BinaryHeap
    The binary heap is also using the object pool to implement an ordinary binary heap datastructure.
    A binary heap stores the value in form of a binary tree. 
    Where any parent has at most two children. 
    The order of the values is given by the heap order property. 
	This implemenation uses an object pool to store its values. 
	The heap order is given by the template paramenter TCompType. 
	The heap order for minimum heaps insists that any parent value is smaller 
	than its child values (parent < children).
	The heap order for maximum heaps insists that any parent value is greater 
	than its children (parent > children).
	The following 3 tasks are necessary to make sure that a heap will always 
	stay in the given heap order. 

7. BinaryIndexHeap
    The index heap is a binary heap, that works with indices rather than values. 
