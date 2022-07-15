
#include<iostream>
#include<string>
#include<sstream>
#include<vector>
using namespace std;

int main(){
    
    vector<string> SententialForm;
	string s;
    
    getline(cin,s); 
    string result;
    stringstream input(s);
    while(input>>result)
        SententialForm.push_back(result);
    //for(int i=0;i<SententialForm.size();i++){
       // cout<<SententialForm[i]<<endl;
    //}
	
	vector<string> token;
	int x=0;
	int i=0;
	//int z=0;
	token.push_back("Prog"); 
	cout<<"Prog"<<endl;
	while(i<SententialForm.size()){
	if(token[x]==SententialForm[i]){
		x++;
		i++;
	}
	else if(token[x]=="Prog"){
        if(SententialForm[i]=="floatdcl" || SententialForm[i]=="intdcl" || SententialForm[i]=="id" || SententialForm[i]=="print" || SententialForm[i]=="$"){
		token.pop_back();
		token.push_back("Dcls");
		token.push_back("Stmts");
		token.push_back("$");
		}
		else{
		cout<<"false";
		}
		for(int j=0;j<token.size();j++){
        cout<<token[j]<<" ";
    }
	cout<<endl;
	}
	else if(token[x]=="Dcls"){
        if(SententialForm[i]=="floatdcl" || SententialForm[i]=="intdcl"){
		token.erase(token.begin()+x);
		token.insert(token.begin()+x,"Dcl");
		token.insert(token.begin()+x+1,"Dcls");
		}
		else if(SententialForm[i]=="id" || SententialForm[i]=="print" || SententialForm[i]=="$"){
		token.erase(token.begin()+x);
		}
		else{
		cout<<"false";
		}
		for(int j=0;j<token.size();j++){
        cout<<token[j]<<" ";
    
    }
	cout<<endl;
	}
	else if(token[x]=="Dcl"){
        if(SententialForm[i]=="floatdcl"){
		token.erase(token.begin()+x);
		token.insert(token.begin()+x,"floatdcl");
		token.insert(token.begin()+x+1,"id");
		}
		else if(SententialForm[i]=="intdcl"){
		token.erase(token.begin()+x);
		token.insert(token.begin()+x,"intdcl");
		token.insert(token.begin()+x+1,"id");
		}
		else{
		cout<<"false";
		}
		for(int j=0;j<token.size();j++){
        cout<<token[j]<<" ";
    
    }
	cout<<endl;
	}
	else if(token[x]=="Stmts"){
        if(SententialForm[i]=="id" || SententialForm[i]=="print"){
		token.erase(token.begin()+x);
		token.insert(token.begin()+x,"Stmt");
		token.insert(token.begin()+x+1,"Stmts");
		}
		else if(SententialForm[i]=="$"){
		token.erase(token.begin()+x);
		}
		else{
		cout<<"false";
		}
		for(int j=0;j<token.size();j++){
        cout<<token[j]<<" ";
    
    }
	cout<<endl;
	}
	else if(token[x]=="Stmt"){
        if(SententialForm[i]=="id"){
		token.erase(token.begin()+x);
		token.insert(token.begin()+x,"id");
		token.insert(token.begin()+x+1,"assign");
		token.insert(token.begin()+x+2,"Val");
		token.insert(token.begin()+x+3,"Expr");
		}
		else if(SententialForm[i]=="print"){
		token.erase(token.begin()+x);
		token.insert(token.begin()+x,"print");
		token.insert(token.begin()+x+1,"id");
		}
		else{
		cout<<"false";
		}
		for(int j=0;j<token.size();j++){
        cout<<token[j]<<" ";
    
    }
	cout<<endl;
	}
	else if(token[x]=="Expr"){
        if(SententialForm[i]=="plus"){
		token.erase(token.begin()+x);
		token.insert(token.begin()+x,"plus");
		token.insert(token.begin()+x+1,"Val");
		token.insert(token.begin()+x+2,"Expr");
		}
		else if(SententialForm[i]=="minus"){
		token.erase(token.begin()+x);
		token.insert(token.begin()+x,"minus");
		token.insert(token.begin()+x+1,"Val");
		token.insert(token.begin()+x+2,"Expr");
		}
		else if(SententialForm[i]=="id" || SententialForm[i]=="print" || SententialForm[i]=="$"){
		token.erase(token.begin()+x);
		}
		else{
		cout<<"false";
		}
		for(int j=0;j<token.size();j++){
        cout<<token[j]<<" ";
    
    }
	cout<<endl;
	}
	else if(token[x]=="Val"){
        if(SententialForm[i]=="id"){
		token.erase(token.begin()+x);
		token.insert(token.begin()+x,"id");
		}
		else if(SententialForm[i]=="inum"){
		token.erase(token.begin()+x);
		token.insert(token.begin()+x,"inum");
		}
		else if(SententialForm[i]=="fnum"){
		token.erase(token.begin()+x);
		token.insert(token.begin()+x,"fnum");
		}
		else{
		cout<<"false";
		}
		for(int j=0;j<token.size();j++){
        cout<<token[j]<<" ";
    
    }
	cout<<endl;
	}
	//z++;
	}
	if((i==SententialForm.size())){
		cout<<"accept"<<endl;
	}
    return 0;
}
