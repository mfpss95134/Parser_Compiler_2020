#include<iostream>
#include<sstream>
#include<fstream>
#include<vector>
#include<set>
#include<algorithm>
#include <map>
#include <stack>
#include <queue>
#include "Production.h" 
using namespace std;
map<int, bool> derives_lambda;
map<int, set<string> > First_set;
map<string, set<string> > First_set2;
map<string, set<string> > Follow_set;
map<int, set<string> > Predict_set;
map<int, set<string> > Predict_set2;
vector<Production> Productions;
set<string> nonTerminals;
set<string> Terminals;
set<string> Symbols;
map<string, map<string, int> > Table;
string START_SYMBOL;
string tokStr;
stack<string> PARSE_STK;
queue<string> TS;
vector<string> orderedNT;




void FINAL_CLEAR()
{
	map<int, bool> empty_map1; derives_lambda.swap(empty_map1); derives_lambda.clear();
	map<int, set<string> > empty_map2; First_set.swap(empty_map2);  First_set.clear();
	map<string, set<string> > empty_map3; First_set2.swap(empty_map3); First_set2.clear();
	map<string, set<string> > empty_map4; Follow_set.swap(empty_map4); Follow_set.clear();
	map<int, set<string> > empty_map5; Predict_set.swap(empty_map5); Predict_set.clear();
	map<int, set<string> > empty_map6; Predict_set2.swap(empty_map6); Predict_set2.clear();
	vector<Production> empty_vec1; Productions.swap(empty_vec1); Productions.clear();
	nonTerminals.clear();
	Terminals.clear();
	Symbols.clear();
	map<string, map<string, int> > empty_map7; Table.swap(empty_map7); Table.clear();
	START_SYMBOL="";
	tokStr="";
	stack<string> PARSE_STK; while(!PARSE_STK.empty()) PARSE_STK.pop();
	queue<string> TS; while(!TS.empty()) TS.pop();
	vector<string> empty_vec2; orderedNT.swap(empty_vec2); orderedNT.clear();
	
	return;
}


void print_PARSE()
{
	stack<string> stk1=PARSE_STK;
	stack<string> stk2;
	while(!stk1.empty())
	{
		stk2.push(stk1.top());
		stk1.pop();
	}
	while(!stk2.empty())
	{
		cout << stk2.top();
		stk2.pop();
	}
	cout << "\t";
}

void print_TS()
{
	queue<string> ts=TS;
	
	while(!ts.empty())
	{
		cout << ts.front();
		ts.pop();
	}
	cout << endl;
}

bool MATCH()
{
	if(PARSE_STK.top()==TS.front())
		return true;
	else
		return false;
}

bool isTerminal(string tok)
{
	if(Terminals.count(tok) || islower(tok[0]) || tok=="$" || tok=="+" || tok=="(" || tok==")" || tok==";")
		return true;
	else 
		return false;
}

void mark_lambda() 
{
    bool 				rhs_derives_lambda;
    bool 				changes;
    Production 			p;
    
    derives_lambda.clear(); //Initially, nothing is marked. 
    for(int i = 0; i < Productions.size(); i++) 
	{
        derives_lambda[Productions[i].number] = false;
    }
    
    //do {
        changes = false;
        for(int i = 0; i < Productions.size(); i++) 
		{
            p = Productions[i];
            if(!derives_lambda[p.number]) 
			{
				//Mark true if a production derives only a lambda
                if(p.RHS.size() == 0 || p.RHS[0] == "L") 
				{
                    changes = true;
					derives_lambda[p.number] = true;
                    continue;
                }
                
                //Check whether each part of RHS derive lambda
                for(int j = 0; j < p.RHS.size(); j++) 
				{
					if(islower(p.RHS[j][0]) || p.RHS[j]=="$" || p.RHS[j]=="+" || p.RHS[j]=="(" || p.RHS[j]==")" || p.RHS[j]==";")
					{
						rhs_derives_lambda=false;
						break;
					}
					else
					{
						for(int k=0; k<Productions.size(); k++)
						{
							if(p.RHS[j] == Productions[k].LHS && Productions[k].RHS[0]=="L")
							{
								//if(Productions[k].RHS[0]=="L")
									rhs_derives_lambda=true;
							}
						}
					}
                }
                
                if(rhs_derives_lambda) 
				{
                    changes = true;
                    derives_lambda[p.number] = true;
                }
            }
        }
    //} while(changes);
    
    return;
}


