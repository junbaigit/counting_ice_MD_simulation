#include <iostream>
#include <windows.h>
#include <stdlib.h>
#include <vector>
#include <filesystem>
#include <fstream>
#include <sstream>

#include <iterator> 
#include <map> 


namespace fs = std::experimental::filesystem;
using namespace std;


/*
struct contains each molecular x y z position data
*/
struct Position {
	float x;
	float y;
	float z;

	Position()
	{
		
	}

	//caculate two molecular distance
	const float operator - (const Position& right) const
	{
		float result;

		result = sqrtf(pow((x - right.x),2) + pow((y - right.y), 2) + pow((z - right.z), 2));

		return result;
	}

};


vector<Position> list1 = vector<Position>();                     //list for previous molecular position
vector<Position> list2 = vector<Position>();                     //list for last molecular position
vector<fs::path> fileList = vector<fs::path>();                  //list for file path
vector<int> iceNumber = vector<int>();                           //number of ice 
vector<fs::path> fileList2 = vector<fs::path>();
map<int, fs::path> fList;

/*
remove space from string 
return clean string without white space
*/
string removeSpace(string s)
{
	s.erase(remove(s.begin(),s.end(),' '));
	return s;
}

/*
read directory and files
load molecular data into list
*/
void loadData()
{
	string path = "C:\\Users\\junym\\Desktop\\ice2\\i258";
	//string path = "\\ice_count";
	for (const auto & entry : fs::directory_iterator(path))
	{
		//std::cout << entry.path() << std::endl;

		fs::path Path = entry.path();

		string s = Path.u8string();
		cout << s << endl;
		string number = s.substr(37, 3);
		cout << number << endl;
		number.erase(remove(number.begin(), number.end(), ' '));
		number.erase(remove(number.begin(), number.end(), '.'));
		number.erase(remove(number.begin(), number.end(), 'g'));

		fList.insert(pair<int, fs::path>(stoi(number), Path));
		//cout << fileList.back() << endl;
	}

	int z = 0;
	int countIce = 0;
	//read 2 files at a time
	cout << "how many file: " << fList.size() << endl;
	for (int i = 0; i < fList.size()-1; i++)
	{
		z++;
		cout << "times of file processed: " << z << endl;

		ifstream file1(fList.at(i));
		ifstream file2(fList.at(i+1));

		

		string line1;
		string line2;

		Position previous;
		Position last;

		
		int flag = 0;
		//read first file
		
		while (file1 && getline(file1, line1))
		{
			
			flag++;
			//cout << line1.size() << endl;
			//cout << flag << endl;
			if (line1.length() == 44 && flag == 3)
			{
				string s1 = line1.substr(22,6);
				string s2 = line1.substr(30,6);
				string s3 = line1.substr(38,6);

				s1.erase(remove(s1.begin(), s1.end(), ' '));
				s2.erase(remove(s2.begin(), s2.end(), ' '));
				s3.erase(remove(s3.begin(), s3.end(), ' '));

				float x = stof(s1);
				float y = stof(s2);
				float z = stof(s3);

				previous.x = x;
				previous.y = y;
				previous.z = z;

				list1.push_back(previous);
				flag = -1;
				//cout << s1 << endl;			
			}
			
			
		}
		cout << "list 1 size : " << list1.size() << endl;
		int flag2 = 0;
		//read second file
		while (file2 && getline(file2, line2))
		{
			flag2++;
			
			if (line2.length() == 44 && flag2 == 3)
			{	
				

				string s1 = line2.substr(22, 6);
				string s2 = line2.substr(30, 6);
				string s3 = line2.substr(38, 6);

				//cout << s1 << s2<< s3 << endl;

				s1.erase(remove(s1.begin(), s1.end(), ' '));
				s2.erase(remove(s2.begin(), s2.end(), ' '));
				s3.erase(remove(s3.begin(), s3.end(), ' '));

				
		

				float x = stof(s1);
				float y = stof(s2);
				float z = stof(s3);

				last.x = x;
				last.y = y;
				last.z = z;

				list2.push_back(last);
				flag2 = -1;
				//cout << s1 << endl;			
			}
			
			
		}
		cout << "list 2 size : " << list2.size() << endl;

		//for (int i = 0; i < list2.size(); i++)
		//{
		//	cout << "x2 : " << list2.at(i).x << endl;
		//	cout << "y2 : " << list2.at(i).y << endl;
		//	cout << "z2 : " << list2.at(i).z << endl;
		//}

		for (int i = 0; i < list1.size(); i++)
		{
			//cout << "x1 : " << list2.at(i).x << endl;
			//cout << "y1 : " << list2.at(i).y << endl;
			//cout << "z1 : " << list2.at(i).z << endl;

			//cout << "x2 : " << list1.at(i).x << endl;
			//cout << "y2 : " << list1.at(i).y << endl;
			//cout << "z2 : " << list1.at(i).z << endl;

			float checkIce = list2.at(i) - list1.at(i);

			//cout << "after calcu" << checkIce << endl;
			//cout << checkIce << endl;
			if (checkIce <= 0.08)
			{
				countIce++;
			}
		}

		cout << countIce << endl;
		list1.clear();
		list2.clear();
		iceNumber.push_back(countIce);
		countIce = 0;
		cout << "ice list size : " << iceNumber.size() << endl;
	}
	
	
}

void writeData()
{
	ofstream myfile;
	myfile.open("i258.txt");
	for (int i = 0; i<iceNumber.size();i++)
	{
		myfile << iceNumber.at(i) << "\n";
	}
	
	myfile.close();
}

int main(int argc, char** argv)
{
	
	loadData();
	writeData();
}