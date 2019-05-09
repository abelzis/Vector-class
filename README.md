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
  // add value 3 to the end of the array
  a.push_back(3);      // { 0, 0, 0, 0, 0, 3 }
  // change 3rd value to 7
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
    
    
# Comparisons

So is my class even comparable to `std::vector`? Let's find out!

### Test 1
A simple test is being made: `push_back()` same amount of same values. Compare speed and size in memory. **Note**: `Release mode`, `/O2` optimization flag

Code used:
```cpp
#include <iostream>
#include <vector>
#include "Vector.h"
#include <fstream>
#include <chrono>

// Timer class for easier time calculations
class Timer {
private:
	using hrClock = std::chrono::high_resolution_clock;
	using durationDouble = std::chrono::duration<double>;
	using tmPt = std::chrono::time_point<hrClock>;

	//variables
	tmPt start_;
	durationDouble duration_{ 0 }, null_duration_{ 0 };

	inline void addDuration_() { duration_ += hrClock::now() - start_; }
public:
	//default constructor
	Timer() : start_{ hrClock::now() } {}

	//functions
	inline void reset() {
		start_ = hrClock::now();
		duration_ = null_duration_;
	}
	inline double elapsed() {
		addDuration_();
		return duration_.count();
	}
	inline void pause() {
		addDuration_();
	}
	inline void resume() {
		start_ = hrClock::now();
	}
};


int main()
{
	unsigned int sz = 100000000;  // 100000, 1000000, 10000000, 100000000

	std::ofstream results("results.txt");
	
	for (int j = 0; j < 100; j++)
	{
		// start v1 fill time
		Timer t1;
		std::vector<int> v1;
		for (int i = 1; i <= sz; ++i)
			v1.push_back(i);
		t1.pause();
		// end v1 fill time
		std::cout << "std::vector<int> fill " << sz << ": " << t1.elapsed() << " s\n";
		results << t1.elapsed() << "\n";
	}
	results << "\n\n\n\n\n\n\n";
	
	for (int j = 0; j < 100; j++)
	{
		// start v2 fill time
		Timer t2;
		Vector<int> v2;
		for (int i = 1; i <= sz; ++i)
			v2.push_back(i);
		t2.pause();
		// end v2 fill time
		std::cout << "Vector<int>   \t fill " << sz << ": " << t2.elapsed() << " s\n";
		results << t2.elapsed() << "\n";
	}
}
```

**Results**:

![speed](https://i.gyazo.com/5c9cb4728cd2cd3c20ec496584a60e4f.png)

![memory](https://i.gyazo.com/1bd40ef0c0b7e25eaa473e13366ac378.png)

![comparison](https://i.gyazo.com/ac5005f81096a30c2d86aae9bf516c14.png)

**Outcome**: speed and memory use is pretty much identical.

### Test 2
Another key thing about vectors is their reallocation management. Let's test to see how many reallocations happen in 10,000,000 push_back calls.

Code:
```cpp
#include <iostream>
#include <vector>
#include "Vector.h"

int main()
{
	//alter between `std::vector` and `Vector`
	std::vector<int> big;
	//Vector<int> big;
	int prev_capacity = 0, count = 0;

	int* test1 = nullptr, *test2 = nullptr, *test3 = nullptr;	//check for memory addresses to know if reallocation happened

	for (unsigned i = 0; i < 10000000; i++)
	//while (1)
	{
		big.push_back(i);

		if (big.capacity() > prev_capacity)	//check capacity changes
		{
			//test 1
			if (big.size() > 8)	//out_of_range safety	
			{
				if (test1 != nullptr)
				{
					if (test1 != &big[6])
					{
						std::cout << "REALOCATED TEST1!";
						std::cout << " | CAPACITY: " << big.capacity() << "\n";
					}
				}
				test1 = &big[6];
			}
			
			//test 2
			if (big.size() > 1566)
			{
				if (test2 != nullptr)
				{
					if (test2 != &big[1566])
					{
						std::cout << "REALOCATED TEST2!";
						std::cout << " | CAPACITY: " << big.capacity() << "\n";
					}
				}
				test2 = &big[1545];
			}
			
			//test 3
			if (big.size() > 114445)
			{
				if (test2 != nullptr)
				{
					if (test2 != &big[6])
					{
						std::cout << "REALOCATED TEST3!";
						std::cout << " | CAPACITY: " << big.capacity() << "\n";
					}
				}
				test3 = &big[114430];
			}

			//count reallocations
			std::cout << "COUNT: " << ++count << " | INCREASED BY: " << (double)((double)big.capacity() / (double)prev_capacity) << "\n";
			prev_capacity = big.capacity();
		}
	}
}
```

**Results**:

`std::vector`:

![std::vector1](https://i.gyazo.com/e101ce0a10864ac7f4b1b32951416527.png)
![std::vector2](https://i.gyazo.com/be7adbefe951ffa1777f2f1051f0ecc7.png)

`Vector`:

![vector1](https://i.gyazo.com/345bfa77f1328ab104a317914f650479.png)
![vector2](https://i.gyazo.com/a0365ddf9823be733661dd3696269fab.png)

**Outcome**: there are technically no differences. Almost same formula for reallocation used - change capacity by 1.5* previous_size + 1.

# Should I use your class instead of `std::vector`?

**Answer**: **No**. You shouldn't use it as it's not as stable, probably not as efficient overall. And there are still many functions missing. Iterators are probably buggy either. Just stick with `std::vector` as it's the standart.

Yes only if you want to improve it yourself. *Use with careful considerations.*
