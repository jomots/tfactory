# tfactory
  Extremely simple templated factory. It's contained in single tiny header file. Allowes to register factory classes with or without parameters.
  Usage example:
```c++
#include "tfactory.h"

#include <iostream>
#include <string>

class Base
{
public:
	virtual void doSomething()=0;
	virtual ~Base(){}
};

class Derived1:public Base{
public:
	Derived1(int num=64):num(num){}
	void doSomething() override {std::cout<<"num="<<num<<"\n";}
private:
	int num;
};

class Derived2:public Base{
public:	
	Derived2(std::string str="Lone Starr"):str(str){}
	void doSomething() override {std::cout<<"str="<<str<<"\n";}
private:	
	std::string str;
};

class Unattached{
public:
	void doSomething()  {std::cout<<"i'm unattached\n";}
};

int main() {

	std::string allstars {"Barf"};
	int number=256;

	TFactory<std::string,Base> testFactory;
	
	//With default constructors
	testFactory.registerClass<Derived1>("Derived1Default");
	testFactory.registerClass<Derived2>("Derived2Default");
	
	//With some initial params
	testFactory.registerClass<Derived1>("Derived1Param",number);	
	testFactory.registerClass<Derived2>("Derived2Param", allstars);
	
	//Fails at build:
	//testFactory.registerClass<Unattached>("Unattached");
	
	//Simple instantiation and use
	testFactory.getInstance("Derived1Default")->doSomething();
	testFactory.getInstance("Derived2Default")->doSomething();
	
	//Assign instance to unique_ptr
	auto uniquePtr= testFactory.getInstance("Derived1Param");
	uniquePtr->doSomething();
	
	//Assign instance to shared_ptr
	std::shared_ptr<Base> sharedPtr=testFactory.getInstance("Derived2Param");
	sharedPtr->doSomething();
	
	//Throws out of range exception
	try
	{
		testFactory.getInstance("Derivam2")->doSomething();
	}
	catch(std::exception& ex )
	{
		std::cout<<"exception: "<<ex.what()<<"\n";
	}

}
```
