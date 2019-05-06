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
	

public:
	//constructors	
	Vector() : size_(0), capacity_(0), elem_(nullptr) { }	//default constructor
	Vector(size_t capacity) : size_(0), capacity_(capacity), elem_(new T[capacity]) { }
	Vector(size_t capacity, T value) : size_(capacity), capacity_(capacity), elem_(new T[capacity]) { this->fill_n_(&elem_[0], capacity, value); }
	Vector(const Vector& obj);
	
	//get'er functions
	inline size_t size() const { return size_; }
	inline size_t capacity() const { return capacity_; }


	//functions
	T at(size_t n) const { if (n < size_) return elem_[n]; else throw std::out_of_range{ "Vector::at() index value out of range." }; }
	T front() const { if (size_ > 0) return elem_[0]; else throw std::logic_error{ "Vector::front() empty vector" }; }
	T back() const { if (size_ > 0) return elem_[size_ - 1]; else throw std::logic_error{ "Vector::back() empty vector" }; }
	void reserve(const size_t capacity);
	void push_back(const T value);
	bool empty() const;
	inline T* begin() const { if (size_ > 0) return &elem_[0]; }
	inline T* end() const { if (size_ > 0) return &elem_[size_]; }

	//operators
	T& operator[](size_t index) const;
	Vector<T>& operator=(const Vector<T>& obj);


	~Vector();

};

//PRIVATE FUNCTIONS

//copies values from source to destination in given range
//NOTE: destination is tend to be overwritten
template<class T>
void Vector<T>::copyValues_(T* src_begin, T* src_end, T* dstn)
{
	T* iterator = src_begin;	//iterator pointer
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
			//std::copy(&(elem_[0]), &(elem_[size_]), new_elem);	//copy elements to new array
			this->copyValues_(&(elem_[0]), &(elem_[size_]), new_elem);

		capacity_ = capacity;	//set capacity to new capacity
		elem_ = new_elem;	//point old array pointer to new array address
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
		delete elem_;	//delete previous records
		elem_ = new T[obj.capacity_];	//create new array
		capacity_ = obj.capacity_;
		size_ = obj.size_;
		this->copyValues_(&(obj.elem_[0]), &(obj.elem_[obj.size_]), elem_);	//copy valeus
	}
	return *this;
}

template<class T>
Vector<T>::~Vector()
{
	delete elem_;
}
