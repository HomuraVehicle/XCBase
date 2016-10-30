#ifndef XC_CHAIN_INC
#define XC_CHAIN_INC 100
#
#include<iterator>
#include<functional>
#include"rvalue_reference.hpp"
namespace xc{
	template<typename T>
	struct chain{
	public:
		typedef chain<T> this_type;
		typedef unsigned int size_type;
	public:
		struct element{
			friend struct chain<T>;
		private:
			T value;
			element *prev;
			element *next;
		public:
			element() :prev(0), next(0){}
			explicit element(const T& value_) :value(value_), prev(0), next(0){}
			element(const element& Elm) :value(Elm.value), prev(0), next(0){}
			element& operator=(const element& Elm){
				if(this != &Elm){
					value = Elm.value;
				}
				return *this;
			}
		public:
			operator bool(){ return next == 0; }
			T& operator*(){ return value; }
			const T& operator*()const{ return value; }
			T* operator->(){ return &value; }
			const T* operator->()const{ return &value; }
		public:
			static void connect(element& Prev, element& Next){
				Prev.next = &Next;
				Next.prev = &Prev;
			}
		};
	public:
		struct const_iterator;
		struct iterator :public std::iterator<std::bidirectional_iterator_tag, T>{
		private:
			element* Cur;
		public:
			iterator() :Cur(0){}
			explicit iterator(element* Cur_) :Cur(Cur_){}
			iterator(const iterator& Other) :Cur(Other.Cur){}
			iterator& operator=(const iterator& Other){
				if(this != &Other){
					Cur = Other.Cur;
				}
				return *this;
			}
		public:
			iterator& operator++(){
				Cur = Cur->next;
				return *this;
			}
			iterator operator++(int){
				iterator Ans(*this);
				this->operator++();
				return Ans;
			}
			iterator& operator--(){
				Cur = Cur->prev;
				return *this;
			}
			iterator operator--(int){
				iterator Ans(*this);
				this->operator--();
				return Ans;
			}
			T& operator*(){ return Cur->operator*(); }
			const T& operator*()const{ return Cur->operator*(); }
			T* operator->(){ return Cur->operator->(); }
			const T* operator->()const{ return Cur->operator->(); }
			operator const_iterator();
			friend bool operator==(const iterator& Itr1, const iterator& Itr2){
				return Itr1.Cur == Itr2.Cur;
			}
			friend bool operator!=(const iterator& Itr1, const iterator& Itr2){
				return Itr1.Cur != Itr2.Cur;
			}
		public:
			element* current()const{ return Cur; }
		};
		struct const_iterator :public std::iterator<std::bidirectional_iterator_tag, const T>{
		private:
			element* Cur;
		public:
			const_iterator() :Cur(0){}
			explicit const_iterator(element* Cur_) :Cur(Cur_){}
			const_iterator(const iterator& Other) :Cur(Other.current()){}
			const_iterator(const const_iterator& Other) :Cur(Other.Cur){}
			const_iterator& operator=(const const_iterator& Other){
				if(this != &Other){
					Cur = Other.Cur;
				}
				return *this;
			}
		public:
			const_iterator& operator++(){
				Cur = Cur->next;
				return *this;
			}
			const_iterator operator++(int){
				iterator Ans(*this);
				this->operator++();
				return Ans;
			}
			const_iterator& operator--(){
				Cur = Cur->prev;
				return *this;
			}
			const_iterator operator--(int){
				iterator Ans(*this);
				this->operator--();
				return Ans;
			}
			const T& operator*()const{ return Cur->operator*(); }
			const T* operator->()const{ return Cur->operator->(); }
			friend bool operator==(const const_iterator& Itr1, const const_iterator& Itr2){
				return Itr1.Cur == Itr2.Cur;
			}
			friend bool operator!=(const const_iterator& Itr1, const const_iterator& Itr2){
				return Itr1.Cur != Itr2.Cur;
			}
		public:
			element* current(){ return Cur; }
		};
	private:
		element Sentinel;
		size_type Size;
	public:
		chain() :Size(0){
			Sentinel.next = &Sentinel;
			Sentinel.prev = &Sentinel;
		}
		chain(rvalue_reference<this_type>& rref){
			if(rref.ref.empty()){
				Size = 0;
				Sentinel.next = &Sentinel;
				Sentinel.prev = &Sentinel;
				return;
			}

			element::connect(Sentinel, *(rref.ref.Sentinel.next));
			element::connect(*(rref.ref.Sentinel.prev), Sentinel);
			Size = rref.ref.Size;

			rref.ref.Sentinel.next = &(rref.ref.Sentinel);
			rref.ref.Sentinel.prev = &(rref.ref.Sentinel);
			rref.ref.Size = 0;
		}
		this_type& operator=(rvalue_reference<this_type>& rref){
			if(this!=&(rref.ref)){
				clear();

				if(!rref.ref.empty()){
					element::connect(Sentinel, *(rref.ref.Sentinel.next));
					element::connect(*(rref.ref.Sentinel.prev), Sentinel);
					Size = rref.ref.Size;

					rref.ref.Sentinel.next = &(rref.ref.Sentinel);
					rref.ref.Sentinel.prev = &(rref.ref.Sentinel);
					rref.ref.Size = 0;
				}
			}

			return *this;
		}
	private:
		chain(const this_type&){}
		this_type& operator=(const this_type&){}
	public:
		iterator begin(){ return iterator(Sentinel.next); }
		iterator end(){ return iterator(&Sentinel); }
		const_iterator cbegin()const{ return const_iterator(Sentinel.next); }
		const_iterator cend()const{ return const_iterator(&Sentinel); }
		const_iterator begin()const{ return cbegin(); }
		const_iterator end()const{ return cend(); }
	public:
		bool empty(){ return Size == 0; }
		size_type size(){ return Size; }
		T& front(){ return *(*(Sentinel.next)); }
		T& back(){ return *(*(Sentinel.prev)); }
		const T& front()const{ return *(*(Sentinel.next)); }
		const T& back()const{ return *(*(Sentinel.prev)); }
		void push_back(element& Elem){
			element& Prev = *(Sentinel.prev);
			element::connect(Prev, Elem);
			element::connect(Elem, Sentinel);

			++Size;
		}
		void pop_back(){
			if(empty())return;

			element& Prev = *(Sentinel.prev);
			element::connect(*(Prev.prev), Sentinel);
			Prev.next = 0;
			Prev.prev = 0;

			--Size;
		}
		void push_front(element& Elem){
			element& Next = *(Sentinel.next);
			element::connect(Sentinel, Elem);
			element::connect(Elem, Next);

			++Size;
		}
		void pop_front(){
			if(empty())return;

			element& Next = *(Sentinel.next);
			element::connect(Sentinel, *(Next.next));
			Next.next = 0;
			Next.prev = 0;

			--Size;
		}
		iterator insert(const_iterator pos, element& Elem){
			element& Curr = *pos.current();
			element::connect(*(Curr.prev), Elem);
			element::connect(Elem, Curr);

			++Size;

			return iterator(Curr.prev);
		}
		iterator erase(const_iterator pos){
			if(pos == end())return end();

			element& Curr = *pos.current();
			element& Next = *(Curr.next);
			element::connect(*(Curr.prev), *(Curr.next));
			Curr.next = 0;
			Curr.prev = 0;

			--Size;

			return iterator(&Next);
		}
		iterator erase(const_iterator pos, const_iterator last){
			if(pos == end())return end();

			element& Prev = *(pos.current()->prev);

			while(pos != last){
				element& Curr = *(pos.current());
				++pos;

				Curr.next = 0;
				Curr.prev = 0;
				--Size;
			}

			element::connect(Prev, *(last.current()));
			return iterator(last.current());
		}
		void swap(this_type& other){
			std::swap(Sentinel.prev, other.Sentinel.prev);
			std::swap(Sentinel.next, other.Sentinel.next);
			std::swap(Size, other.Size);
		}
		void clear(){
			erase(begin(), end());
		}
		void splice(const_iterator pos, this_type& other){
			if(other.empty())return;
			auto AddSize = other.size();

			element& Prev = *(pos.current().prev);
			element& Curr = *(pos.current());

			element::connect(Prev, *(other.Sentinel.next));
			element::connect(*(other.Sentinel.prev), Curr);
			other.Sentinel.next = &(other.Sentinel);
			other.Sentinel.prev = &(other.Sentinel);

			Size += AddSize;
		}
	};
	template<typename T>
	chain<T>::iterator::operator typename xc::chain<T>::const_iterator(){
		return xc::chain<T>::const_iterator(current());
	}

