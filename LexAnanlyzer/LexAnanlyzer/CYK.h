#pragma once
class CYK
{
private:
	ParseTree* tree = NULL;
	string str = "";
	int num_of_vert = 0;
	vector<pair<string, pair<string, string> > > two_prod;
	vector<pair<string, string> > one_prod;
	vector<int> two_prod_nums, one_prod_nums;
	vector<vector<int> > sem_adj_list;
	vector<vector<string> > sem_rules;
	vector<int> indexes;
	vector<string> atrib;
	vector<pair<string, pair<int, string> > > variables;

	vector<vector<set<pair<string, pair<int, pair<pair<int, int>, pair<int, int> > > > > > > dp;

	vector<pair<string, pair<int, pair<pair<int, int>, pair<int, int> > > > > get_one_prods(string s);
	vector<pair<string, pair<int, pair<pair<int, int>, pair<int, int> > > > > get_two_prods(set<pair<string, pair<int, pair<pair<int, int>, pair<int, int> > > > > a,
		set<pair<string, pair<int, pair<pair<int, int>, pair<int, int> > > > >, pair<int, int> w1, pair<int, int>w2);
	

public:
	CYK() { tree = new ParseTree; };
	void ReadProdutions(istream& in);
	bool Accept(string& s);
	void PrintTree(ParseTree* vert, int level);
	void PrintBinaryTree(ParseTree* vert, int level);
	void PrintTree();
	void CompleteNums();
	void CompleteNums(ParseTree* vert, int &k);
	void ReadRules(ifstream& in);
	void create_tree(ParseTree *vert, pair<string, pair<int, pair<pair<int, int>, pair<int, int> > > > p) {
		int i;
		vert->value = p.first;

		if (p.second.first < one_prod_nums.size()) {
			ParseTree* newby = new ParseTree;

			vert->children.push_back(newby);

			for (i = 0; i < one_prod_nums.size(); ++i)
				if (one_prod[i].first == p.first && one_prod_nums[i] == p.second.first) {
					vert->children[0]->value = one_prod[i].second;
					return;
				}
		}

		ParseTree* newby1 = new ParseTree;
		ParseTree* newby2 = new ParseTree;

		vert->children.push_back(newby1);
		vert->children.push_back(newby2);

		pair<string, string> output;
		for (i = 0; i < two_prod_nums.size(); ++i)
			if (two_prod[i].first == p.first && two_prod_nums[i] == p.second.first) {
				output = two_prod[i].second;
				break;
			}

		for (auto p : dp[p.second.second.first.first][p.second.second.first.second]) {
			if (p.first == output.first) {
				create_tree(vert->children[0], p);
				break;
			}
		}

		for (auto p : dp[p.second.second.second.first][p.second.second.second.second]) {
			if (p.first == output.second) {
				create_tree(vert->children[1], p);
				break;
			}
		}
	}
	void CreateDepGraph();
	void CreateDepGraph(ParseTree* vert);
	void CompleteAtrib();
	void CheckAtrib(ostream& out);
	void CheckAtrib(ParseTree* vert, ostream& out);
	string CreateRPN();
	string CreateRPN(ParseTree* vert);
};

void CYK::ReadProdutions(istream& in)
{
	int n, i, type;
	string input, output1, output2;

	in >> n;

	for (i = 0; i < n; ++i) {
		in >> input;
		in >> type;
		if (type == 1) {
			in >> output1;
			one_prod.push_back(make_pair(input, output1));
		}
		else
		{
			in >> output1 >> output2;
			two_prod.push_back(make_pair(input, make_pair(output1, output2)));
		}
	}

	sort(all(one_prod));
	one_prod_nums.push_back(0);
	for (i = 1; i < one_prod.size(); ++i)
		if (one_prod[i].first == one_prod[i - 1].first)
			one_prod_nums.push_back(one_prod_nums.back() + 1);
		else
			one_prod_nums.push_back(0);

	sort(all(two_prod));
	two_prod_nums.push_back(one_prod_nums.size());
	for (i = 1; i < two_prod.size(); ++i)
		if (two_prod[i].first == two_prod[i - 1].first)
			two_prod_nums.push_back(two_prod_nums.back() + 1);
		else
			two_prod_nums.push_back(one_prod_nums.size());
}

vector<pair<string, pair<int, pair<pair<int, int>, pair<int, int> > > > > CYK::get_one_prods(string s)
{
	vector<pair<string, pair<int, pair<pair<int, int>, pair<int, int> > > > > res;
	int i;
	
	for (i = 0; i < one_prod.size(); ++i) {		
		if (one_prod[i].second == s)
			res.push_back(mp(one_prod[i].first, mp(one_prod_nums[i], mp(mp(-1, -1), mp(-1, -1)))));
	}

	return res;
}

