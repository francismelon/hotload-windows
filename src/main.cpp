#include <iostream>
#include "TestModule.h"

int main()
{
	TestModule test;
	// Customize dll path to use, must to use '\\'
	std::string testModulePath = "\\dll\\test.dll";
	test.setModulePath(testModulePath);
	std::cout << "Load Module!\n";
	test.LoadModule();

	std::cout << "Add(5, 6): " << test.Add(5, 6) << std::endl;
	std::cout << "Sub(6, 5): " << test.Sub(6, 5) << std::endl;

    std::cout << "Release Module!\n";
	test.UnloadModule();
}
