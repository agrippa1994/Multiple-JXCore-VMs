#pragma once
#include "dispatch_queue.h"
#include "jx.h"

#include <thread>
#include <map>
#include <memory>
#include <string>
#include <istream>
#include <fstream>
#include <streambuf>

class VM {
	dispatch_queue queue_;
	static std::map<std::thread::id, std::shared_ptr<class VM>> virtualMachines_;
public:
	std::thread::id id_;

public:
	template<typename ...T>
	static std::shared_ptr<VM> createVM(T&&... args);

	static bool hasVM();
	static std::shared_ptr<VM> currentVM();
	static std::shared_ptr<VM> deleteVM();
	static const decltype(virtualMachines_)& virtualMachines();

private:
	VM(const std::string& content, bool isFile = false);

	VM(VM const&) = delete;
	VM& operator=(VM const&) = delete;

private:
	template<class RetType, class T, class = std::enable_if<!std::is_same<void, RetType>::value>::type, class ...P>
	RetType executeWithReturn(T fnc, P&&... args);

	template<class RetType, class T, class = std::enable_if<std::is_same<void, RetType>::value>::type, class ...P>
	void executeWithReturn(T fnc, P&&... args);

public:
	template<class T, class ...P>
	typename std::result_of<T(P...)>::type operator()(T func, P&&... p);

	template<class T, class ...P>
	typename std::result_of<T(P...)>::type exec(T func, P&&... p);

};

#include "VM_Impl.hpp"