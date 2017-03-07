
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
	Derived1(int num=10):num(num){}
	void doSomething() override {std::cout<<"num="<<num<<"\n";}
private:
	int num;
};


class Derived2:public Base{
public:
	Derived2(int num=8):num(num),str("lonestar"){}
	Derived2(std::string str):num(0),str(str){}
	void doSomething() override {std::cout<<"num="<<num<<", str="<<str<<"\n";}
protected:
	int num;
	std::string str;

};


class Derived3:public Derived2{
private:
	bool isItMe;
public:
	Derived3(int num=33,bool isItYou=true):Derived2(num),isItMe(isItYou){}
	void doSomething() override {std::cout<<"num="<<num<<" ,isItMe="<<std::boolalpha<<isItMe<<"\n";}
};

class Unattached{
public:
	void doSomething()  {std::cout<<"i'm unattached\n";}
};


enum class  en {Derived1Default,Derived1Param1,Derived2Default,Derived2Param1,Derived2Param2,Derived3Default,Derived3Param2,uat};



template <typename T>
struct EnumClassHash
{
	std::size_t operator()(T t) const
	{
		return static_cast<std::size_t>(t);
	}
};


int main()
{
	std::string allstars {"allstars"};
	int number=256;

	TFactory<std::string,Base> testfactoryWStr;
	testfactoryWStr.registerClass<Derived1>("Derived1Default");
	testfactoryWStr.registerClass<Derived1>("Derived1Param1",11);
	testfactoryWStr.registerClass<Derived2>("Derived2Default");
	testfactoryWStr.registerClass<Derived2>("Derived2Param1", number);
	testfactoryWStr.registerClass<Derived2>("Derived2Param2", allstars);

	std::cout<<"test with string key:\n";
	testfactoryWStr.getInstance("Derived1Default")->doSomething();
	testfactoryWStr.getInstance("Derived1Param1")->doSomething();
	testfactoryWStr.getInstance("Derived2Default")->doSomething();
	testfactoryWStr.getInstance("Derived2Param1")->doSomething();
	testfactoryWStr.getInstance("Derived2Param2")->doSomething();
	try
	{
		testfactoryWStr.getInstance("Derivam2")->doSomething();//good - Throws exception
	}
	catch(std::exception& ex )
	{
		std::cout<<"\nexception: "<<ex.what()<<"\n";
	}

	int number2=1024;
	TFactory<en,Base,EnumClassHash<en>> testfactoryWEnum;
	testfactoryWEnum.registerClass<Derived1>(en::Derived1Default);
	testfactoryWEnum.registerClass<Derived2>(en::Derived2Param1,number2);
	testfactoryWEnum.registerClass<Derived3>(en::Derived3Default);	
	testfactoryWEnum.registerClass<Derived3>(en::Derived3Default,16);//overwrites previous constructor
	testfactoryWEnum.registerClass<Derived3>(en::Derived3Param2,18,false);
	//testfactoryWEnum.registerClass<Unattached>(en::uat);//excellent - failes at compilation

	std::cout<<"\ntest with enum key:\n";
	testfactoryWEnum.getInstance(en::Derived1Default)->doSomething();
	testfactoryWEnum.getInstance(en::Derived2Param1)->doSomething();
	testfactoryWEnum.getInstance(en::Derived3Default)->doSomething();
	testfactoryWEnum.getInstance(en::Derived3Param2)->doSomething();

}
