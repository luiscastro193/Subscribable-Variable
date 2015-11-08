#ifndef VARIABLE_H
#define VARIABLE_H

#include "nano_signal_slot.hpp"

template< typename T > 
class Variable{
private:
	T value;
	Nano::Signal<void (const T &)> subscribers;
	bool pending_loop;
	bool repeat_loop;
public:
	inline Variable(){
		pending_loop = false;
		repeat_loop = false;
	}
	inline Variable(const Variable<T> & other){
		pending_loop = false;
		repeat_loop = false;
		value = other.get();
	}
	inline Variable(const T & initial_value){
		pending_loop = false;
		repeat_loop = false;
		value = initial_value;
	}
	template <void (* function_ptr) (const T &)>
	inline void subscribe(){
		subscribers.template connect<function_ptr>();
	}
	template <typename Foo, void (Foo::* member_ptr) (const T &)>
	inline void subscribe(Foo* instance){
		subscribers.template connect<Foo, member_ptr>(instance);
	}
	template <typename Foo, void (Foo::* member_ptr) (const T &) const>
	inline void subscribe(Foo* instance){
		subscribers.template connect<Foo, member_ptr>(instance);
	}
	template <void (* function_ptr) (const T &)>
	inline void desubscribe(){
		subscribers.template disconnect<function_ptr>();
	}
	template <typename Foo, void (Foo::* member_ptr) (const T &)>
	inline void desubscribe(Foo* instance){
		subscribers.template disconnect<Foo, member_ptr>(instance);
	}
	template <typename Foo, void (Foo::* member_ptr) (const T &) const>
	inline void desubscribe(Foo* instance){
		subscribers.template disconnect<Foo, member_ptr>(instance);
	}
	void send(){
		if( pending_loop )
			repeat_loop = true;
		else{
			do{
				repeat_loop = false;
				pending_loop = true;
				subscribers.emit(value);
				pending_loop = false;
			} while (repeat_loop);
		}
	}
	inline T get() const{
		return value;
	}
	T operator = (const Variable<T> & other){
		if( value != other.get() ){
			value = other.get();
			send();
		}
		return value;
	}
	T operator = (const T & new_value){
		if( value != new_value ){
			value = new_value;
			send();
		}
		return value;
	}
};

#endif
