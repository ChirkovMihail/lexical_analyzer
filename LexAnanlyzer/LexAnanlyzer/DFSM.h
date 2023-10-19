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
	string get_state_type(int state);

public:
	DFSM(int num_states, vector<char>& alphabet, vector<set<char> >& weights, vector<bool>& final_states, vector<vector<int> >& transit_func);
	string is_accept(string& s, string& path);
	string is_accept(string& s);
};