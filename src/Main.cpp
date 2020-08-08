/*
Project: CuckyEd

File: src/Main.cpp
Purpose: Entry point

Authors: Regan "cuckydev" Green
*/

//Standard library
#include <iostream>

//CuckyEd classes
#include "Project.h"

int main(int argc, char *argv[])
{
	//Check for project path
	if (argc < 2)
	{
		std::cout << "No project given (TODO: Make open menu thing)" << std::endl;
		return 1;
	}
	
	//Open project (temp)
	CuckyEd::Project::Info::Instance project_info(argv[1]);
	if (project_info.GetError())
	{
		std::cout << project_info.GetError() << std::endl;
		return 1;
	}
	return 0;
}
