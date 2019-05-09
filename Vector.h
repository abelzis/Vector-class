#pragma once

#include <exception>
#include <ostream>

template <class T>
class Vector
{
private:
	size_t size_, capacity_;
	T *elem_;

	//private functions
	void copyValues_(T* src_begin, T* src_end, T* dstn);
	void fill_n_(T* begin, size_t qty, T value);
	void reserve_(const size_t capacity);
	inline typename std::remove_reference<T>::type&& move_(T& obj);
	inline typename std::remove_reference<T>::type&& move_(T&& obj);

public:

	//iterator class
	class iterator
	{
	private:
		T* ptr_;	//pointer
	public:
		iterator() : ptr_(nullptr) { }						//default constructor
		iterator(T* ptr) : ptr_(ptr) { }
		iterator(T& value) : ptr_(&value) { }
		iterator(const iterator& it) : ptr_(it.ptr_) { }	//copy constructor
		iterator(T&& value) : ptr_(&value) { }				//move constructor

		//get'er
		inline T& value() const { return *ptr_; }
		inline T* pointer() const { return ptr_; }

		//set'er
		inline void setValue(const T& value) { if (ptr_ != nullptr) *ptr_ = value; }
		inline void setPointer(const T*& pointer) { ptr_ = pointer; }

		iterator& operator=(T* ptr) { ptr_ = ptr; return *this; }					//assigning another iterator
		iterator& operator=(const iterator& it) { ptr_ = it.ptr_; return *this; }	//assigning another iterator
		//assigning l-value
		T& operator=(T&& value)
		{
			if (ptr_ == nullptr)
				ptr_ = new int;
			return (*ptr_ = value);
		}
		//move assignment
		iterator& operator=(iterator&& it)
		{
			if (&it != this)
			{
				ptr_ = it.ptr_;
				it.ptr_ = nullptr;
			}
			return *this;
		}
		inline friend std::ostream& operator<<(std::ostream& out, const iterator& it) { return (out << *(it.ptr_)); }	//stream operator
		inline iterator& operator++() { ptr_++; return *this; }															//pre-increment operator
		inline iterator& operator--() { ptr_--; return *this; }															//pre-decrement operator
		inline iterator operator++(int a) { ptr_++; return *this; }														//post-increment operator
		inline iterator operator--(int a) { ptr_--; return *this; }														//post-decrement operator
		inline friend bool operator==(const iterator& a, const iterator& b) { return a.ptr_ == b.ptr_; }				//equal operator comparing to iterator
		inline friend bool operator==(const iterator& a, const T& b) { return *a.ptr_ == b; }							//equal operator comparing to value
		inline friend bool operator!=(const iterator& a, const iterator& b) { return !(a == b); }						//not equal operator comparing to iterator
		inline friend bool operator!=(const iterator& a, const T& b) { return !(a == b); }								//not equal operator comparing to value

		//~iterator() { delete ptr_; }	//destructor
	};	//END OF ITERATOR CLASS


	//constructors	
	Vector() : size_(0), capacity_(0), elem_(nullptr) { }	//default constructor
	Vector(size_t size) : size_(size), capacity_(size), elem_(new T[size]) { fill_n_(begin(), size, 0); }
	Vector(size_t size, T value) : size_(size), capacity_(size), elem_(new T[size]) { fill_n_(begin(), size, value); }
	Vector(const Vector& obj);								//copy constructor
	Vector(Vector<T>::iterator pos_start, T* pos_end);

	//get'er and set'er functions
	inline size_t size() const { return size_; }			//returns size
	inline size_t capacity() const { return capacity_; }	//returns capacity

	inline T at(const size_t n) const { if (n < size_) return elem_[n]; throw std::out_of_range{ "Vector::at() index value out of range." }; }					//returns value at index
	inline void at(const size_t n, const T& value) { if (n < size_) elem_[n] = value; throw std::out_of_range{ "Vector::at() index value out of range." }; }	//set value at index

	inline T front() const { if (size_ > 0) return elem_[0]; throw std::logic_error{ "Vector::front() empty vector" }; }					//return first value
	inline void front(const T& value) { if (size_ > 0) elem_[0] = value; throw std::logic_error{ "Vector::front() empty vector" }; }		//set first value

	inline T back() const { if (size_ > 0) return elem_[size_ - 1]; throw std::logic_error{ "Vector::back() empty vector" }; }				//return last value  
	inline void back(const T& value) { if (size_ > 0) elem_[size_ - 1] = value; throw std::logic_error{ "Vector::back() empty vector" }; }	//set last value

