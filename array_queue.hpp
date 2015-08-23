#ifndef XCBASE_ARRAYQUEUE_INC
#define XCBASE_ARRAYQUEUE_INC 100
#
#include"rvalue_reference.hpp"
namespace xc{
	template<typename T, unsigned int size_>
	class array_queue {
		typedef array_queue<T, size_> my_type;
	public:
		struct iterator {
		private:
			T* ptr;
			T* buf;
		public:
			iterator() :ptr(0), buf(0) {}
			iterator(T* ptr_, T* buf_) :ptr(ptr_), buf(buf_) {}
			const iterator& operator++() {
				if (ptr == buf + size_)ptr = buf;
				else ++ptr;
				return *this;
			}
			const iterator& operator--() {
				if (ptr == buf)ptr = buf + size_;
				else --ptr;
				return *this;
			}
			iterator operator++(int) {
				iterator pre(*this);
				++(*this);
				return pre;
			}
			iterator operator--(int) {
				iterator pre(*this);
				--(*this);
				return pre;
			}
			T* operator->() { return ptr; }
			const T* operator->() const { return ptr; }
			T& operator*() { return *ptr; }
			const T& operator*() const { return *ptr; }
			friend bool operator==(iterator itr1, iterator itr2) {
				return itr1.ptr == itr2.ptr;
			}
			friend bool operator!=(iterator itr1, iterator itr2) {
				return itr1.ptr != itr2.ptr;
			}
		};
		struct const_iterator {
		private:
			T* ptr;
			T* buf;
		public:
			const_iterator() :ptr(0), buf(0) {}
			const_iterator(const iterator& itr_) :ptr(itr_.ptr), buf(itr_.buf) {}
			const_iterator(T* ptr_, T* buf_) :ptr(ptr_), buf(buf_) {}
			const const_iterator& operator++() {
				if (ptr == buf + size_)ptr = buf;
				else ++ptr;
				return *this;
			}
			const const_iterator& operator--() {
				if (ptr == buf)ptr = buf + size_;
				else --ptr;
				return *this;
			}
			const_iterator operator++(int) {
				iterator pre(*this);
				++(*this);
				return pre;
			}
			const_iterator operator--(int) {
				iterator pre(*this);
				--(*this);
				return pre;
			}
			const T* operator->() const { return ptr; }
			const T& operator*() const { return *ptr; }
			friend bool operator==(const_iterator itr1, const_iterator itr2) {
				return itr1.ptr == itr2;
			}
			friend bool operator!=(const_iterator itr1, const_iterator itr2) {
				return itr1.ptr != itr2;
			}
		};
	private:
		T Buf[size_ + 1];	//need not size_ but size_ + 1 because end should be --begin at full.
		iterator Begin;
		iterator End;
		unsigned int Size;
	public:
		array_queue() :Begin(Buf, Buf), End(Buf, Buf), Size(0){}
		bool empty()const { return Size==0; }
		bool full()const { return Size == size_; }
		bool push(const T& Val_) {
			if (full())return true;
			*End = Val_;
			++End;
			++Size;
			return false;
		}
		bool push(rvalue_reference<T> rrval_){
			if(full())return true;
			*End = xc::move(rrval_);
			++End;
			++Size;
			return false;
		}
		T& next() { return *Begin; }
		void pop() {
			if (empty())return;
			++Begin;
			--Size;
		}
		void clear() {
			End = Begin;
			Size = 0;
		}
		iterator begin() { return Begin; }
		iterator end() { return End; }
		const_iterator cbegin()const { return Begin; }
		const_iterator cend()const { return End; }
		unsigned int size()const{ return Size; }
	};
}
#
#endif
