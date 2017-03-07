
#pragma once

#include <functional>
#include <unordered_map>
#include <memory>
#include <stdexcept>


template<typename BaseClass>
using  Ctor= std::function<std::unique_ptr<BaseClass>()> ;


/**
 * Templated Factory class. 
 * 
 * @KeyType - Template parameter -  key type.
 * @BaseClass - Template parameter -  base class(usually abstract interface, but could be any virtual class).
 * @KeyHash - Template parameter -  hash functor for KeyType. If KeyType has standard hash function(int, std::string, etc.) - 
 * could be skipped.
 */
template<typename KeyType,typename BaseClass,typename KeyHash=std::hash<KeyType>>
class TFactory
{
public:
	/**
	* Registers class constructor.
	* if pair key-constructor already exists, old constructor will be overwritten with the new one.
	* @DerivedClass - Template parameter - class derived from \BaseClass .
	* @key -  key under which class constructor will be saved.
	* @args - any number of arguments required to pass to \DerivedClass constructor.
	*/
	template <typename DerivedClass, typename ... ArgTypes>
	static void registerClass(KeyType key,ArgTypes&& ... args)
	{
		//args are captured by value.Reason: no guarantee that referenced variable
		//will stay alive at instantiation moment.
		registeredClasses[key]= [args ...]() {return std::make_unique<DerivedClass>(args ...);};
	}
	/**
	* Checks if key was already used for class constructor registration.
	* @key - to look for.
	*/
	static bool isRegistered(KeyType key)
	{
		 return   registeredClasses.find(key)!=std::end(registeredClasses);
	}
	/**
	* Returns unique_ptr to class instance. 
	* @key -  to look for class constructor.
	*/
	static std::unique_ptr<BaseClass> getInstance(KeyType key)
	{
		if( !isRegistered(key)) throw std::out_of_range("TFactory: Key not found.");
		return registeredClasses[key]();
	}
private:
	static std::unordered_map <KeyType, Ctor<BaseClass>,KeyHash> registeredClasses;
};

template<typename KeyType,typename BaseClass,typename KeyHash>
std::unordered_map <KeyType,Ctor<BaseClass>,KeyHash> TFactory<KeyType,BaseClass,KeyHash>::registeredClasses {};