	template<typename T, typename compare_ = std::less<T>>
	struct sorted_chain{
	public:
		typedef chain<T> this_type;
		typedef unsigned int size_type;
		typedef compare_ compare;
	public:
		struct element{
			friend struct sorted_chain<T>;
		private:
			T value;
			element *next;
		public:
			element() :next(0){}
			explicit element(const T& value_) :value(value_), next(0){}
			element(const element& Elm) :value(Elm.value), next(0){}
			element& operator=(const element& Elm){
				if(this != &Elm){
					value = Elm.value;
				}
				return *this;
			}
		public:
			operator bool(){ return next == 0; }
			T& operator*(){ return value; }
			const T& operator*()const{ return value; }
			T* operator->(){ return &value; }
			const T* operator->()const{ return &value; }
		public:
			static void connect(element& Prev, element& Next){
				Prev.next = &Next;
			}
		};
	public:
		struct const_iterator;
		struct iterator :public std::iterator<std::forward_iterator_tag, T>{
		private:
			element* Cur;
		public:
			iterator() :Cur(0){}
			explicit iterator(element* Cur_) :Cur(Cur_){}
			iterator(const iterator& Other) :Cur(Other.Cur){}
			iterator& operator=(const iterator& Other){
				if(this != &Other){
					Cur = Other.Cur;
				}
				return *this;
			}
		public:
			iterator& operator++(){
				Cur = Cur->next;
				return *this;
			}
			iterator operator++(int){
				iterator Ans(*this);
				this->operator++();
				return Ans;
			}
			T& operator*(){ return Cur->operator*(); }
			const T& operator*()const{ return Cur->operator*(); }
			T* operator->(){ return Cur->operator->(); }
			const T* operator->()const{ return Cur->operator->(); }
			operator const_iterator();
			friend bool operator==(const iterator& Itr1, const iterator& Itr2){
				return Itr1.Cur == Itr2.Cur;
			}
			friend bool operator!=(const iterator& Itr1, const iterator& Itr2){
				return Itr1.Cur != Itr2.Cur;
			}
		public:
			element* current()const{ return Cur; }
		};
		struct const_iterator :public std::iterator<std::forward_iterator_tag, const T>{
		private:
			element* Cur;
		public:
			const_iterator() :Cur(0){}
			explicit const_iterator(element* Cur_) :Cur(Cur_){}
			const_iterator(const iterator& Other) :Cur(Other.current()){}
			const_iterator(const const_iterator& Other) :Cur(Other.Cur){}
			const_iterator& operator=(const const_iterator& Other){
				if(this != &Other){
					Cur = Other.Cur;
				}
				return *this;
			}
		public:
			const_iterator& operator++(){
				Cur = Cur->next;
				return *this;
			}
			const_iterator operator++(int){
				iterator Ans(*this);
				this->operator++();
				return Ans;
			}
			const T& operator*()const{ return Cur->operator*(); }
			const T* operator->()const{ return Cur->operator->(); }
			friend bool operator==(const const_iterator& Itr1, const const_iterator& Itr2){
				return Itr1.Cur == Itr2.Cur;
			}
			friend bool operator!=(const const_iterator& Itr1, const const_iterator& Itr2){
				return Itr1.Cur != Itr2.Cur;
			}
		public:
			element* current(){ return Cur; }
		};
	private:
		element Sentinel;
		compare Comp;
	public:
		sorted_chain(){
			Sentinel.next = &Sentinel;
		}
		sorted_chain(xc::rvalue_reference<this_type>& Otehr){
			if(Otehr.ref.empty()){
				Sentinel.next = &Sentinel;
				return;
			}

			element& Cur = Otehr.ref.Sentinel;
			while(Cur.next != Otehr.ref.Sentinel){
				Cur = *(Cur.next);
			}

			Sentinel.next = Otehr.ref.Sentinel.next;
			Cur.next = &Sentinel.next;

			Otehr.ref.Sentinel.next = &(Otehr.ref.Sentinel);
		}
		this_type& operator=(xc::rvalue_reference<this_type>& Otehr){
			if(this != &Otehr.ref){
				clear();

				if(Otehr.ref.empty())return;

				element& Cur = Otehr.ref.Sentinel;
				while(Cur.next != Otehr.ref.Sentinel){
					Cur = *(Cur.next);
				}

				Sentinel.next = Otehr.ref.Sentinel.next;
				Cur.next = &Sentinel.next;

				Otehr.ref.Sentinel.next = &(Otehr.ref.Sentinel);
			}

			return *this;
		}
	private:
		sorted_chain(const this_type&){}
		this_type& operator=(const this_type&){}
	public:
		iterator begin(){ return iterator(Sentinel.next); }
		iterator end(){ return iterator(&Sentinel); }
		const_iterator cbegin()const{ return const_iterator(Sentinel.next); }
		const_iterator cend()const{ return const_iterator(&Sentinel); }
		const_iterator begin()const{ return cbegin(); }
		const_iterator end()const{ return cend(); }
	public:
		bool empty(){ return Sentinel.next == &Sentinel; }
		T& next(){ return *(*(Sentinel.next)); }
		const T& next()const{ return *(*(Sentinel.next)); }
		void push(element& Elem){
			element* p1 = Sentinel.next;
			element* p2 = &Sentinel;
			while(true){
				//check with p2 < Elem < p1?
				if(p1 == &Sentinel || Comp(*Elem, *(*p1))){
					element::connect(*p2, Elem);
					element::connect(Elem, *p1);
					return;
				}
				p2 = p1->next;

				//check with p1 < Elem < p2?
				if(p2 == &Sentinel || Comp(*Elem, *(*p2))){
					element::connect(*p1, Elem);
					element::connect(Elem, *p2);
					return;
				}
				p1 = p2->next;
			}
		}
		void pop(){
			element& Next = *(Sentinel.next);
			element::connect(Sentinel, *(Next.next));
			Next.next = 0;
		}
		void swap(this_type& other){
			std::swap(Sentinel.next, other.Sentinel.next);
			std::swap(Size, other.Size);
		}
		void clear(){
			element* p1 = Sentinel.next;
			element* p2 = &Sentinel;
			while(true){
				//check with p1 == end?
				if(p1 == &Sentinel){
					Sentinel.next = &Sentinel;
					return;
				}
				p2 = p1->next;
				p1->next = 0;

				//check with p2 == end?
				if(p2 == &Sentinel){
					Sentinel.next = &Sentinel;
					return;
				}
				p1 = p2->next;
				p2->next = 0;
			}
		}
	};
	template<typename T, typename compare_>
	sorted_chain<T, compare_>::iterator::operator typename xc::sorted_chain<T, compare_>::const_iterator(){
		return xc::sorted_chain<T>::const_iterator(current());
	}
}
#
#endif