	//functions
	void reserve(const size_t capacity);	//reserves size
	void push_back(const T value);			//push back element at tail
	void pop_back();						//delete last element

		//swaps
	void swap(size_t pos_a, size_t pos_b);								//swaps two values at positions
	void swap(T* pos_a, T* pos_b);										//swaps two values
	void swap(Vector<T>& obj);											//swaps vectors
	void swap(Vector<T>::iterator pos_a, Vector<T>::iterator pos_b);	//swaps two values using iterators

	void insert(size_t pos, const T value);								//single value insertion

	void erase(size_t pos);													//single value erasion
	void erase(Vector<T>::iterator pos_start, T* pos_end);					//range erasion
	void erase(Vector<T>::iterator pos_start, Vector<T>::iterator pos_end);	//range erasion

	bool empty() const;						//check if array is empty
	inline void clear() { Vector<T>{}.swap(*this); }								//clears vector
	void shrink_to_fit() { if (size_ < capacity_) reserve_(size_); }
	inline T* begin() const { if (size_ > 0) return &elem_[0]; return nullptr; }	//begin iterator
	inline T* end() const { if (size_ > 0) return &elem_[size_]; return nullptr; }	//end iterator

	//operators
	T& operator[](size_t index) const;
	Vector<T>& operator=(const Vector<T>& obj);
	bool friend operator==(const Vector<T>& a, const Vector<T>& b)
	{
		if (a.size_ != b.size_)
			return false;
		for (size_t i = 0; i < a.size_; i++)
			if (a.elem_[i] != b.elem_[i])
				return false;
		return true;
	}

	friend std::ostream& operator<<(std::ostream& out, const Vector& obj)	//stream operator
	{
		if (!obj.empty())
			for (Vector::iterator it = obj.begin(); it != obj.end(); it++)
				out << it << " ";
		else
			out << "Vector object is empty.";
		return out;
	}

	~Vector();

};

//PRIVATE FUNCTIONS

//copies values from source to destination in given range
//NOTE: destination is tend to be overwritten
template<class T>
void Vector<T>::copyValues_(T* src_begin, T* src_end, T* dstn)
{
	T* iterator = src_begin;			//iterator pointer
	size_t index = 0;
	while (iterator != src_end)
		dstn[index++] = *iterator++;	//copy values
}


//fills n positions an array with given value
template<class T>
void Vector<T>::fill_n_(T* begin, size_t qty, T value)
{
	T* iterator = begin;
	for (size_t i = 0; i < qty; i++)
		*iterator++ = value;
}

template<class T>
void Vector<T>::reserve_(const size_t capacity)
{
	T *new_elem = new T[capacity];

	if (!this->empty())
		this->copyValues_(begin(), end(), new_elem);	//copy elements to new array

	delete[] elem_;
	capacity_ = capacity;	//set capacity to new capacity
	elem_ = new_elem;		//point old array pointer to new array address
}

template<class T>
inline typename std::remove_reference<T>::type&& Vector<T>::move_(T& obj)
{
	return (static_cast<typename std::remove_reference<T>::type&&> (obj));
}

template<class T>
inline typename std::remove_reference<T>::type&& Vector<T>::move_(T&& obj)
{
	return (static_cast<typename std::remove_reference<T>::type&&> (obj));
}


//CONSTRUCTORS
template<class T>
Vector<T>::Vector(const Vector& obj) : size_(obj.size_), capacity_(obj.capacity_), elem_(new T[capacity_])
{
	this->copyValues_(obj.begin(), obj.end(), elem_);	//copy values
}

template<class T>
Vector<T>::Vector(Vector<T>::iterator pos_start, T* pos_end)
{
	T* ptr_temp = pos_start.pointer();	//temp pointer to start
	int size_temp_ = 0;
	while (pos_start != pos_end)		//calculate size
	{
		size_temp_++;
		pos_start++;
	}

	//set values
	size_ = size_temp_;
	capacity_ = size_;
	elem_ = new T[size_];
	this->copyValues_(ptr_temp, pos_end, elem_);
}


//PUBLIC FUNCTIONS
template<class T>
void Vector<T>::reserve(const size_t capacity)
{
	if (capacity > capacity_)	//check if reserve size is greater than size already built
	{
		reserve_(capacity);
	}
}

