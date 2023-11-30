#pragma once
class CYK
{
private:
	ParseTree* tree = NULL;
	string str = "";
	vector<pair<string, pair<string, string> > > two_prod;
	vector<pair<string, string> > one_prod;
	vector<int> two_prod_nums, one_prod_nums;

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

	//for (i = 0; i < one_prod.size(); ++i)
	//	cout << one_prod[i].first << ' ' << one_prod[i].second << ' ' << one_prod_nums[i] << '\n';

	//for (i = 0; i < two_prod.size(); ++i)
	//	cout << two_prod[i].first << ' ' << two_prod[i].second.first << ' ' << two_prod[i].second.second << ' ' << two_prod_nums[i] << '\n';
}

vector<pair<string, pair<int, pair<pair<int, int>, pair<int, int> > > > > CYK::get_one_prods(string s)
{
	vector<pair<string, pair<int, pair<pair<int, int>, pair<int, int> > > > > res;
	int i;
	
	for (i = 0; i < one_prod.size(); ++i) {
		//cout << one_prod[i].second << ' ' << s << '\n';
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

	//for (i = 0; i < n; ++i, cout << '\n')
	//	for (j = 0; j < n; ++j, cout << '\n')
	//		for (auto p : dp[i][j])
	//			cout << i << ' ' << j << ' '<< p.first << ' ';

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
