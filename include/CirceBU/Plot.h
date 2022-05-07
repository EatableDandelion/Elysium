#pragma once

#include <string>
#include <vector>
#include <iostream>

namespace Circe
{
	enum LineType
	{
		LINE,
		STEPS
	};
	
	class Series2d
	{
		public:
			Series2d(const std::string& fileName, const std::string& seriesName, const int& xIndex, const int& yIndex, const bool& visible, const LineType& type = LineType::LINE);
		
			std::string getSeriesName() const;
			std::string getFileName() const;
			int getXIndex() const;
			int getYIndex() const;
			bool isVisible() const;
			LineType getType() const;
			
			void setSeriesName(const std::string& newName);
			void setVisible(const bool& isVisible);
		
		private:
			std::string fileName;
			std::string seriesName;
			int xIndex;
			int yIndex;
			bool visible;
			LineType type;
	};
	
	class Plot2d
	{
		public:
			Plot2d(const std::string& title);
			~Plot2d();	
			
			void setTitle(const std::string& title);
			void setXAxisLabel(const std::string& label);
			void setYAxisLabel(const std::string& label);
			Series2d addSeries(const std::string& fileName, const std::string& seriesName, const int& xIndex, const int& yIndex, const LineType& type = LineType::LINE);
			void display();

		private:
			FILE* pipe;
			std::string title;
			std::vector<Series2d> allSeries;
			void printCommand(const std::string& command);
	};
}