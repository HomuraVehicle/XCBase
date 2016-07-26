#ifndef XCBASE_BITSET_INC
#define XCBASE_BITSET_INC 100
#
namespace xc{
	template<unsigned int size_, bool overflow_ = (size_>32)>
	struct bitset{
		typedef bitset<size_, false> this_bitset;
	private:
		unsigned int Bits;
		struct reference{
		private:
			unsigned int& Bits;
			unsigned int Acs;
		public:
			reference(unsigned int& Bits_, unsigned int Acs_)
				:Bits(Bits_)
				, Acs(Acs_){
			}
			reference& operator=(bool val){
				if(val)Bits = ((Bits&(~Acs)) | Acs);
				else Bits = (Bits&(~Acs));
				return *this;
			}
			operator bool(){ return (Bits&Acs)!=0; }
		};
	public:
		bitset():Bits(0){}
		bitset(unsigned int Bits_):Bits(Bits_){}
	public:
		reference operator[](unsigned int pos){
			return reference(Bits, (1 << pos));
		}
		void set(){ Bits = 0xffffffff; }
		void reset(){ Bits = 0; }
		void set(unsigned int pos, bool val = true){
			unsigned int Acs = (1 << pos);
			if(val)Bits = (Bits&(~Acs)) | Acs;
			else Bits = (Bits&(~Acs));
		}
		void reset(unsigned int pos){
			unsigned int Acs = (1 << pos);
			Bits = (Bits&(~Acs));
		}
		this_bitset& operator<<=(unsigned int num){
			Bits <<= num;
			return *this;
		}
		this_bitset& operator>>=(unsigned int num){
			Bits <<= (32 - size_);
			Bits >>= (32 - size_) + num;
			return *this;
		}
		this_bitset& operator&=(const this_bitset& Other){
			Bits &= Other.Bits;
			return *this;
		}
		this_bitset& operator|=(const this_bitset& Other){
			Bits |= Other.Bits;
			return *this;
		}
		this_bitset& operator^=(const this_bitset& Other){
			Bits ^= Other.Bits;
			return *this;
		}
		this_bitset& operator~(){
			Bits = ~Bits;
			return *this;
		}
		void flip(){
			operator~();
		}
		this_bitset operator<<(unsigned num){
			this_bitset Other(*this);
			Other <<= num;
			return Other;
		}
		this_bitset operator >> (unsigned num){
			this_bitset Other(*this);
			Other >>= num;
			return Other;
		}
		friend this_bitset operator&(const this_bitset& Bits1, const this_bitset& Bits2){
			this_bitset Other(Bits1);
			Other &= Bits2;
			return Other;
		}
		friend this_bitset operator|(const this_bitset& Bits1, const this_bitset& Bits2){
			this_bitset Other(Bits1);
			Other |= Bits2;
			return Other;
		}
		friend this_bitset operator^(const this_bitset& Bits1, const this_bitset& Bits2){
			this_bitset Other(Bits1);
			Other ^= Bits2;
			return Other;
		}
		friend bool operator==(const this_bitset& Bits1, const this_bitset& Bits2){
			return (Bits1.Bits << (32 - size_) == Bits2.Bits << (32 - size_));
		}
		friend bool operator!=(const this_bitset& Bits1, const this_bitset& Bits2){
			return (Bits1.Bits << (32 - size_) != Bits2.Bits << (32 - size_));
		}
	public:
		unsigned int count()const{
			unsigned int Cnt = 0;
			unsigned int CopyBits = Bits;
			for(unsigned int i = 0; i < size_; ++i){
				if(CopyBits & 1)++Cnt;
				CopyBits >>= 1;
			}
			return Cnt;
		}
		unsigned int size()const{ return size_; }
		bool test(unsigned int pos)const{
			return (Bits >> pos) & 1;
		}
		bool any()const{
			return static_cast<bool>(Bits << (32 - size_));
		}
		bool all()const{
			return !static_cast<bool>((~Bits) << (32 - size_));
		}
		bool none()const{
			return !any();
		}
	};
	template<unsigned int size_>
	struct bitset<size_, true>{};
}
#
#endif
