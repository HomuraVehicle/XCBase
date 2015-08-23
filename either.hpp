#ifndef XC_EITHER_INC
#define XC_EITHER_INC 100
#
namespace xc{
	template<typename value_type_,typename alternate_type_>
	struct either{
	public:
		typedef value_type_ value_type;
		typedef alternate_type_ alternate_type;
		typedef either<value_type,alternate_type> my_type;
	private:
		value_type Value;
		alternate_type Alternate;
	public:
		either():Value(),Alternate(){}
		either(const my_type& My_):Value(My_.Value),Alternate(My_.Alternate){}
		my_type& operator=(const my_type& My_){
			Value=My_.Value;
			Alternate=My_.Alternate;
			return *this;
		}
		either(const value_type& Value_):Value(Value_),Alternate(){}
		my_type& operator=(const value_type& Value_){
			Value=Value_;
			return *this;
		}
		either(const alternate_type& Alternate_):Value(),Alternate(Alternate_){}
		my_type& operator=(const alternate_type& Alternate_){
			Alternate=Alternate_;
			return *this;
		}
	public:
		operator bool()const{return !static_cast<bool>(Alternate);}
		value_type& operator *(){ return Value; }
		value_type* operator->(){ return &Value; }
		const value_type& operator *()const{ return Value; }
		const value_type* operator->()const{ return &Value; }
		alternate_type alternate()const{return Alternate;}
	};
	template<typename alternate_type_>
	struct either<void, alternate_type_>{
		typedef alternate_type_ alternate_type;
		typedef either<void, alternate_type> my_type;
	private:
		alternate_type Alternate;
	public:
		either() :Alternate(){}
		either(const my_type& My_) : Alternate(My_.Alternate){}
		my_type& operator=(const my_type& My_){
			Alternate = My_.Alternate;
			return *this;
		}
		either(const alternate_type& Alternate_) :Alternate(Alternate_){}
		my_type& operator=(const alternate_type& Alternate_){
			Alternate = Alternate_;
			return *this;
		}
	public:
		operator bool()const{ return !static_cast<bool>(Alternate); }
		alternate_type alternate()const{ return Alternate; }
	};
}
#
#endif