vector<pair<string, pair<int, pair<pair<int, int>, pair<int, int> > > > > CYK::get_two_prods(set<pair<string, pair<int, pair<pair<int, int>, pair<int, int> > > > > a,
	set<pair<string, pair<int, pair<pair<int, int>, pair<int, int> > > > > b,
	pair<int, int> w1, pair<int, int> w2)
{
	vector<pair<string, pair<int, pair<pair<int, int>, pair<int, int> > > > > res;
	int i;

	for (auto p1 : a) {
		for (auto p2 : b) {
			for (i = 0; i < two_prod.size(); ++i)
				if (two_prod[i].second.first == p1.first && two_prod[i].second.second == p2.first)
					res.push_back(mp(two_prod[i].first, mp(two_prod_nums[i], mp(w1, w2))));
		}
	}

	return res;
}

bool CYK::Accept(string& s)
{
	int n = s.size(), i, j, k;
	vector<pair<string, pair<int, pair<pair<int, int>, pair<int, int> > > > > res;
	string w;

	dp.resize(n, vector<set<pair<string, pair<int, pair<pair<int, int>, pair<int, int> > > > > >(n));

	for (i = 0; i < n; ++i) {
		w = "";
		w += s[i];
		res = get_one_prods(w);
		if (res.size() > 0) {			
			dp[0][i].insert(all(res));
		}
	}

	for (i = 1; i < n; ++i) {
		for (j = 0; j < n - i; ++j) {
			for (k = 0; k <= i - 1; ++k) {
				res = get_two_prods(dp[k][j], dp[i - k - 1][j + k + 1], mp(k, j), mp(i - k - 1, j + k + 1));
				if (res.size() > 0)
					dp[i][j].insert(all(res));
			}
		}
	}

	bool flag = false;
	pair<string, pair<int, pair<pair<int, int>, pair<int, int> > > > start;

	for (auto p : dp[n - 1][0])
		if (p.first == "Function") {
			flag = true;
			start = p;
			break;
		}

	if (!flag) {
		return false;
	}

	tree = new ParseTree;
	create_tree(tree, start);
	return true;
}

void CYK::PrintTree(ParseTree* vert, int level)
{
	for (int i = 0; i < level; ++i)
		cout << "    ";
	vert->print();
	for (int i = 0; i < vert->children.size(); ++i)
		PrintTree(vert->children[i], level + 1);
}

void CYK::PrintBinaryTree(ParseTree* vert, int level)
{
	if (vert->children.size() <= 1){
		for (int i = 0; i < level; ++i)
			cout << "    ";
		vert->print();
		for (int i = 0; i < vert->children.size(); ++i)
			PrintBinaryTree(vert->children[i], level + 1);
	}
	else
	{
		PrintBinaryTree(vert->children[0], level + 1);
		for (int i = 0; i < level; ++i)
			cout << "    ";
		vert->print();
		PrintBinaryTree(vert->children[1], level + 1);
	}
	
}

void CYK::PrintTree()
{
	//PrintTree(tree, 0);
	PrintBinaryTree(tree, 0);
}

void CYK::CompleteNums()
{
	int zero = 0;
	CompleteNums(tree, zero);
}

void CYK::CompleteNums(ParseTree* vert, int &k)
{
	vert->num = k;
	k++;
	for (int i = 0; i < vert->children.size(); ++i) {
		CompleteNums(vert->children[i], k);
	}
	num_of_vert = max(num_of_vert, k);
}

void CYK::ReadRules(ifstream& in)
{
	sem_adj_list.resize(num_of_vert);
	atrib.resize(num_of_vert);

	int n, i, j, k;
	string s;
	vector<string> buf(5);

	in >> n;

	for (i = 0; i < n; ++i) {
		in >> buf[0];
		in >> k;
		if (k == 1)
			in >> buf[1];
		else
			in >> buf[1] >> buf[2];
		in >> buf[3] >> buf[4];
		sem_rules.push_back(buf);
	}
}

void CYK::CreateDepGraph()
{
	CreateDepGraph(tree);
}

int find_in_subtree(ParseTree* vert, string& s)
{
	queue<ParseTree*> Q;
	ParseTree* curr_vert;

	Q.push(vert);
	while (!Q.empty())
	{
		curr_vert = Q.front();
		Q.pop();

		if (curr_vert->value == s) {
			return curr_vert->num;
		}

		for (int i = 0; i < curr_vert->children.size(); ++i) {
			Q.push(curr_vert->children[i]);
		}
	}
	return -1;
}

void fill_name(ParseTree* vert, vector<pair<int, char> >& name)
{
	if (vert->value.size() == 1) {
		name.push_back(mp(vert->num, vert->value[0]));
	}

	for (int i = 0; i < vert->children.size(); ++i)
		fill_name(vert->children[i], name);
}

