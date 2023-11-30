#pragma once
struct ParseTree
{
	string value = "";
	vector<ParseTree*> children;

	void print()
	{
		cout << value << '\n';
	}
};