set<string> FIRST(vector<string> RHS)
{
	set<string> result;
	
	if(Terminals.count(RHS[0]))  //N->aXXX 
	{
		result.insert(RHS[0]);
	}
	else if(RHS[0]=="L")  //LAMBDA
	{
		return result;
	}
	else  //N->AXXX
	{
		for(int i=0; i<RHS.size(); i++)
		{
			if(RHS[i]=="$" || RHS[i]=="+" || RHS[i]=="(" || RHS[i]==")" || RHS[i]==";")
			{
				result.insert(RHS[i]);
				continue;
			}
			
			for(int j=0; j<Productions.size(); j++)
			{
				if(RHS[i] == Productions[j].LHS)
				{
					set<string> tmp=FIRST(Productions[j].RHS);
					set_union(result.begin(),result.end(),tmp.begin(),tmp.end(),inserter(result,result.begin()));
				}
			}
		}
	}
	
	
	return result;
}


set<string> FIRST_(vector<string> vec)
{
	set<string> result;
	
	if(isTerminal(vec[0]))  //N->aXXX 
	{
		result.insert(vec[0]);
	}
	else  //N->AXXX
	{
		string nT=vec[0];
		set_union(result.begin(),result.end(),First_set2[nT].begin(),First_set2[nT].end(),inserter(result,result.begin()));
		
		for(int i=1; i<vec.size(); i++)
		{
			if(isTerminal(vec[i]))
			{
				result.insert(vec[i]);
			}
			else
			{
				string nT=vec[0];
				set_union(result.begin(),result.end(),First_set2[nT].begin(),First_set2[nT].end(),inserter(result,result.begin()));
			}
		}
	}
	
	return result;
}


set<string> FOLLOW(string LHS)
{
	set<string> result;
	
	//cout << "FOLLOW(" << LHS << ")" << endl;
	for(int i=0; i<Productions.size(); i++)
	{
		vector<string>::iterator it=find(Productions[i].RHS.begin(), Productions[i].RHS.end(), LHS);
		
		if(it!=Productions[i].RHS.end())  //FOUND
		{
			int j=distance(Productions[i].RHS.begin(), it);
			//cout << "FOUND: " << Productions[i].number << endl;
			
			
			vector<string> vec;
			for(int k=j+1; k<Productions[i].RHS.size(); k++)
			{
				vec.push_back(Productions[i].RHS[k]);
			}
			
			
			if(vec.size()==0)
			{
				if(LHS==Productions[i].LHS)
					continue;
				
				set<string> tmp=FOLLOW(Productions[i].LHS);
				set_union(result.begin(),result.end(),tmp.begin(),tmp.end(),inserter(result,result.begin()));
			}
			else
			{
				set<string> tmp=FIRST_(vec);
				set_union(result.begin(),result.end(),tmp.begin(),tmp.end(),inserter(result,result.begin()));
				
				bool drv_l=false;
				for(int k=0; k<vec.size(); k++)
				{
					if(isTerminal(vec[k]))
					{
						drv_l=false;
						break;
					}
					else
					{
						for(int l=0; l<Productions.size(); l++)
						{
							if(vec[k] == Productions[l].LHS && Productions[l].RHS[0]=="L")
							{
								drv_l=true;
							}
						}
					}
				}
				
				if(drv_l)
				{
					if(LHS==Productions[i].LHS)
						continue;
					
					set<string> tmp=FOLLOW(Productions[i].LHS);
					set_union(result.begin(),result.end(),tmp.begin(),tmp.end(),inserter(result,result.begin()));
				}
					
			}
		}
	}
	
	return result;
}