void CYK::CreateDepGraph(ParseTree* vert)
{
	int i, j, x, y;

	for (i = 0; i < vert->children.size(); ++i) {
		for (j = 0; j < sem_rules.size(); ++j) {
			if (vert->value == sem_rules[j][0] && (vert->children[i]->value == sem_rules[j][1]
				|| vert->children[i]->value == sem_rules[j][2])) {
				x = find_in_subtree(vert, sem_rules[j][3]);
				y = find_in_subtree(vert, sem_rules[j][4]);
				if (x != -1 && y != -1) 
					sem_adj_list[y].push_back(x);					
				else
				{
					if (sem_rules[j][4] == "int")
						atrib[x] = "int";
					if (sem_rules[j][4] == "double")
						atrib[x] = "double";
					if (sem_rules[j][3] == "var") {
						vector<pair<int, char> > name;
						fill_name(vert, name);
						sort(all(name));
						string s = "";
						for (auto p : name) {
							if (!(p.second >= 'a' && p.second <= 'z' || p.second >= 'A' && p.second <= 'Z') && s.size() > 0) {
								variables.push_back(mp(s, mp(y, "")));
								s = "";
							}
							else
							{
								s += p.second;
							}
						}						
						if (s.size() > 0)
							variables.push_back(mp(s, mp(y, "")));
					}

				}
				break;
			}
		}
		CreateDepGraph(vert->children[i]);
	}
}

void complete_proccesed(int vert, vector<vector<int> >& adj_list, vector<bool>& used, vector<int>& proccesed)
{
	used[vert] = 1;
	for (auto next_vert : adj_list[vert])
		if (!used[next_vert])
			complete_proccesed(next_vert, adj_list, used, proccesed);
	proccesed.push_back(vert);
}

void CYK::CompleteAtrib()
{
	vector<bool>used(num_of_vert);
	vector<int> proccesed;

	int i, j;

	for (i = 0; i < num_of_vert; ++i)
		if (!used[i])
			complete_proccesed(i, sem_adj_list, used, proccesed);

	reverse(all(proccesed));
	indexes.resize(num_of_vert, -1);
	for (i = 0; i < num_of_vert; ++i)
		if (indexes[proccesed[i]] == -1)
			indexes[proccesed[i]] = i;

	for (i = 0; i < num_of_vert; ++i)
		for (auto next_vert : sem_adj_list[proccesed[i]])
			atrib[next_vert] = atrib[proccesed[i]];

	for (i = 0; i < variables.size(); ++i)
		variables[i].second.second = atrib[variables[i].second.first];

	
}

void CYK::CheckAtrib(ostream &out)
{
	CheckAtrib(tree, out);
}

void CYK::CheckAtrib(ParseTree* vert, ostream& out)
{
	if (vert->value == "Function") {
		vector<pair<int, char> > name;
		string s = "", atr2;

		fill_name(vert->children[1]->children[1]->children[0]->children[1]->children[0], name);
		sort(all(name));

		for (auto p : name)
			s += p.second;

		for (auto p : variables)
			if (p.first == s) {
				atr2 = p.second.second;
				break;
			}

		if (atrib[vert->children[0]->num] != atr2) {
			out << "Error with return :( \n";
		}
	}

	if ((vert->value == "Op" || vert->value == "Operators") && vert->children.size() > 1 &&
		vert->children[0]->value == "Op1" && vert->children[1]->value == "Op2") {

		vector<pair<int, char> > name;
		string s = "", atr1, atr2;

		fill_name(vert->children[0]->children[0]->children[0], name);
		sort(all(name));
		
		for (auto p : name)
			s += p.second;
		
		for (auto p : variables)
			if (p.first == s) {
				atr1 = p.second.second;
				break;
			}

		atr2 = atrib[vert->children[1]->children[0]->children[0]->num];
		
		if (atr1 != atr2)
			out << "Operators error\n";
	}

	if (vert->value == "Expr" && vert->children.size() > 1 &&
		vert->children[0]->value == "Expr1" && vert->children[1]->value == "ExprPlus") {
		if (atrib[vert->children[0]->num] != atrib[vert->children[1]->num])
			out << "Operators error\n";
	}

	if (vert->value == "Expr" && vert->children.size() > 1 &&
		vert->children[0]->value == "Expr1" && vert->children[1]->value == "ExprMinus") {
		if (atrib[vert->children[0]->num] != atrib[vert->children[1]->num])
			out << "Operators error\n";
	}

	if (vert->value == "Term" && vert->children.size() > 1 &&
		vert->children[0]->value == "SimpleExprMult" && vert->children[1]->value == "SpaceWithTerm") {
		if (atrib[vert->children[0]->num] != atrib[vert->children[1]->num])
			out << "Operators error\n";
	}

	if (vert->value == "Term" && vert->children.size() > 1 &&
		vert->children[0]->value == "SimpleExprDiv" && vert->children[1]->value == "SpaceWithTerm") {
		if (atrib[vert->children[0]->num] != atrib[vert->children[1]->num])
			out << "Operators error\n";
	}

	if (vert->value == "Expr" && vert->children.size() > 1 &&
		vert->children[0]->value == "SimpleExprMult" && vert->children[1]->value == "SpaceWithTerm") {
		if (atrib[vert->children[0]->num] != atrib[vert->children[1]->num])
			out << "Operators error\n";
	}

	if (vert->value == "Expr" && vert->children.size() > 1 &&
		vert->children[0]->value == "SimpleExprDiv" && vert->children[1]->value == "SpaceWithTerm") {
		if (atrib[vert->children[0]->num] != atrib[vert->children[1]->num])
			out << "Operators error\n";
	}

	for (int i = 0; i < vert->children.size(); ++i)
		CheckAtrib(vert->children[i], out);
}