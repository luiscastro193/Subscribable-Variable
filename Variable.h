#ifndef VARIABLE_H
#define VARIABLE_H

#include <boost/signals2/signal.hpp>

template< class T > 
class Variable{
private:
	T value;
	boost::signals2::signal<void (const T &)> subscribers;
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
	inline void subscribe(const boost::function1<void, const T &> & function){
		subscribers.connect(function);
	}
	inline void desubscribe(const boost::function1<void, const T &> & function){
		subscribers.disconnect(function);
	}
	void signal(){
		if( pending_loop )
			repeat_loop = true;
		else{
			do{
				repeat_loop = false;
				pending_loop = true;
				subscribers(value);
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
			signal();
		}
		return value;
	}
	T operator = (const T & new_value){
		if( value != new_value ){
			value = new_value;
			signal();
		}
		return value;
	}
};

#endif
