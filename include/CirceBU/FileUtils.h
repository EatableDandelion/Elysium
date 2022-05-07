#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

namespace Circe
{
	class FileReader
	{
		public:
			FileReader(const std::string& name);
			
			~FileReader();
			
			void read();
			
			std::vector<std::string> getLines() const;
			
			bool isEmpty();
			
			void clearFile();
			
			void close();
			
		private:
			std::ifstream file;
			const std::string fileName;
			std::vector<std::string> lines;
	};
	
	class KillSwitch // Verify if file is empty. If not, "isActivated" is true;
	{
		public:
			KillSwitch();
			
			~KillSwitch();
			
			bool isActivated();
			
		private:
			FileReader reader;
	};
	
	class DataFileWriter
	{
		public:
			DataFileWriter(const std::string& name, const char& delimiter);
			~DataFileWriter();
			
			template<typename... Args>
			void writeHeader(const Args... variables)
			{
				if(!file.is_open())
				{
					file.open(fileName);
				}

				file << addHeader(variables...) << std::endl;
			}
			
			template<typename... Args>
			void writeLine(const Args... variables)
			{
				if(!file.is_open())
				{
					file.open(fileName);
				}

				file << addLine(variables...) << std::endl;
			}
		
		private:
			const std::string fileName;
			std::ofstream file;
			char delimiter;
			
			template<typename T, typename... Args>
			std::string addLine(const T variable, const Args... variables)
			{
				return std::to_string(variable)+delimiter+addLine(variables...);
			}
			
			template<typename T>
			std::string addLine(const T variable)
			{
				return std::to_string(variable);
			}
			
			template<typename T, typename... Args>
			std::string addHeader(const T variable, const Args... variables)
			{
				return std::string(variable)+delimiter+addHeader(variables...);
			}
			
			template<typename T>
			std::string addHeader(const T variable)
			{
				return variable;
			}
	};
	
	
}
