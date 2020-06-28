#pragma once
#include <cstddef>
#include <vector>
#include <iostream>
#include <fstream>  
#include <filesystem>
#include <string>

class FileHandler
{
public:

	static void SaveToFile(std::string const data, std::string const filename)
	{
		std::string const path = Path + "/../saved/" + filename;
		//std::cout << "Saving to file " << path << "... ";

		std::filesystem::create_directory((std::filesystem::current_path().string() + "\\data").c_str());
		if (std::filesystem::exists((std::filesystem::current_path().string() + "\\data").c_str()))
		{
			std::ofstream f(path, std::ios::binary);
			f << data;
			f.close();
			//std::cout << "done" << std::endl;
		}
		else
		{
			//std::cout << "could not create folder" << std::endl;
		}
	}
	template<typename dataType>
	static std::vector<dataType> LoadFromFile(std::string const& filename)
	{
		std::string path = Path + "/../saved/" + filename;
		//std::cout << "Loading file " << path << "... ";

		std::ifstream ifs(path, std::ios::binary | std::ios::ate);

		if (!ifs) {
			//std::cout << "Could not open file!" << std::endl;
			return {};
		}

		auto end = ifs.tellg();
		ifs.seekg(0, std::ios::beg);

		auto size = std::size_t(end - ifs.tellg());

		if (size == 0) {
			//std::cout << "File size is 0?!" << std::endl;
			return {};
		}

		std::vector<dataType> buffer(size / sizeof(dataType));

		if (!ifs.read(reinterpret_cast<char*>(&buffer.front()), buffer.size() * sizeof(dataType))) {
			//std::cout << "Could not load data!" << std::endl;
			return {};
		}
		ifs.close();
		std::cout << "done" << std::endl;
		return buffer;
	}
	template<typename dataType>
	static std::vector<std::vector<dataType>> LoadFromFile2D(std::string const& filename)
	{
		std::string path = Path + "/../saved/" + filename;
		//std::cout << "Loading file " << path << "... ";

		std::ifstream ifs(path, std::ios::binary | std::ios::ate);

		if (!ifs) {
			//std::cout << "Could not open file!" << std::endl;
			return {};
		}

		auto end = ifs.tellg();
		ifs.seekg(0, std::ios::beg);

		auto size = std::size_t(end - ifs.tellg());

		if (size == 0) {
			//std::cout << "File size is 0?!" << std::endl;
			return {};
		}

		std::vector<std::vector<dataType>> result;
		int dim1, dim2;
		ifs.read(reinterpret_cast<char*>(&dim1), sizeof(dim1));
		ifs.read(reinterpret_cast<char*>(&dim2), sizeof(dim2));

		for (int i = 0; i < dim1; i++)
		{
			std::vector<dataType> buffer(dim2);
			if (!ifs.read(reinterpret_cast<char*>(&buffer.front()), buffer.size() * sizeof(dataType))) {
				//std::cout << "Could not load data!" << std::endl;
				return {};
			}
			else
			{
				result.push_back(buffer);
			}
		}
		ifs.close();
		//std::cout << "done" << std::endl;
		return result;
	}
	template<typename dataType>
	static void SaveToFile(std::vector<dataType> data, std::string filename)
	{
		std::string path = Path + "/../saved/" + filename;
		//std::cout << "Saving to file " << path << "... ";

		std::filesystem::create_directory((std::filesystem::current_path().string() + "\\data").c_str());
		if (std::filesystem::exists((std::filesystem::current_path().string() + "\\data").c_str()))
		{
			std::ofstream f(path, std::ios::binary);
			f.write(reinterpret_cast<char*>(data.data()), data.size() * sizeof(dataType));
			f.close();
			//std::cout << "done" << std::endl;
		}
		else
		{
			//std::cout << "could not create folder" << std::endl;
		}
	}
	template<typename dataType>
	static void SaveToFile(std::vector<std::vector<dataType>> data, std::string filename)
	{
		std::string path = Path + "/../saved/" + filename;
		//std::cout << "Saving to file " << path << "... ";
		std::ofstream f(path, std::ios::binary);
		int dim1 = data.size();
		int dim2 = data[0].size();
		f.write(reinterpret_cast<char*>(&dim1), sizeof(int));
		f.write(reinterpret_cast<char*>(&dim2), sizeof(int));
		for(int i = 0; i < dim1; ++i)
			f.write(reinterpret_cast<char*>(data[i].data()), dim2 * sizeof(dataType));
		f.close();
		//std::cout << "done" << std::endl;
	}
	static bool CheckFileExistence(std::string filename)
	{
		std::string path = Path + "/../saved/" + filename;
		//std::cout << "Checking if file " << path << " exists... ";

		std::ifstream ifs(path, std::ios::binary | std::ios::ate);

		if (!ifs) {
			//std::cout << "not found" << std::endl;
			return false;
		}
		//std::cout << "found" << std::endl;
		return true;
	}
};