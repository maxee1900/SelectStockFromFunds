#include <iostream>
#include <math.h>
#include <algorithm>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;   

bool cmp(const pair<string, int>& p1, const pair<string, int>& p2)
{
	return p1.second > p2.second;
}

bool cmp2(const pair<string, double>& p1, const pair<string, double>& p2)
{
	return p1.second > p2.second;   
}

// Terminal: maxValue minValue     

int main(int argc, char** argv)
{
	cout << "---------- 选股小程序  by Ma Xin ----------" << endl << endl;
	cout << "使用指南： *.exe(当前程序) Stock.txt maxValue(基金十大持仓中第一位持仓权重)  minValue(最后一位持仓权重)" << endl;
	cout << "注意：将Stock.txt与 *.exe 可执行放在同一目录下" << endl;
	cout << "注意：Stock.txt有中文字符，如果这些字符由excel复制而来，则默认格式为UTF8，不符合。因此需要打开该txt文件选择另存为，选择格式“ANSI”，另存为新的Stock.txt文件。"
		<< endl << endl;   


	if (argc != 4)
	{
		cout << "Error: incorrect input." << endl;   
		cout << "Usage: *.exe Stock.txt maxValue minValue" << endl; 
		return -1;   
	}

	string filename = string(argv[1]); 
	// 当前路径位于.vcxproj文件的同目录
	// string filename = ".\\Stock.txt";

	double maxValue = stod(argv[2]);
	double minValue = stod(argv[3]);
 
	ifstream infile;
	infile.open(filename); 
	if (!infile.is_open())
	{
        cerr << "Error: file open failed." << endl;
		return -1; 
	}

	string s; 
	getline(infile, s);
	getline(infile, s);

	// 第三行开始有基金信息
	vector<string> jiNames;       
	while (!infile.eof())
	{ 
		getline(infile, s); 
		if (s == "")
			break;  
		istringstream ss(s);
		int a;
		string name; 
		ss >> a >> name;
		jiNames.push_back(name);  
	}

	int num = jiNames.size();  
	cout << "totally find " << num << " funds." << endl;    

	vector<string> guNames;      
	while (!infile.eof())
	{
		getline(infile, s); 
		if (s == "")
			continue;    
		guNames.push_back(s);  
	}

	infile.close(); 

	if (jiNames.size() != guNames.size())
		cerr << "Error: different size." << endl; 

	map<string, vector<string>> jiAndCang;    
	for (int i = 0; i < jiNames.size(); i++)
	{
		string s = guNames[i]; 
		istringstream ss(s); 
		vector<string> cang; 
		for (int j = 0; j < 10; j++)
		{
			string guname; 
			ss >> guname; 
			cang.push_back(guname);  
		}

		string jiname = jiNames[i]; 
		jiAndCang.insert(pair<string, vector<string>>(jiname, cang));  
	}

	multiset<string> allGu_multi;
	set<string> allGu;   

	for (int i = 0; i < guNames.size(); i++)
	{
		string s = guNames[i]; 
		istringstream ss(s); 
		string name; 
		for (int a = 0; a < 10; a++)
		{
			ss >> name; 
			allGu.insert(name);
			allGu_multi.insert(name);  
		}
	}

	cout << "totally find " << allGu.size() << " stocks." << endl;   
	map<string, int> guAndCount;  
	map<string, double> guAndScore;    

	for (auto it = allGu.begin(); it != allGu.end(); it++)
	{
		string name = *it;   
		int cnt = allGu_multi.count(name); 
		guAndCount[name] = cnt;   
		guAndScore[name] = 0;   
	}

	// map不能使用sort排序，转换为vector  
	vector< pair<string, int>> vGuAndCount(guAndCount.begin(), guAndCount.end()); 
	sort(vGuAndCount.begin(), vGuAndCount.end(), cmp);  // 从大到小  
	cout << endl << "-------" << endl;    
	for (int i = 0; i < vGuAndCount.size(); i++)
		cout << "Count: " << vGuAndCount[i].second << ",  Stock name: " << vGuAndCount[i].first << endl; 

	// 计算各股票权值    
	double values[10]; 
	for (int i = 0; i < 10; i++)
	{
		values[i] = maxValue - (maxValue - minValue)*i / 9;   
	}

	for (auto it = jiAndCang.begin(); it != jiAndCang.end(); it++)
	{
		vector<string> vNames = it->second;
		if (vNames.size() != 10)
			cerr << "Error: incorrct cang size." << endl;   

		for (int i = 0; i < vNames.size(); i++)
		{
			string name = vNames[i]; 
			double value = values[i];   
			guAndScore[name] += value;   
		}
	}

	// sort   
	vector<pair<string, double> > vGuAndScore(guAndScore.begin(), guAndScore.end()); 
	sort(vGuAndScore.begin(), vGuAndScore.end(), cmp2);   
	cout << endl << "-------" << endl;
	cout << fixed; 
	for (int i = 0; i < vGuAndScore.size(); i++)
		cout << setprecision(3) << "Score: " << vGuAndScore[i].second << ",  Stock name: " << vGuAndScore[i].first << endl;

	// 输出文件 
	//string out = filename.substr(0, filename.find("\\")); 
	string out = "./Stock_Score.txt";   
	ofstream outfile(out);
	if (!outfile.is_open())
		cerr << "outfile open failed." << endl;    

	outfile << "# 股票在所有持仓中出现次数 " << endl << endl;    
	for (int i = 0; i < vGuAndCount.size(); i++)
		outfile << "Count: " << vGuAndCount[i].second << ",  Stock name: " << vGuAndCount[i].first << endl;

	outfile << endl;   
	outfile << "# 股票在所有持仓中得分" << endl; 
	outfile << fixed; 
	for (int i = 0; i < vGuAndScore.size(); i++)
		outfile << setprecision(3) << "Score: " << vGuAndScore[i].second << ",  Stock name: " << vGuAndScore[i].first << endl;
	outfile.close();  

	cout << "Stock_Score.txt has been saved in " << out << endl; 

	return 0;   
}
