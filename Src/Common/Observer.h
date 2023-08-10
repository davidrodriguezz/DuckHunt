#pragma once
#include <iostream>
#include <list>
#include <string>


template<typename T> 
class IObserver {

public:
	virtual ~IObserver() {};
	virtual void Update(const T) = 0;
};

template<typename T> 
class ISubject {
protected:
	std::list<IObserver<T>*> list_observer_;
public:
	virtual ~ISubject() {};
	virtual void Suscribe(IObserver<T>* observer) = 0;
	virtual void UnSuscribe(IObserver<T>* observer) = 0;
	virtual void Notify(T) = 0;
};
