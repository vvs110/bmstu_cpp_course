#pragma once

#include <exception>
#include <iostream>
#include <utility>

namespace bmstu
{
template <typename T>
class stack
{
   public:
	stack()
		: data_(nullptr),
		  size_(0),
		  capacity_(0) {}  // создает пустой стек с нулевым указателем на ячейку
						   // памяти, начальные размер и емкость равны нулю

	bool empty() const noexcept
	{
		return size_ == 0;
	}  // возвращает true если размер равен нулю

	size_t size() const noexcept
	{
		return size_;
	}  // возвращает количество элементов в стеке

	template <typename... Args>
	void emplace(Args&&... args)
	{
		if (size_ == capacity_)	 // проверяет есть ли место
		{
			resize();  // если нет, вызывает функцию resize()
		}
		new (data_ + size_) T(std::forward<Args>(
			args)...);	// конструирует объект типа Т по адресу data_+size_ и
						// передает аргументы args
		++size_;  // увеличиваем чтобы поместить аргументы в новую ячейку памяти
	}

	void push(T&& value)  // принимает временный объект Rvalue, передает его в
						  // placement new чтобы вызвать конструктор перемещения
	{
		if (size_ == capacity_)
		{
			resize();
		}
		new (data_ + size_) T(std::move(value));
		++size_;
	}

	void clear() noexcept  // вызвает деструктор каждого объекта типа Т
	{
		while (size_ > 0)
		{
			--size_;
			(data_ + size_)->~T();
		}
	}

	void push(const T& value)  // принимает существующий объект Lvalue и
							   // вызывает конструктор копирования
	{
		if (size_ == capacity_)
		{
			resize();
		}
		new (data_ + size_) T(value);
		++size_;
	}

	void pop()
	{
		if (empty())  // если стек пуст выбрасывается ошибка
		{
			throw std::underflow_error("Стек пустой");
		}
		--size_;
		(data_ + size_)->~T();	// если нет, то уменьшаем размер и уничтожаем
								// последний объект
	}

	T& top()  // возвращает ссылку на последний элемент
	{
		if (empty())
		{
			throw std::underflow_error("Стек пустой, нет доступа к top");
		}
		return data_[size_ - 1];
	}

	const T& top() const  // аналогично для константных объектов стека
	{
		if (empty())
		{
			throw std::underflow_error("Стек пустой, нет доступа к top");
		}
		return data_[size_ - 1];
	}
	void resize()
	{
		size_t new_capacity =
			(capacity_ == 0)
				? 1
				: capacity_ + 1;  // если вместимость равна нулю, то выделяет
								  // только одну ячейку в памяти, если нет, то
								  // увеличивает размер на единицу
		T* new_data = (T*)::operator new(
			new_capacity * sizeof(T));	// выделяет блок сырой памяти нужного
										// размера без вызова конструктора
		for (size_t i = 0; i < size_; ++i)
		{
			new (new_data + i) T(std::move(
				data_[i]));	 // перемещает объект из старой памяти в новую
			(data_ + i)->~T();	// разрушает старый объект
		}
		::operator delete(data_);  // освобождение памяти
		data_ = new_data;
		capacity_ = new_capacity;
	}

   private:
	T* data_;  // указатель на "сырую" память, где лежат объекты
	size_t size_;	   // количество элементов в стеке
	size_t capacity_;  // размер выделенной памяти
};
}  // namespace bmstu
