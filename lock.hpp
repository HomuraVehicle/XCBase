#ifndef XCBASE_LOCK_INC
#define XCBASE_LOCK_INC 100
#
namespace xc {
	template<typename T>
	class lock_guard {
		T* ptr;
	private:
		lock_guard(const lock_guard&);
		const lock_guard& operator=(const lock_guard&);
	public:
		lock_guard(T& ref_) :ptr(&ref_) {
			if(ptr->lock())ptr = 0;
		}
		~lock_guard(){ if(owns_lock())ptr->unlock(); }
		bool owns_lock() const { return ptr != 0; }
		operator bool() const{ return owns_lock(); }
	};
	template<typename T>
	class unique_lock {
		T* ptr;
		bool IsLock;
	private:
		unique_lock(const unique_lock&);
		const unique_lock& operator=(const unique_lock&);
	public:
		unique_lock() :ptr(0), IsLock(false) {}
		unique_lock(T& ref_, bool defer_lock_ = false) :ptr(&ref_), IsLock(false) {
			if(!defer_lock_)IsLock = !(ptr->lock());
		}
		~unique_lock(){ reset(); }
		bool owns_lock() const { return IsLock; }
		operator bool() const{ return owns_lock(); }
		void reset() {
			if(owns_lock()) {
				ptr->unlock();
				IsLock = false;
			}
			ptr = 0;
		}
		void reset(T& ref_, bool defer_lock_ = false) {
			reset();
			ptr = &ref_;
			if(!defer_lock_)IsLock = ptr->lock();
		}
		bool lock() {
			if(IsLock)return false;
			if(ptr->lock())return true;
			IsLock = true;
			return false;
		}
		void unlock() {
			if(!IsLock)return;
			ptr->unlock();
			IsLock = false;
		}
	};
	template<typename T1>
	bool lock(T1& t1) {
		return t1.lock();
	}
	template<typename T1, typename T2>
	bool lock(T1& t1, T2& t2) {
		if(t1.lock())return true;

		if(t2.lock()) {
			t1.unlock();
			return true;
		}
		return false;
	}
	template<typename T1, typename T2, typename T3>
	bool lock(T1& t1, T2& t2, T3& t3) {
		if(t1.lock())return true;

		if(lock(t2, t3)) {
			t1.unlock();
			return true;
		}
		return false;
	}
	template<typename T1, typename T2, typename T3, typename T4>
	bool lock(T1& t1, T2& t2, T3& t3, T4& t4) {
		if(t1.lock())return true;

		if(lock(t2, t3, t4)) {
			t1.unlock();
			return true;
		}
		return false;
	}
	template<typename T1, typename T2, typename T3, typename T4, typename T5>
	bool lock(T1& t1, T2& t2, T3& t3, T4& t4, T5& t5) {
		if(t1.lock())return true;

		if(lock(t2, t3, t4, t5)) {
			t1.unlock();
			return true;
		}
		return false;
	}
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
	bool lock(T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6) {
		if(t1.lock())return true;

		if(lock(t2, t3, t4, t5, t6)) {
			t1.unlock();
			return true;
		}
		return false;
	}
}
#
#endif
