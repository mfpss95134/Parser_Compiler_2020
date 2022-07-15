#include<iostream>
#include<fstream>
#include<iostream>
#include<string>
#include<sstream>
#include<vector>
using namespace std;
int main()
{
	fstream file;
	vector<string>table;
	vector<vector<string>>table2;
	string line;
	string tmp; //暫存
	istringstream iline; //一行的資料流
						 //string filename;
						 //getline(cin,filename); 
	ifstream in("CFG.txt");
	if (!file) {
		cout << ("檔案無法開啟!\n");
	}
	else
	{
		while (getline(in, line)) {
			iline.str(line);

			while (iline >> tmp) {
				table.push_back(tmp);
			}
			table2.push_back(table);
			iline.clear();
			table.clear();
		}
		file.close();
	}

	//	int i, j;
	//    for( i=0 ; i<table2.size() ; i++ ){
	//   for( j=0 ; j<table2[i].size() ; j++ ){
	//     cout << table2[i][j] << " ";
	//    }
	//    cout << endl;
	//  }
	//	cout<<table2[3][4];

	vector<string>nonterminal;
	int i;
	for (i = 0; i<table2.size(); i++) {
		//		if(table2[i][1]!="|"){
		nonterminal.push_back(table2[i][1]);
		//		}	
	}



	//	for(int j=0;j<nonterminal.size();j++){
	//       cout<<nonterminal[j]<<endl;
	//   }


	vector<vector<string>>transform;
	vector<vector<string>>transform2;
	vector<string>pt;
	vector<vector<string>>pt2;
	int x1 = 0;
	while (x1<table2.size()) {
		if (table2[x1][2] != ">" && x1>0) {
			table2[x1].erase(table2[x1].begin() + 1);
			table2[x1].insert(table2[x1].begin() + 1, table2[x1 - 1][1]);
			table2[x1].insert(table2[x1].begin() + 2, table2[x1 - 1][2]);
		}
		x1++;
	}
	int x = 0;
	while (x<table2.size()) {    ////first and follow

		if (table2[x][2] == ">") {
			int temp = 0;
			int j = 0, jt = 0;
			transform2.push_back(table2[x]);

			while (transform2.size()>0) {
				transform.push_back(transform2[0]);

				for (j = 0; j<nonterminal.size(); j++) {

					if (nonterminal[j] == transform2[0][3]) {
						temp = 1;
						jt = j;
						break;
					}
				}
				while (temp == 1) {
					transform2[0].erase(transform2[0].begin() + 3);
					int n;
					for (n = 3; n<table2[jt].size(); n++) {
						if (table2[jt][n] != "L") {
							transform2[0].insert(transform2[0].begin() + n, table2[jt][n]);
						}
					}
					int y = 1;
					if ((jt + y)<table2.size() && (jt + y) != (table2.size() - 1)) {
						while (table2[jt + y][1] == table2[jt][1]) {
							transform2.push_back(transform[0]);
							for (int tt = 1; tt<transform2.size() - 1; tt++) {
								transform2.push_back(transform2[1]);
								transform2.erase(transform2.begin() + 1);
							}
							transform2[y].erase(transform2[y].begin() + 3);
							int m;
							for (m = 3; m<table2[jt + y].size(); m++) {

								if (table2[jt + y][m] != "L") {
									transform2[y].insert(transform2[y].begin() + m, table2[jt + y][m]);
								}
							}

							if (transform2[y].size()<4) {
								transform2.erase(transform2.begin() + y);
								if (table2[x][3] != "L") {
									int t1, t2, y1 = y, temp1 = 0, x2 = x;
									vector<string>check;
									while (temp1 != 1) {
										for (t1 = 0; t1<table2.size(); t1++) {
											for (t2 = 3; t2<table2[t1].size(); t2++) {
												temp1 = 1;
												if (table2[x2][1] == table2[t1][t2]) {
													if (t2 == table2[t1].size() - 1) {
														x2 = t1;
														temp1 = 0;
													}
													else {
														transform2.push_back(transform[0]);
														transform2[y1].erase(transform2[y1].begin() + 3);
														if (t2 + 1<table2[t1].size()) {
															for (m = t2 + 1; m<table2[t1].size(); m++) {
																transform2[y1].push_back(table2[t1][m]);
															}
														}
														y1++;
													}
													check.push_back(table2[t1][1]);
												}
											}
										}
										if (check.size()>1) {
											for (int c1 = 0; c1<check.size(); c1++) {
												for (int c2 = c1 + 1; c2<check.size(); c2++) {
													if (check[c1] == check[c2]) {
														temp1 = 0;
														for (int c3 = 0; c3<table2.size(); c3++) {
															if (table2[c3][1] == check[c1]) {
																x2 = c3;
															}
														}
														check.erase(check.begin() + c1);
													}
												}
											}
										}
									}
								}
							}
							y++;
						}
					}
					if ((jt + 1) == (table2.size() - 1)) {
						transform2.push_back(transform[0]);
						for (int tt = 1; tt<transform2.size() - 1; tt++) {
							transform2.push_back(transform2[1]);
							transform2.erase(transform2.begin() + 1);
						}
						transform2[1].erase(transform2[1].begin() + 3);
						int m;
						for (m = 3; m<table2[jt + 1].size(); m++) {

							if (table2[jt + 1][m] != "L") {
								transform2[1].insert(transform2[1].begin() + m, table2[jt + 1][m]);
							}
						}
						if (transform2[1].size()<4) {
							transform2.erase(transform2.begin() + 1);
							if (table2[x][3] != "L") {
								int t1, t2, y1 = 1, temp1 = 0, x2 = x;
								vector<string>check;
								while (temp1 != 1) {
									for (t1 = 0; t1<table2.size(); t1++) {
										for (t2 = 3; t2<table2[t1].size(); t2++) {
											temp1 = 1;
											if (table2[x2][1] == table2[t1][t2]) {
												if (t2 == table2[t1].size() - 1) {
													x2 = t1;
													temp1 = 0;
												}
												else {
													transform2.push_back(transform[0]);
													transform2[y1].erase(transform2[y1].begin() + 3);
													if (t2 + 1<table2[t1].size()) {
														for (m = t2 + 1; m<table2[t1].size(); m++) {
															transform2[y1].push_back(table2[t1][m]);
														}
													}
													y1++;
												}
												check.push_back(table2[t1][1]);
											}
										}
									}
									if (check.size()>1) {
										for (int c1 = 0; c1<check.size(); c1++) {
											for (int c2 = c1 + 1; c2<check.size(); c2++) {
												if (check[c1] == check[c2]) {
													temp1 = 0;
													for (int c3 = 0; c3<table2.size(); c3++) {
														if (table2[c3][1] == check[c1]) {
															x2 = c3;
														}
													}
													check.erase(check.begin() + c1);
												}
											}
										}
									}
								}
							}
						}
					}


					for (j = 0; j<nonterminal.size(); j++) {
						if (nonterminal[j] == transform2[0][3]) {
							temp = 1;
							jt = j;
							break;
						}
						else
							temp = 0;
					}


				}
				if (temp == 0) {

					if (transform2[0][3] == "L") {
						transform2.erase(transform2.begin());
						int t1, t2, y1 = 0, temp = 0, x2 = x, m;
						vector<string>check;
						while (temp != 1) {
							temp = 1;
							for (t1 = 0; t1<table2.size(); t1++) {
								for (t2 = 3; t2<table2[t1].size(); t2++) {
									
									if (table2[x2][1] == table2[t1][t2]) {
										if (t2 == table2[t1].size() - 1 && t1 != 0) {
											x2 = t1;
											temp = 0;
										}
										else {
											transform2.push_back(transform[0]);
											transform2[y1].erase(transform2[y1].begin() + 3);
											if (t2 + 1<table2[t1].size()) {
												for (m = t2 + 1; m<table2[t1].size(); m++) {
													transform2[y1].push_back(table2[t1][m]);
												}
											}
											y1++;
										}
										check.push_back(table2[t1][1]);
									}
								}
							}
							if (check.size()>1) {
								for (int c1 = 0; c1<check.size(); c1++) {
									for (int c2 = c1 + 1; c2<check.size(); c2++) {
										if (check[c1] == check[c2]) {
											temp = 0;
											for (int c3 = 0; c3<table2.size(); c3++) {
												if (table2[c3][1] == check[c1] && c3 != 0) {
													x2 = c3;
												}
											}
											check.erase(check.begin() + c1);

										}
									}
								}
							}
						}

					}
					for (j = 0; j<nonterminal.size(); j++) {

						if (nonterminal[j] == transform2[0][3]) {
							temp = 1;
						}
					}
					if (transform2[0][3] != "L"&& temp == 0) {
						pt.push_back(transform2[0][3]);
						transform2.erase(transform2.begin());
					}
					transform.erase(transform.begin());

				}

			}
		}
		pt2.push_back(pt);
		pt.clear();
		x++;

	}
	int c, d;
	for (c = 0; c<pt2.size(); c++) {
		for (d = 0; d<pt2[c].size(); d++) {
			cout << pt2[c][d] << " ";
		}
		cout << endl;
	}



	return 0;
}
