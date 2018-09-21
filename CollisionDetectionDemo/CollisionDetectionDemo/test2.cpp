/*
*
*
*@author: Aver Jing
*@description：
*@date：
*
*
*/

#include <iostream>
#include <memory>

template<typename T1>
void print_test(std::shared_ptr<T1> t1) {
	std::cout << *t1;
}

int main(){
	auto p = std::make_shared<int>(10);
	print_test(p);
	
}	