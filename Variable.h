#ifndef VARIABLE_H
#define VARIABLE_H

#include <boost/signals2/signal.hpp>

template< class T > 
class Variable{
private:
	T value;
	boost::signals2::signal<void (const T *)> subscribers;
public:
	inline Variable(){;}
	
	inline Variable(T initial_value){
		value = initial_value;
	}
	inline void subscribe(void function(const T *)){
		subscribers.connect(function);
	}
	inline void desubscribe(void function(const T *)){
		subscribers.disconnect(function);
	}
	inline void signal(){
		subscribers(&value);
	}
	inline T get() const{
		return value;
	}
	T operator = (const Variable<T> other){
		if( value != other.get() ){
			value = other.get();
			subscribers(&value);
		}
		return value;
	}
	T operator = (const T new_value){
		if( value != new_value ){
			value = new_value;
			subscribers(&value);
		}
		return value;
	}
};

#endif
