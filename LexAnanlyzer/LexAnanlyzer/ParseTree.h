#pragma once
struct ParseTree
{
	string value = "";
	int num = -1;
	vector<ParseTree*> children;

	void print()
	{
		cout << value << " (" << num << ")\n";
	}
};

