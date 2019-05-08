# Vector-class
Self made Vector class. It is a container, similar, but more primitive, to `std::vector`. This class stores an array of variables in one single line in memory, without any spaces inbetween values.

# How to use it
Download the header, place it in your C++ program folder, include it with `#include "Vector.h"`. Syntax is simple: 

```cpp
Vector<data type> variable_name;
variable_name.function();
```
  
Example:
```cpp 
#include "Vector.h"

int main()
{
  // creates an array with size and capacity of 5 elements with values set to 0.
  Vector<int> a { 5 }; // { 0, 0, 0, 0, 0 }
  a.push_back(3);      // { 0, 0, 0, 0, 0, 3 }
  a[2] = 7;            // { 0, 0, 7, 0, 0, 3 }
}
```

# List of functions available
  ### Iterators
    1. begin
    2. end
  ### Capacity
    1. size
    2. capacity
    3. empty
    4. reserve
    5. shrink_to_fit
  ### Element access
    1. operator[]
    2. at
    3. front
    4. back
  ### modifiers
    1. push_back
    2. pop_back
    3. insert
    4. erase
    5. swap
    6. clear
  ### Nested class
    1. ::iterator