template<class T>
void Vector<T>::push_back(const T value)
{
	if (size_ >= capacity_)	//if Vector is full, then increase capacity
		this->reserve((size_t)(1.5*capacity_ + 1));
	elem_[size_] = value;
	size_++;
}

template<class T>
void Vector<T>::pop_back()
{
	elem_[size_] = 0;
	size_--;
}

template<class T>
void Vector<T>::swap(size_t pos_a, size_t pos_b)
{
	//swap using move semantic (l-values
	T temp = move_(elem_[pos_a]);
	elem_[pos_a] = move_(elem_[pos_b]);
	elem_[pos_b] = move_(temp);
}

template<class T>
void Vector<T>::swap(T* pos_a, T* pos_b)
{
	//swap using move semantic (l-values)
	T temp = move_(*pos_a);
	*pos_a = move_(*pos_b);
	*pos_b = move_(temp);
}

template<class T>
void Vector<T>::swap(Vector<T>::iterator pos_a, Vector<T>::iterator pos_b)
{
	T value = pos_a.value();
	pos_a.setValue(pos_b.value());
	pos_b.setValue(value);
}

template<class T>
void Vector<T>::swap(Vector<T>& obj)
{
	//create temporary "object" to point to 'this'
	T* temp_elem_ = this->elem_;
	size_t temp_size_ = this->size_;
	size_t temp_capacity_ = this->capacity_;

	//point 'this' to 'obj'
	this->elem_ = obj.elem_;
	this->size_ = obj.size_;
	this->capacity_ = obj.capacity_;

	//point 'obj' to "object"
	obj.elem_ = temp_elem_;
	obj.size_ = temp_size_;
	obj.capacity_ = temp_capacity_;

	//delete pointer
	temp_elem_ = nullptr;
}

template<class T>
void Vector<T>::insert(size_t pos, const T value)
{
	this->push_back(value);
	//TODO: WRITE INSERTION FUNCTION AND DELETION FUNCTION

	Vector<T>::iterator it1 = (this->end());
	it1--;

	Vector<T>::iterator it2 = it1;
	it2--;
	while (it1 != &elem_[pos])
	{
		this->swap(it1, it2);
		it1--;
		it2--;
	}
}

template<class T>
void Vector<T>::erase(size_t pos)
{
	Vector<T>::iterator it1 = &elem_[pos], it2 = &elem_[pos + 1];

	while (it2 != end())
	{
		this->swap(it1, it2);
		it1++;
		it2++;
	}

	this->pop_back();
}

template<class T>
void Vector<T>::erase(Vector<T>::iterator pos_start, T* pos_end)
{
	Vector<T>::iterator end_end = this->end();
	if (pos_end == this->end())
		while (pos_start != pos_end)
		{
			pos_start++;
			size_--;
		}

	else
	{
		Vector<T>::iterator it = pos_end;
		while (it != this->end())
		{
			this->swap(pos_start, it);
			pos_start++;
			it++;
			size_--;
		}
	}
}

template<class T>
void Vector<T>::erase(Vector<T>::iterator pos_start, Vector<T>::iterator pos_end)
{
	Vector<T>::iterator end_it = pos_end;
	if (pos_end == this->end())
		while (pos_start != pos_end)
		{
			pos_start++;
			size_--;
		}

	else
	{
		while (pos_start != this->end())
		{
			if (pos_start != end_it)
				size_--;
			if (pos_start == end_it)
				end_it++;
			this->swap(pos_start, pos_end);
			pos_start++;
			pos_end++;
		}
		//size_++;
	}
}

template<class T>
bool Vector<T>::empty() const
{
	if (size_ == 0)	//if empty
		return true;
	return false;
}


//OPERATORS
template<class T>
T& Vector<T>::operator[](size_t index) const
{
	if (index < 0 || index >= size_)
		throw std::out_of_range{ "Vector::operator[] index value out of range." };

	return elem_[index];	//return value
}


template<class T>
Vector<T>& Vector<T>::operator=(const Vector<T>& obj)
{
	if (&obj != this)
	{
		delete elem_;					//delete previous records
		elem_ = new T[obj.capacity_];	//create new array
		capacity_ = obj.capacity_;
		size_ = obj.size_;
		this->copyValues_(&(obj.elem_[0]), &(obj.elem_[obj.size_]), elem_);		//copy valeus
	}
	return *this;
}

//DESTRUCTOR
template<class T>
Vector<T>::~Vector()
{
	size_ = 0;
	capacity_ = 0;
	delete[] elem_;
}
