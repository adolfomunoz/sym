#ifndef _VISITOR_H_
#define _VISITOR_H_

#include <utility>
#include <memory>

template<typename T, typename... Types>
struct VisitorTypeList {
	using FirstType = T;
};

template<typename T, typename U>
struct IsInTypeList {
	const static bool value = false;
};

template<typename T, typename U>
struct IsInTypeList<T,VisitorTypeList<U>> {
	const static bool value = false;
};

template<typename T, typename... Types>
struct IsInTypeList<T,VisitorTypeList<T, Types...>> {
	const static bool value = true;
};

template<typename T, typename U, typename... Types>
struct IsInTypeList<T,VisitorTypeList<U, Types...>> {
	const static bool value = IsInTypeList<T,VisitorTypeList<Types...>>::value; 
};




// Visitor template declaration
template<typename... Types>
class Visitor;

// specialization for single type    
template<typename T>
class Visitor<T> {
public:
    using TypeList = VisitorTypeList<T>;

    virtual void visit(T & visitable) = 0;
};

// specialization for multiple types
template<typename T, typename... Types>
class Visitor<T, Types...> : public Visitor<Types...> {
public:
    // promote the function(s) from the base class
    using Visitor<Types...>::visit;
    using TypeList = VisitorTypeList<T, Types...>;

    virtual void visit(T & visitable) = 0;
};





//VisitorApply template declaration, for applying generic functions throughout the visitor hierachy
template<typename... Types>
class VisitorApply;

//Specialization for a single type
template<typename F, typename Vis, typename T>
class VisitorApply<F, Vis, T> : public Vis  {
protected:
	F& f;
public:
	VisitorApply(F& _f) : f(_f) { }
	void visit(T& visitable) override { f(visitable); }
	using Vis::visit;
};

//Specialization for multiple types
template<typename F, typename Vis, typename T, typename... Types>
class VisitorApply<F, Vis, T, Types...> : public VisitorApply<F, Vis, Types...> 
{
protected:
	using VisitorApply<F, Vis, Types...>::f;
public:
	using VisitorApply<F, Vis, Types...>::VisitorApply;
	void visit(T & visitable) override { f(visitable); }
	using VisitorApply<F, Vis, Types...>::visit;
};

//Specialization for a TypeList
template<typename F, typename Vis, typename T, typename... Types>
class VisitorApply<F, Vis, VisitorTypeList<T, Types...>> : public VisitorApply<F, Vis, T, Types...> {
public:
	using VisitorApply<F, Vis, T, Types...>::VisitorApply;
};

//Specialization for no type so it expands to the visitor type list
template<typename F, typename Vis>
class VisitorApply<F, Vis> : public VisitorApply<F, Vis, typename Vis::TypeList> {
public:
	using VisitorApply<F, Vis, typename Vis::TypeList>::VisitorApply;
};


/**
 * This class helps applying different functions to the visitable with different return types.
 */
template<typename F, typename ReturnType>
class ApplyWithReturnType {
	F& f;
	ReturnType ret;
public:
	const ReturnType& returned() const { return ret; }
	ApplyWithReturnType(F& _f) : f(_f) { }
	template<typename P>
	void operator()(P& p) { ret = f(p); }	
};

/**
 * Specialization for void return type
 */
template<typename F>
class ApplyWithReturnType<F, void> {
	F& f;
public:
	void returned() const { return; }
	ApplyWithReturnType(F& _f) : f(_f) { }
	template<typename P>
	void operator()(P& p) { f(p); }
};

/**
 * This class helps applying different functions to the visitable with different return types.
 */
template<typename F, typename ReturnType>
class ApplyWithReturnTypeConst {
	const F& f;
	ReturnType ret;
public:
	const ReturnType& returned() const { return ret; }
	ApplyWithReturnTypeConst(const F& _f) : f(_f) { }
	template<typename P>
	void operator()(P& p) { ret = f(p); }	
};

/**
 * Specialization for void return type
 */
template<typename F>
class ApplyWithReturnTypeConst<F, void> {
	const F& f;
public:
	void returned() const { return; }
	ApplyWithReturnTypeConst(const F& _f) : f(_f) { }
	template<typename P>
	void operator()(P& p) { f(p); }
};


template<bool contains = false>
struct VisitIfContained {
	template<typename Visitor, typename Visitable>
	static void do_visit(Visitable& visitable, Visitor& visitor) { }
};

template<>
struct VisitIfContained<true> {
	template<typename Visitor, typename Visitable>
	static void do_visit(Visitable& visitable, Visitor& visitor) { visitor.visit(visitable); }
};

template<typename T, typename Vis>
class VisitableBase {
public:
    using Visitor = Vis;
    //Does nothing, just in case there are non visitable objects in the hierachy
    virtual void accept(Visitor& visitor) 
    {	VisitIfContained<IsInTypeList<T, typename Vis::TypeList>::value>::do_visit(static_cast<T&>(*this),visitor); }

    //Applies a generic void function to the visitable hierachy (probably one of its subclasses)
    template<typename F>
    auto apply(F& f) {
	ApplyWithReturnType<F, decltype(f(std::declval<typename Vis::TypeList::FirstType&>()))> fi(f);
	VisitorApply<decltype(fi), Vis> visitor(fi);
	this->accept(visitor);
	return fi.returned();
    }

    //Applies a generic void function to the visitable hierachy (probably one of its subclasses)
    template<typename F>
    auto apply(const F& f) {
	ApplyWithReturnTypeConst<F, decltype(f(std::declval<typename Vis::TypeList::FirstType&>()))> fi(f);
	VisitorApply<decltype(fi), Vis> visitor(fi);
	this->accept(visitor);
	return fi.returned();
    }

    //Const version of the above
    template<typename F>
    auto apply(F& f) const {
	ApplyWithReturnType<F, decltype(f(std::declval<const typename Vis::TypeList::FirstType&>()))> fi(f);
	VisitorApply<decltype(fi), Vis> visitor(fi);
	const_cast<VisitableBase<T, Vis>*>(this)->accept(visitor);
	return fi.returned();
    }

    //Applies a generic void function to the visitable hierachy (probably one of its subclasses)
    template<typename F>
    auto apply(const F& f) const {
	ApplyWithReturnTypeConst<F, decltype(f(std::declval<const typename Vis::TypeList::FirstType&>()))> fi(f);
	VisitorApply<decltype(fi), Vis> visitor(fi);
	const_cast<VisitableBase<T,Vis>*>(this)->accept(visitor);
	return fi.returned();
    }

    std::shared_ptr<T> clone_to_shared_ptr() const {
	return this->apply([] (const auto& e) { return std::shared_ptr<T>(e.clone_to_shared_ptr()); });
    }
};


template<typename Derived, typename Base>
class VisitableDerived : public Base {
public:
    //We inherit the constructor just in case we want to inherit it again in "Derived"
    using Base::Base;

    virtual void accept(typename Base::Visitor& visitor) {
        visitor.visit(static_cast<Derived&>(*this));
    }

    template<typename F>
    auto apply(F& f)       { return f(static_cast<Derived&>(*this)); }

    template<typename F>
    auto apply(const F& f) { return f(static_cast<Derived&>(*this)); }

    template<typename F>
    auto apply(F& f) const { return f(static_cast<const Derived&>(*this)); }

    template<typename F>
    auto apply(const F& f) const { return f(static_cast<const Derived&>(*this)); }

    std::shared_ptr<Derived> clone_to_shared_ptr() const {
		return std::make_shared<Derived>(static_cast<const Derived&>(*this));
    }

};

// Helper type for apply
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;


#endif
