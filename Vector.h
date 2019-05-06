#pragma once

#include <exception>

template <class T>
class Vector
{
private:
	size_t size_, capacity_;
	T *elem_;

	void copyValues_(T* src_begin, T* src_end, T* dstn);
	void fill_n_(T* begin, size_t qty, T value);
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
		inline iterator& operator++(int a) { ptr_++; return *this; }													//increment operator
		inline iterator& operator--(int a) { ptr_--; return *this; }													//decrement operator
		inline friend bool operator==(const iterator& a, const iterator& b) { return a.ptr_ == b.ptr_; }				//equal operator
		inline friend bool operator!=(const iterator& a, const iterator& b) { return !(a == b); }						//not equal operator
		
	};


	//constructors	
	Vector() : size_(0), capacity_(0), elem_(nullptr) { }	//default constructor
	Vector(size_t size) : size_(size), capacity_(size), elem_(new T[size]) { fill_n_(begin(), size, 0); }
	Vector(size_t size, T value) : size_(size), capacity_(size), elem_(new T[size]) { fill_n_(begin(), size, value); }
	Vector(const Vector& obj);								//copy constructor
	
	//get'er functions
	inline size_t size() const { return size_; }
	inline size_t capacity() const { return capacity_; }


	//functions
	T at(size_t n) const { if (n < size_) return elem_[n]; throw std::out_of_range{ "Vector::at() index value out of range." }; }	//returns value at index
	T front() const { if (size_ > 0) return elem_[0]; throw std::logic_error{ "Vector::front() empty vector" }; }					//return first value
	T back() const { if (size_ > 0) return elem_[size_ - 1]; throw std::logic_error{ "Vector::back() empty vector" }; }			//return last value  
	void reserve(const size_t capacity);	//reserves size
	void push_back(const T value);			//push back element at tail
	void swap(T& a, T& b);
	void insert(T* pos, const T value);		//single value insertion
	bool empty() const;						//check if array is empty
	inline T* begin() const { if (size_ > 0) return &elem_[0]; return nullptr; }	//begin iterator
	inline T* end() const { if (size_ > 0) return &elem_[size_]; return nullptr; }	//end iterator

	//operators
	T& operator[](size_t index) const;
	Vector<T>& operator=(const Vector<T>& obj);

	friend std::ostream& operator<<(std::ostream& out, const Vector& obj)	//stream operator
	{
		for (Vector::iterator it = obj.begin(); it != obj.end(); it++) 
			out << it << " "; 
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
inline typename std::remove_reference<T>::type&& Vector<T>::move_(T&& obj)
{
	return static_cast<typename std::remove_reference<T>::type&&> (obj);
}

//template<class T>
//inline typename std::remove_reference<T>::type&& Vector<T>::move_(T&& obj)
//{
//	static_cast<typename std::remove_reference<T>::type&&>(obj);
//}


//CONSTRUCTORS
template<class T>
Vector<T>::Vector(const Vector& obj) : size_(obj.size_), capacity_(obj.capacity_), elem_(new T[capacity_])
{
	this->copyValues_(obj.begin(), obj.end(), elem_);	//copy values
}


//PUBLIC FUNCTIONS
template<class T>
void Vector<T>::reserve(const size_t capacity)
{
	if (capacity > capacity_)	//check if reserve size is greater than size already built
	{
		T *new_elem = new T[capacity];

		if (!this->empty())
			this->copyValues_(begin(), end(), new_elem);	//copy elements to new array

		capacity_ = capacity;	//set capacity to new capacity
		elem_ = new_elem;		//point old array pointer to new array address
	}
}

template<class T>
void Vector<T>::push_back(const T value)
{
	if (size_ >= capacity_)	//if Vector is full, then increase capacity
		this->reserve((size_t)(1.5*capacity_));
	elem_[size_] = value;
	size_++;
}

template<class T>
void Vector<T>::swap(T& a, T& b)
{
	T temp = move_(a);
	a = move_(b);
	b = move_(a);
}

template<class T>
void Vector<T>::insert(T* pos, const T value)
{
	this->push_back(value);


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


template<class T>
Vector<T>::~Vector()
{
	delete elem_;
}
