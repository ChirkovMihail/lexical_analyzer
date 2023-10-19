#include "Libraries.h"
#include "DFSM.h"
#include "Token.h"
#include "Hash_table.h"

void input_dfa(DFSM*& dfsm)
{
	int num_of_states, size_of_alph, num_of_prods, num_of_weights, num_of_final_states, i;
	int curr_state, next_state, weight_ind;
	string weight;

	vector<char> alphabet;
	vector<set<char> > weights;
	vector<bool> final_states;
	vector<vector<int> > transit_func;

	ifstream fin("input_dfsm.txt");

	fin >> num_of_states;

	fin >> size_of_alph;
	alphabet.resize(size_of_alph);
	for (i = 0; i < size_of_alph; ++i)
		fin >> alphabet[i];

	fin >> num_of_weights;
	weights.resize(num_of_weights);
	for (i = 0; i < num_of_weights; ++i) {
		fin >> weight;		
		for (auto c : weight)
			weights[i].insert(c);
	}

	fin >> num_of_final_states;
	final_states.resize(num_of_states);
	for (i = 0; i < num_of_final_states; ++i) {
		fin >> curr_state;
		final_states[curr_state - 1] = true;
	}

	transit_func.resize(num_of_states, vector<int>(num_of_weights, -1));
	fin >> num_of_prods;
	for (i = 0; i < num_of_prods; ++i) {
		fin >> curr_state >> next_state >> weight_ind;
		transit_func[curr_state - 1][weight_ind - 1] = next_state - 1;
	}

	dfsm = new DFSM(num_of_states, alphabet, weights, final_states, transit_func);

	fin.close();
	alphabet.shrink_to_fit();
	weight.shrink_to_fit();
	for (auto w : weights)
		w.clear();
	final_states.shrink_to_fit();
	for (auto t : transit_func)
		t.shrink_to_fit();
	transit_func.shrink_to_fit();
}

int main()
{
	DFSM* dfsm;
	input_dfa(dfsm);

	HashTable hashh(1000);

	ifstream fin("text_input.txt");
	string word, type;

	ofstream fout("text_output.txt");

	while (!fin.eof()) {
		fin >> word;
		if (fin.eof())
			break;
				
		type = dfsm->is_accept(word);		
				
		if (type.size() > 0) {
			Token new_tok(word, type);
			hashh.insert_token(new_tok);
		}
		else
		{
			fout << "Wrong word : " << word << '\n';
		}
	}

	hashh.print_table(fout);

	fin.close();
	return 0;
}