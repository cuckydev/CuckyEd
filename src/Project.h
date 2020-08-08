#pragma once

/*
Project: CuckyEd

File: src/Project.h
Purpose: Declare project classes

Authors: Regan "cuckydev" Green
*/

//CuckyEd classes
#include "Error.h"

//CuckyEd namespace
namespace CuckyEd
{
	//Project namespace
	namespace Project
	{
		//Info class
		class Info
		{
			private:
				//Error
				CuckyEd::Error error;
				
			public:
				//Constructors
				Info() {}
				Info(std::istream &stream) { Read(stream); }
				Info(const std::string &path) { Read(path); }
				
				//Destructor
				~Info();
				
				//Interface
				bool Read(std::istream &stream);
				bool Read(const std::string &path);
		};
	}
}