int main()
{
	//string fileName="CFG.txt";
	
	string fileName;
	cout << "FILE NAME OF CFG: ";
	cin >> fileName;
	ifstream in(fileName);
	string line;
	int numBer=0;
	
	
	
	//READ FILE
	while(getline(in, line))
	{
		//cout << line << endl;
		stringstream ss(line);
		string tmpStr;
		Production P;
		
		ss >> numBer; //get number
		ss >> tmpStr;
		if(tmpStr!="|") //NORMAL CONDICTION
		{
			nonTerminals.insert(tmpStr);
			P.branch=false;
			P.number=numBer;
			P.LHS=tmpStr;
			while(ss >> tmpStr)
			{
				if(tmpStr==">")
					ss >> tmpStr; //skip ">"
				
				P.RHS.push_back(tmpStr);
			}
		}
		else //BRANCH CONDICTION
		{
			P.branch=true;
			P.number=numBer;
			P.LHS=Productions.back().LHS;
			while(ss >> tmpStr)
			{			
				P.RHS.push_back(tmpStr);
			}
		}
		
		Productions.push_back(P);
	}
	in.close();
	
	
	//STORE SYMBOLS
	for(int i=0; i<Productions.size(); i++)
	{
		Productions[i].print();
		
		Symbols.insert(Productions[i].LHS);
		for(int j=0; j<Productions[i].RHS.size(); j++)
		{
			Symbols.insert(Productions[i].RHS[j]);
		}
	}
	
	
	cout << "\nSymbols :" << endl; 
	Symbols.erase("L");
	for(set<string>::iterator it=Symbols.begin(); it!=Symbols.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl << endl;
	
	
	cout << "Terminals :" << endl;
	set_difference(Symbols.begin(), Symbols.end(), nonTerminals.begin(), nonTerminals.end(), inserter(Terminals,Terminals.begin()));
	for(set<string>::iterator it=Terminals.begin(); it!=Terminals.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl << endl;
	
	
	cout << "Non_Terminals :" << endl; 
	for(set<string>::iterator it=nonTerminals.begin(); it!=nonTerminals.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl << endl;
	
	
	//MARK WHETHER A RULE DERIVES LAMBDA
	mark_lambda();
	cout << "Rules :" << endl;
	for(map<int, bool>::iterator it=derives_lambda.begin(); it!=derives_lambda.end(); it++)
	{
		cout << it->first << " ";
	}
	cout << endl << endl;
	
	
	cout << "Derives Empty? " << endl;
	for(int i=0; i<Productions.size(); i++)
	{
		cout << derives_lambda[Productions[i].number] << " ";
	}
	cout << endl << endl;
	
	
	cout << "First set :" << endl;
	for(int i=0; i<Productions.size(); i++)
	{
		First_set[i+1]=FIRST(Productions[i].RHS);
	}
	for(map<int, set<string> >::iterator it=First_set.begin(); it!=First_set.end(); it++) //for(int i=1; i<=Productions.size(); i++)
	{	
		cout << it->first << ":  ";
		for(set<string>::iterator it2=it->second.begin(); it2!=it->second.end(); it2++) //for(set<string>::iterator it2=First_set[i].begin(); it2!=First_set[i].end(); it2++)
		{
			cout << *it2 << " ";
		}
		cout << endl;
	}
	cout << endl;
	
	
	cout << "First set of NonTerminals :" << endl;
	for(int i=0; i<Productions.size(); i++)
	{
		int rule_num1=Productions[i].number;
		string LHS1=Productions[i].LHS;
		
		for(int j=0; j<Productions.size(); j++)
		{
			int rule_num2=Productions[j].number;
			string LHS2=Productions[j].LHS;
			
			if(LHS1==LHS2)
			{
				set_union(First_set[rule_num1].begin(),First_set[rule_num1].end(),First_set[rule_num2].begin(),First_set[rule_num2].end(),inserter(First_set2[LHS1],First_set2[LHS1].begin()));
			}
		}
	}
	for(set<string>::iterator it=nonTerminals.begin(); it!=nonTerminals.end(); it++)
	{
		string LHS=*it;
		
		cout << LHS << ":  ";
		for(set<string>::iterator it2=First_set2[LHS].begin(); it2!=First_set2[LHS].end(); it2++)
		{
			cout << *it2 << " ";
		}
		cout << endl;
	}
	cout << endl;
	
	
	cout << "Follow set of NonTerminals :" << endl;
	for(int i=0; i<Productions.size(); i++)
	{
		int rule_num=Productions[i].number;
		string LHS=Productions[i].LHS;
		
		if(derives_lambda[rule_num])
		{
			Follow_set[LHS]=FOLLOW(Productions[i].LHS);
		}
	}
	for(map<string, set<string> >::iterator it=Follow_set.begin(); it!=Follow_set.end(); it++)
	{
		string nT=it->first;
		
		cout << nT << ":  ";		
		for(set<string>::iterator it2=Follow_set[nT].begin(); it2!=Follow_set[nT].end(); it2++)
		{
			cout << *it2 << " ";
		}
		cout << endl;
	}
	cout << endl;
	
	
	cout << "Predict set of NonTerminals :" << endl;
	for(int i=0; i<Productions.size(); i++)
	{
		int rule_num=Productions[i].number;
		string LHS=Productions[i].LHS;
		
		if(derives_lambda[rule_num])
		{
			set_union(First_set[rule_num].begin(),First_set[rule_num].end(),Follow_set[LHS].begin(),Follow_set[LHS].end(),inserter(Predict_set[rule_num],Predict_set[rule_num].begin()));
		}
		else
		{
			 set_union(First_set[rule_num].begin(),First_set[rule_num].end(),Predict_set[rule_num].begin(),Predict_set[rule_num].end(),inserter(Predict_set[rule_num],Predict_set[rule_num].begin()));
		} 
	}
	for(int i=1; i<=Productions.size(); i++)
	{
		cout << i << ":  ";
		
		for(set<string>::iterator it=Predict_set[i].begin(); it!=Predict_set[i].end(); it++)
		{
			cout << *it << " ";
		}
		cout << endl;
	}
	cout << endl;
	
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	
	/* 
	//READ PREDICT SET
	ifstream in2("predict.txt");
	for(int numBer=1; getline(in2, line); numBer++)
	{
		stringstream ss(line);
		string tmpStr;
		
		while(ss >> tmpStr)
		{
			//cout << "read: ";
			//cout << tmpStr << " ";
			Predict_set[numBer].insert(tmpStr);
		}
		//cout << endl;
	}
	in2.close();
	
	
	//OUTPUT PREDICT SET 
	for(int i=0; i<Productions.size(); i++) 
	{
		for(set<string>::iterator it=Predict_set[i+1].begin(); it!=Predict_set[i+1].end(); it++)
		{
			cout << *it << " ";
		}
		cout << endl;
	}
	*/
	 
	
	//OUTPUT nonTerminals OF TABLE
	cout << "------------------------------------------------------------------\n" << "\t";
	int cnt=0;
	string str;
	for(set<string>::iterator it=Terminals.begin(); it!=Terminals.end(); it++)
	{
		if(*it != "$" && *it != "+" && *it != "(" && *it != ")" && *it != ";")
		{
			cout << *it << "\t";
			cnt++;
		}
		else
		{
			str=*it;
		}
		
		if(cnt==Terminals.size()-1)
			cout << str << "\t";
	}
	cout << endl;
	
	
	// Initialize entries of table to 0
	for(int i=0; i<Productions.size(); i++) // map<string, map<string, int> > Table;
	{
		string LHS=Productions[i].LHS;
		
		for(set<string>::iterator it=Terminals.begin(); it!=Terminals.end(); it++)
		{
			string terminal=*it;
			Table[LHS][terminal]=0;
		}
	}
	
	
	// Store the corresponding rule number into table
	for(int i=0; i<Productions.size(); i++) // map<string, map<string, int> > Table;
	{
		int numBer=Productions[i].number;
		string LHS=Productions[i].LHS;
		
		for(set<string>::iterator it=Predict_set[numBer].begin(); it!=Predict_set[numBer].end(); it++)
		{
			string terminal=*it;
			Table[LHS][terminal]=numBer;
		}
	}
	
	
	// Output the table
	for(int i=0; i<Productions.size(); i++)
	{
		string nonterminal = Productions[i].LHS;
		
		vector<string>::iterator it=find(orderedNT.begin(), orderedNT.end(), nonterminal);
		if(it==orderedNT.end())
			orderedNT.push_back(nonterminal);
	}
	for(int i=0; i<orderedNT.size(); i++)  // map<string, map<string, int> > Table;
	{
		string nonterminal = orderedNT[i];
		cout << nonterminal << "\t";
		
		int tmp=0;
		for(map<string, int>::iterator it=Table[nonterminal].begin(); it!=Table[nonterminal].end(); it++)
		{
			int rule_num = it->second;
			
			if(it->first != "$" && it->first != "+" && it->first != "(" && it->first != ")" && it->first != ";")
			{
				if(rule_num==0)
					cout << " \t";
				else
					cout << rule_num << "\t";
			}
			else
			{
				tmp=rule_num;
			}
		}
		if(tmp==0) cout << " \t"; else cout << tmp << "\t";
		cout << endl;
	}
	cout << "------------------------------------------------------------------\n\n";
	
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	
	
REDO:
	START_SYMBOL=Productions[0].LHS;
	PARSE_STK.push(START_SYMBOL); // PUSH S
	bool ACCEPT=false;
	cout << "TOKEN STRING: ";
	cin >> tokStr;
	//tokStr="abbdc$"; //abbdc$
	for(int i=0; i<tokStr.size(); i++) // Put token string into queue TS;
	{
		string tmpStr;
		tmpStr.push_back(tokStr[i]);
		TS.push(tmpStr);
	}
	
	
	// LL_PARSING
	while(!ACCEPT)
	{
		if(isTerminal(PARSE_STK.top()))
		{
			if(MATCH())
			{
				print_PARSE(); print_TS();
				cout << "  MATCH" << endl;
				
				PARSE_STK.pop();
				TS.pop();
				if(PARSE_STK.top()=="$" && TS.front()=="$")
					ACCEPT=true;
			}
			else
			{
				print_PARSE(); print_TS();
				cout << "  SYNTAX ERROR (EXPECTED " << PARSE_STK.top() << ")" << endl;
				break;
			}
		}	
		else
		{
			string TOS=PARSE_STK.top();
			string PEEK=TS.front();
			int pnum=Table[TOS][PEEK];  // map<string, map<string, int> > Table;
			
			
			if(pnum==0)
			{
				print_PARSE(); print_TS();
				cout << "  SYNTAX ERROR (" << TOS << " vs. " << PEEK << ")" << endl;
				break;
			}
			else
			{
				print_PARSE(); print_TS();
				cout << "  APPLY " << pnum << endl;
				
				
				Production P=Productions[pnum-1];
				PARSE_STK.pop(); // POP	
				for(int j=P.RHS.size()-1; j>=0; j--)
				{
					PARSE_STK.push(P.RHS[j]);
				}
				
				if(PARSE_STK.top()=="L")
					PARSE_STK.pop();
			}
		}	
	}
	
	if(ACCEPT)
	{
		print_PARSE(); print_TS();
		cout << "  ACCEPT" << endl;
	}
	cout << "\n------------------------------------------------------------------" << endl << endl;
	

/////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
	
	
	//system("pause");
	//system("cls");
	//FINAL_CLEAR();
	while(!PARSE_STK.empty()) PARSE_STK.pop();
	while(!TS.empty()) TS.pop();
	goto REDO;
	
	
	return 0;
}
