/*
Project: CuckyEd

File: src/Error.h
Purpose: Declare error class

Authors: Regan "cuckydev" Green
*/

//Standard library
#include <iostream>
#include <vector>
#include <string>

//CuckyEd namespace
namespace CuckyEd
{
	//Error class
	class Error
	{
		private:
			//Error list
			std::vector<std::string> error_list;
			
		public:
			//Error interface
			void Push(const Error &error) { error_list.push_back(error.ToString()); }
			void Push(const std::string &error) { error_list.push_back(error); }
			void Clear() { error_list.clear(); }
			std::string ToString() const;
			
			//Operators
			friend std::ostream& operator<<(std::ostream &out, const Error &x) { out << x.ToString(); return out; }
			operator bool() const { return error_list.size() > 0; }
			operator std::string() const { return ToString(); }
	};
}
