#pragma once

/*
Project: CuckyEd

File: src/Project.h
Purpose: Declare project classes

Authors: Regan "cuckydev" Green
*/

//Standard library
#include <istream>
#include <string>
#include <vector>
#include <map>

//CuckyEd classes
#include "Error.h"

//CuckyEd namespace
namespace CuckyEd
{
	//Project namespace
	namespace Project
	{
		//Info namespace
		namespace Info
		{
			//Info types
			class Value : public std::string
			{
				public:
					Value &operator=(const std::string &other) { assign(other); return *this; }
					Value &operator=(std::string &&other) { assign(other); return *this; }
					operator std::string() const { return std::string(begin(), end()); }
					operator bool() const { return !empty(); }
			};
			
			typedef std::map<std::string, Value> ValueMap;
			
			class Act
			{
				private:
					//Act name
					std::string name;
					
					//Value
					ValueMap values;
					
				public:
					//Constructor
					Act(const std::string &_name) : name(_name) {}
					
					//Interface
					const std::string &GetName() const { return name; }
					Value GetValue(const std::string &key);
					
					void Push(const std::string &key, const Value &value);
			};
			
			class Zone
			{
				private:
					//Zone name
					std::string name;
					
					//Values and acts
					ValueMap values;
					std::vector<Act> acts;
					
				public:
					//Constructor
					Zone(const std::string &_name) : name(_name) {}
					
					//Interface
					const std::string &GetName() const { return name; }
					Value GetValue(const std::string &act, const std::string &key);
					
					void PushAct(const std::string &act);
					void Push(const std::string &key, const Value &value);
			};
			
			//Info class
			class Instance
			{
				private:
					//Error
					CuckyEd::Error error;
					
					//Values and zones
					ValueMap values;
					std::vector<Zone> zones;
					
				public:
					//Constructors
					Instance() {}
					Instance(std::istream &stream) { Read(stream); }
					Instance(const std::string &path) { Read(path); }
					
					//Destructor
					~Instance();
					
					//Interface
					bool Read(std::istream &stream);
					bool Read(const std::string &path);
					
					Value GetValue(const std::string &zone, const std::string &act, const std::string &key);
					
					//Get error
					const CuckyEd::Error &GetError() const { return error; }
					
				private:
					//Value pushes
					void PushAct(const Value &name);
					void PushZone(const Value &name);
					void Push(const std::string &key, const Value &value);
			};
		}
	}
}
