#ifndef __STLEXT_H
#define __STLEXT_H

// Need to find the right place for this as well

#include <functional>

#if 0
template<class _Bfn>
class void_binder1st : 
public std::unary_function<typename _Bfn::second_argument_type, typename _Bfn::result_type> 
{
public:
	void_binder1st(const _Bfn& _X,
		const typename _Bfn::first_argument_type& _Y)
		: op(_X), value(_Y) {}
	void operator()(const typename _Bfn::second_argument_type& _X) const
	{
        op(value, _X); 
    }
protected:
	_Bfn op;
	_Bfn::first_argument_type value;
	};
#else
template <class _Operation>
class void_binder1st
  : public std::unary_function<typename _Operation::second_argument_type, void> {
protected:
  _Operation op;
  typename _Operation::first_argument_type value;
public:
  void_binder1st(const _Operation& __x,
            const typename _Operation::first_argument_type& __y)
	: op(__x), value(__y) {}
  void
  operator()(const typename _Operation::second_argument_type& __x) const {
    op(value, __x);
  }
};

#endif

template<class _Bfn, class _Ty> inline
void_binder1st<_Bfn> void_bind1st(const _Bfn& XX, const _Ty& YY)
{
    return (void_binder1st<_Bfn>(XX, _Bfn::first_argument_type(YY))); 
}

template<class _Ty, class AA>
class void_mem_fun1_t : public std::binary_function<_Ty*, AA, void>
{
private:
	void (_Ty::*_Ptr)(AA);

public:
	explicit void_mem_fun1_t(void (_Ty::*_Pm)(AA)) : _Ptr(_Pm) {}
	void operator()(_Ty *PP, AA _Arg) const
    {
        (PP->*_Ptr)(_Arg); 
    }
};

template<class _Ty, class AA> 
inline void_mem_fun1_t<_Ty, AA> void_mem_fun1(void (_Ty::*_Pm)(AA))
{
    return (void_mem_fun1_t<_Ty, AA>(_Pm)); 
}

/*--------------- function object AssignOP ----------------*/
//
// Assign value to an iterator

template<class ITERATOR, class T>
struct AssignOP_t : public std::unary_function<T, void>
{
    mutable ITERATOR it;

    AssignOP_t(ITERATOR IT) : it(IT) {}

    void operator() (const T& x) const
    {
//        if (x == bpnumber) DebugBreak(); // DEBUG ONLY
        // Is this a bug? shouldn't it be *it = x?
        *it = x;
    }
};

template<class T, class ITERATOR>
AssignOP_t<ITERATOR, T> AssignOp(ITERATOR it)
{
    return AssignOP_t<ITERATOR, T>(it);
}


#endif // __STLEXT_H
