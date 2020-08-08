/*
Project: CuckyEd

File: src/Project.cpp
Purpose: Define project classes

Authors: Regan "cuckydev" Green
*/

//Declaration
#include "Project.h"

//Standard library
#include <iostream>
#include <fstream>
#include <sstream>
#include <stddef.h>

//CuckyEd namespace
namespace CuckyEd
{
	//Project namespace
	namespace Project
	{
		//Info namespace
		namespace Info
		{
			//Act class
			Value Act::GetValue(const std::string &key)
			{
				//Find the value with the given key
				Value result;
				auto val_pos = values.find(key);
				if (val_pos != values.end())
					result = val_pos->second;
				return result;
			}
			
			void Act::Push(const std::string &key, const Value &value)
			{
				values[key] = value;
			}
			
			//Zone class
			Value Zone::GetValue(const std::string &act, const std::string &key)
			{
				//If the requested act contains this key, return its value instead
				Value result;
				if (!act.empty())
				{
					for (auto &i : acts)
						if (i.GetName() == act && (result = i.GetValue(key)))
							return result;
				}
				
				//Find the value with the given key
				auto val_pos = values.find(key);
				if (val_pos != values.end())
					result = val_pos->second;
				return result;
			}
			
			void Zone::PushAct(const std::string &act)
			{
				//Push act
				acts.push_back(Act(act));
			}
			
			void Zone::Push(const std::string &key, const Value &value)
			{
				//If an act has been defined, push onto act
				size_t last;
				if ((last = acts.size()))
					acts[last - 1].Push(key, value);
				else
					values[key] = value;
			}
			
			//Info class
			//Destructor
			Instance::~Instance()
			{
				
			}
			
			//Value pushes
			void Instance::PushAct(const Value &name)
			{
				//If a zone has been defined, push act onto zone
				size_t last;
				if ((last = zones.size()))
					zones[last - 1].PushAct(name);
			}
			
			void Instance::PushZone(const Value &name)
			{
				//Push zone
				zones.push_back(Zone(name));
			}
			
			void Instance::Push(const std::string &key, const Value &value)
			{
				//If a zone has been defined, push onto zone
				size_t last;
				if ((last = zones.size()))
					zones[last - 1].Push(key, value);
				else
					values[key] = value;
			}
			
			//Interface
			static const std::string parse_comment = "#";
			static const std::string parse_delimiter = ":";
			static const std::string parse_whitespace = " \t[]";
			
			void CleanString(std::string &string)
			{
				string = string.substr(string.find_first_not_of(parse_whitespace));
				string = string.substr(0, string.find_last_not_of(parse_whitespace)+1);
			}
			
			bool Instance::Read(std::istream &stream)
			{
				//Read info file line by line
				std::string line, key;
				Value value;
				
				while (std::getline(stream, line))
				{
					//Cut off at comment if there is one
					line = line.substr(0, line.find_first_of(parse_comment));
					
					//Stop processing line if empty
					if (line.length() == 0 || line.find_first_not_of(parse_whitespace) == std::string::npos)
						continue;
					
					//Find the delimiter position
					size_t delim_pos = line.find_first_of(parse_delimiter);
					
					if (delim_pos != std::string::npos)
					{
						//Get the key and value then clean trailing and leading spaces
						key = line.substr(0, delim_pos);
						value = line.substr(delim_pos + 1);
						CleanString(key);
						CleanString(value);
						
						//Don't process if key or value are empty
						if (key.length() == 0 || value.length() == 0)
							continue;
						
						//Execute command if key matches a command name
						static const std::map<std::string, void (Instance::*)(const Value&)> cmd = {
							{"DefZone",	&Instance::PushZone},
							{"DefAct",	&Instance::PushAct},
						};
						
						auto cmd_pos = cmd.find(key);
						if (cmd_pos != cmd.end())
						{
							//Is a command, execute
							(this->*(cmd_pos->second))(value);
						}
						else
						{
							//Not a command, push key and value
							Push(key, value);
						}
					}
				}
				return false;
			}
			
			bool Instance::Read(const std::string &path)
			{
				std::ifstream stream(path);
				if (!stream)
				{
					error.Push("Failed to open project file (" + path + ")");
					return true;
				}
				return Read(stream);
			}
			
			Value Instance::GetValue(const std::string &zone, const std::string &act, const std::string &key)
			{
				//If the requested zone contains this key, return its value instead
				Value result;
				if (!zone.empty())
				{
					for (auto &i : zones)
						if (i.GetName() == zone && (result = i.GetValue(act, key)))
							return result;
				}
				
				//Find the value with the given key
				auto val_pos = values.find(key);
				if (val_pos != values.end())
					result = val_pos->second;
				return result;
			}
		}
	}
}
