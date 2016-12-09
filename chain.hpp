#ifndef XC_CHAIN_INC
#define XC_CHAIN_INC 100
#
#include<iterator>
#include<functional>
#include"rvalue_reference.hpp"
namespace xc{
	namespace{
		struct chain_base;
		struct chain_element_t{
			friend struct chain_base;
		private:
			chain_element_t* prev;
			chain_element_t* next;
		public:
			chain_element_t() :prev(0), next(0){}
			bool owned_by_chain()const{return prev != 0;}
		};
		struct chain_base{
			typedef chain_element_t element;
			static void clear_element(element& Elem){
				Elem.next = 0;
				Elem.prev = 0;
			}
			static bool insert_element(element& New, element& Pos){
				if(New.next != 0 || New.prev != 0)return true;

				Pos.prev->next = &New;
				New.prev = Pos.prev;

				Pos.prev = &New;
				New.next = &Pos;

				return false;
			}
			static bool splice_range_element(element& First, element& Last, element& Pos){
				if(First.next != 0 || First.prev != 0)return true;
				if(Last.next != 0 || Last.prev != 0)return true;

				element& Before = *(Pos.prev);

				Last.prev->next = &Pos;
				First.prev->next = &Last;

				Pos.prev = Last.prev;
				Last.prev = First.prev;

				First.prev = &Before;
				Before.next = &First;

				return false;
			}
			static bool splice_element(element& Elem, element& Pos){
				if(Elem.next != 0 || Elem.prev != 0)return true;

				element& Before = *(Pos.prev);

				Elem.next->prev = Elem.prev;
				Elem.prev->next = Elem.next;

				Before.next = &Elem;
				Elem.prev = &Before;

				Before.prev = &Elem;
				Elem.next = &Pos;

				return false;
			}
			static bool erase_element(element& Erased){
				if(Erased.next == 0 || Erased.prev == 0)return true;

				Erased.next->prev = Erased.prev;
				Erased.prev->next = Erased.next;

				clear_element(Erased);

				return false;
			}
			static unsigned int erase_range_element(element& First, element& Last){
				if(First.next == 0 || First.prev == 0)return 0;
				if(Last.next == 0 || Last.prev == 0)return 0;

				element* Ptr = &First;
				First.prev->next = &Last;
				Last.prev = First.prev;

				unsigned int Cnt = 0;
				while(Ptr != &Last){
					element* Cur = Ptr;
					Ptr = Ptr->next;
					clear_element(*Cur);
					++Cnt;
				}

				return Cnt;
			}
			static void selfing(element& Elem){
				Elem.next = &Elem;
				Elem.prev = &Elem;
			}
			static element& prev(element& Elem){ return *(Elem.prev); }
			static element& next(element& Elem){ return *(Elem.next); }
		};
	}
	typedef chain_element_t chain_element;

