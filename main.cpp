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
	cout << "---------- ѡ��С����  by Ma Xin ----------" << endl << endl;
	cout << "ʹ��ָ�ϣ� *.exe(��ǰ����) Stock.txt maxValue(����ʮ��ֲ��е�һλ�ֲ�Ȩ��)  minValue(���һλ�ֲ�Ȩ��)" << endl;
	cout << "ע�⣺��Stock.txt�� *.exe ��ִ�з���ͬһĿ¼��" << endl;
	cout << "ע�⣺Stock.txt�������ַ��������Щ�ַ���excel���ƶ�������Ĭ�ϸ�ʽΪUTF8�������ϡ������Ҫ�򿪸�txt�ļ�ѡ�����Ϊ��ѡ���ʽ��ANSI�������Ϊ�µ�Stock.txt�ļ���"
		<< endl << endl;   


	if (argc != 4)
	{
		cout << "Error: incorrect input." << endl;   
		cout << "Usage: *.exe Stock.txt maxValue minValue" << endl; 
		return -1;   
	}

	string filename = string(argv[1]); 
	// ��ǰ·��λ��.vcxproj�ļ���ͬĿ¼
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

	// �����п�ʼ�л�����Ϣ
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

	// map����ʹ��sort����ת��Ϊvector  
	vector< pair<string, int>> vGuAndCount(guAndCount.begin(), guAndCount.end()); 
	sort(vGuAndCount.begin(), vGuAndCount.end(), cmp);  // �Ӵ�С  
	cout << endl << "-------" << endl;    
	for (int i = 0; i < vGuAndCount.size(); i++)
		cout << "Count: " << vGuAndCount[i].second << ",  Stock name: " << vGuAndCount[i].first << endl; 

	// �������ƱȨֵ    
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

	// ����ļ� 
	//string out = filename.substr(0, filename.find("\\")); 
	string out = "./Stock_Score.txt";   
	ofstream outfile(out);
	if (!outfile.is_open())
		cerr << "outfile open failed." << endl;    

	outfile << "# ��Ʊ�����гֲ��г��ִ��� " << endl << endl;    
	for (int i = 0; i < vGuAndCount.size(); i++)
		outfile << "Count: " << vGuAndCount[i].second << ",  Stock name: " << vGuAndCount[i].first << endl;

	outfile << endl;   
	outfile << "# ��Ʊ�����гֲ��е÷�" << endl; 
	outfile << fixed; 
	for (int i = 0; i < vGuAndScore.size(); i++)
		outfile << setprecision(3) << "Score: " << vGuAndScore[i].second << ",  Stock name: " << vGuAndScore[i].first << endl;
	outfile.close();  

	cout << "Stock_Score.txt has been saved in " << out << endl; 

	return 0;   
}
