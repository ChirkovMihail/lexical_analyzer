#pragma once
class DFSM
{
private:
	int num_states;
	vector<char> alphabet;
	vector<set<char> > weights;
	vector<bool> final_states;
	vector<vector<int> > transit_func;

	int get_state(int curr_state, char c);

public:
	DFSM(int num_states, vector<char>& alphabet, vector<set<char> >& weights, vector<bool>& final_states, vector<vector<int> >& transit_func);
	bool is_accept(string& s, string& path);
};