	template<typename T>
	struct chain: private chain_base{
	private:
		typedef chain_base base;
	public:
		typedef chain<T> this_type;
		typedef unsigned int size_type;
		typedef chain_element element;
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
				Cur = &base::next(*Cur);
				return *this;
			}
			iterator operator++(int){
				iterator Ans(*this);
				this->operator++();
				return Ans;
			}
			iterator& operator--(){
				Cur = &base::prev(*Cur);
				return *this;
			}
			iterator operator--(int){
				iterator Ans(*this);
				this->operator--();
				return Ans;
			}
			T& operator*(){ return *static_cast<T*>(Cur); }
			const T& operator*()const{ return *static_cast<const T*>(Cur); }
			T* operator->(){ return static_cast<T*>(Cur); }
			const T* operator->()const{ return static_cast<const T*>(Cur); }
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
			const element* Cur;
		public:
			const_iterator() :Cur(0){}
			explicit const_iterator(const element* Cur_) :Cur(Cur_){}
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
				Cur = &base::next(*Cur);
				return *this;
			}
			const_iterator operator++(int){
				iterator Ans(*this);
				this->operator++();
				return Ans;
			}
			const_iterator& operator--(){
				Cur = &base::prev(*Cur);
				return *this;
			}
			const_iterator operator--(int){
				iterator Ans(*this);
				this->operator--();
				return Ans;
			}
			const T& operator*()const{ return *static_cast<const T*>(Cur); }
			const T* operator->()const{ return static_cast<const T*>(Cur); }
			friend bool operator==(const const_iterator& Itr1, const const_iterator& Itr2){
				return Itr1.Cur == Itr2.Cur;
			}
			friend bool operator!=(const const_iterator& Itr1, const const_iterator& Itr2){
				return Itr1.Cur != Itr2.Cur;
			}
		public:
			const element* current()const{ return Cur; }
		};
	private:
		element Sentinel;
		size_type Size;
	public:
		chain() :Size(0){
			base::selfing(Sentinel);
		}
		chain(rvalue_reference<this_type>& rref){
			if(rref.ref.empty()){
				Size = 0;
				base::selfing(Sentinel);
				return;
			}

			Size = rref.ref.Size;
			rref.ref.Size = 0;
			base::splice_range_element(base::next(rref.ref.Sentinel), rref.ref.Sentinel,Sentinel);
		}
		this_type& operator=(rvalue_reference<this_type>& rref){
			if(this!=&(rref.ref)){
				clear();

				if(!rref.ref.empty()){
					Size = rref.ref.Size;
					rref.ref.Size = 0;
					base::splice_range_element(base::next(rref.ref.Sentinel), rref.ref.Sentinel, Sentinel);
				}
			}

			return *this;
		}
	private:
		chain(const this_type&){}
		this_type& operator=(const this_type&){}
	public:
		iterator begin(){ return iterator(&base::next(Sentinel)); }
		iterator end(){ return iterator(&Sentinel); }
		const_iterator cbegin()const{ return const_iterator(&base::next(Sentinel)); }
		const_iterator cend()const{ return const_iterator(&Sentinel); }
		const_iterator begin()const{ return cbegin(); }
		const_iterator end()const{ return cend(); }
	public:
		bool empty(){ return Size == 0; }
		size_type size(){ return Size; }
		T& front(){ return static_cast<T&>(base::next(Sentinel)); }
		T& back(){ return static_cast<T&>(base::prev(Sentinel)); }
		const T& front()const{ return static_cast<const T&>(base::next(Sentinel)); }
		const T& back()const{ return static_cast<const T&>(base::prev(Sentinel)); }
		void push_back(T& Elem){
			++Size;
			base::insert_element(Elem,  Sentinel);
		}
		void pop_back(){
			if(empty())return;

			--Size;
			base::erase_element(base::prev(Sentinel));
		}
		void push_front(T& Elem){
			++Size;
			base::insert_element(Elem, base::next(Sentinel));
		}
		void pop_front(){
			if(empty())return;

			--Size;
			base::erase_element(base::next(Sentinel));
		}
		iterator insert(const_iterator pos, T& Elem){
			++Size;
			base::insert_element(Elem, *const_cast<element*>(pos.current()));

			return iterator(Elem);
		}
		iterator erase(const_iterator pos){
			if(pos == end())return end();

			element& Curr = *pos.current();
			element& Next = *(Curr.next);

			--Size;
			base::erase_element(Curr);

			return iterator(&Next);
		}
		iterator erase(const_iterator first, const_iterator last){
			if(first == end())return end();

			Size -= base::erase_range_element(*const_cast<element*>(first.current()), *const_cast<element*>(last.current()));
			return iterator(last.current());
		}
		iterator find(const T& Elem){
			iterator Itr = begin();
			for(;Itr != end(); ++Itr){
				if(Itr.current() == &Elem)break;
			}
			return Itr;
		}
		void swap(this_type& other){
			std::swap(Sentinel, other.Sentinel);
			std::swap(Size, other.Size);
		}
		void clear(){
			erase(begin(), end());
		}
		void splice(const_iterator pos, this_type& other){
			if(other.empty())return;

			Size += other.size();
			other.Size = 0;

			base::splice_range_element(base::next(other.Sentinel), other.Sentinel, *const_cast<element*>(pos.current()));
		}
		void splice(const_iterator pos, this_type& other, const_iterator elem){
			if(other.empty())return;

			++Size;
			--(other.Size);

			base::splice_element(*const_cast<element*>(elem.current()), *const_cast<element*>(pos.current()));
		}
		void splice(const_iterator pos, this_type& other, const_iterator first, const_iterator last){
			int Num = std::distance(first, last);

			Size += Num;
			other.Size -= Num;

			base::splice_range_element(*const_cast<element*>(first.current()), *const_cast<element*>(last.current()),*const_cast<element*>(pos.current()));
		}
	};
	template<typename T>
	chain<T>::iterator::operator typename xc::chain<T>::const_iterator(){
		return xc::chain<T>::const_iterator(current());
	}

	namespace{
		struct forward_chain_base;
		struct forward_chain_element_t{
			friend struct forward_chain_base;
		private:
			forward_chain_element_t* next;
		public:
			forward_chain_element_t() :next(0){}
		};
		struct forward_chain_base{
			typedef forward_chain_element_t element;
			static void clear_element(element& Elem){
				Elem.next = 0;
			}
			static bool insert_after_element(element& New, element& Pos){
				if(New.next != 0)return true;

				New.next = Pos.next;
				Pos.next = &New;

				return false;
			}
			static bool splice_range_after_element(element& BeforeFirst, element& BeforeLast, element& Pos){
				if(BeforeFirst.next != 0)return true;
				if(BeforeLast.next != 0)return true;

				element& Next = *(Pos.next);

				Pos.next = BeforeFirst.next;
				BeforeFirst.next = BeforeLast.next;
				BeforeLast.next = &Next;

				return false;
			}
			static bool splice_after_element(element& ElemBefore, element& Pos){
				if(ElemBefore.next != 0)return true;

				element& Elem = *(ElemBefore.next);
				element& Next = *(Pos.next);

				Pos.next = ElemBefore.next;
				ElemBefore.next = Elem.next;
				Elem.next = &Next;

				return false;
			}
			static bool erase_after_element(element& Pos){
				if(Pos.next == 0)return true;

				element& Next = *(Pos.next);

				Pos.next = Next.next;
				clear_element(Next);

				return false;
			}
			static unsigned int erase_range_after_element(element& BeforeFirst, element& BeforeLast){
				if(BeforeFirst.next != 0)return true;
				if(BeforeLast.next != 0)return true;

				element* Ptr = BeforeFirst.next;
				BeforeFirst.next = BeforeLast.next;

				unsigned int Cnt = 0;
				while(true){
					element* Cur = Ptr;
					Ptr = Ptr->next;
					clear_element(*Cur);
					++Cnt;
					if(Cur == &BeforeLast)break;
				}

				return Cnt;
			}
			static void selfing(element& Elem){
				Elem.next = &Elem;
			}
			static element& next(element& Elem){ return *(Elem.next); }
		};
	}
	typedef forward_chain_element_t sorted_chain_element;

	template<typename T, typename compare_ = std::less<T>>
	struct sorted_chain: public forward_chain_base{
	private:
		typedef forward_chain_base base;
	public:
		typedef chain<T> this_type;
		typedef unsigned int size_type;
		typedef sorted_chain_element element;
		typedef compare_ compare;
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
				Cur = &base::next(*Cur);
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
				Cur = &base::next(*Cur);
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
			base::selfing(Sentinel);
		}
		sorted_chain(xc::rvalue_reference<this_type>& rref){
			if(rref.ref.empty()){
				base::selfing(Sentinel);
				return;
			}

			element* Ptr = rref.ref.Sentinel;
			while(&base::next(*Ptr) != &(rref.ref.Sentinel)){
				Ptr = &base::next(*Ptr);
			}

			base::splice_range_after_element(rref.ref.Sentinel, *Ptr, Sentinel);
		}
		this_type& operator=(xc::rvalue_reference<this_type>& rref){
			if(this != &(rref.ref)){
				clear();

				if(!rref.ref.empty()){
					element* Ptr = rref.ref.Sentinel;
					while(&base::next(*Ptr) != &(rref.ref.Sentinel)){
						Ptr = &base::next(*Ptr);
					}

					base::splice_range_after_element(rref.ref.Sentinel, *Ptr, Sentinel);
				}
			}

			return *this;
		}
	private:
		sorted_chain(const this_type&){}
		this_type& operator=(const this_type&){}
	public:
		iterator begin(){ return iterator(&base::next(Snetinel)); }
		iterator end(){ return iterator(&Sentinel); }
		const_iterator cbegin()const{ return const_iterator(&base::next(Snetinel)); }
		const_iterator cend()const{ return const_iterator(&Sentinel); }
		const_iterator begin()const{ return cbegin(); }
		const_iterator end()const{ return cend(); }
	public:
		bool empty(){ return &base::next(Sentinel) == &Sentinel; }
		T& next(){ return static_cast<T&>(base::next(Sentinel)); }
		const T& next()const{ return static_cast<const T&>(base::next(Sentinel)); }
		void push(element& Elem){
			element* p1 = Sentinel.next;
			element* p2 = &Sentinel;
			while(true){
				//check with p2 < Elem < p1?
				if(p1 == &Sentinel || Comp(static_cast<T&>(Elem), static_cast<T&>(*p1))){
					base::insert_after_element(Elem, *p1);
					return;
				}
				p2 = p1->next;

				//check with p1 < Elem < p2?
				if(p2 == &Sentinel || Comp(static_cast<T&>(Elem), static_cast<T&>(*p2))){
					base::insert_after_element(Elem, *p2);
					return;
				}
				p1 = p2->next;
			}
		}
		void pop(){
			base::erase_after_element(Sentinel);
		}
		void swap(this_type& other){
			std::swap(Sentinel, other.Sentinel);
		}
		void clear(){
			element* p1 = &base::next(Sentinel);
			element* p2 = &Sentinel;
			while(true){
				//check with p1 == end?
				if(p1 == &Sentinel)break;
				p2 = &base::next(*p1);
				base::clear_element(*p1);

				//check with p2 == end?
				if(p2 == &Sentinel)break;
				p1 = &base::next(*p2);
				base::clear_element(*p2);
			}
			base::selfing(Sentinel);
			return;
		}
	};
	template<typename T, typename compare_>
	sorted_chain<T, compare_>::iterator::operator typename xc::sorted_chain<T, compare_>::const_iterator(){
		return xc::sorted_chain<T>::const_iterator(current());
	}
}
#
#endif
