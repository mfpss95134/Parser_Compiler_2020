#include<iostream>
#include<sstream>
#include<fstream>
#include<vector>
#include<set>
#include<algorithm>
using namespace std;

class Production
{
public:
	bool branch=false;
	int number=99;
	string LHS="";
	vector<string> RHS;
	
	void print()
	{
		cout << number << " " << LHS << "->";
		for(int i=0; i<RHS.size(); i++)
		{
			cout << RHS[i] << " ";
		}
		cout << endl;
		/*
		for(set<string>::iterator it=RHS.begin(); it!=RHS.end(); it++)
		{
			cout << *it << " ";
		}
		cout << endl;
		*/
	}
};
