#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <map>
#include <unordered_map>
#include <iomanip>
#include <sstream>
using namespace std;

float **initGraph(int size);
void adjMatrix(unordered_map<string, vector<string>> &map, float **graph);
float *r(int size);
float *powerIteration(float **matrix, float *v, int p, int size);
string *split(string s, char dlim);

int main() {
	map<string, float> pageRanks;		// page ranks of urls
	unordered_map<string, vector<string>> URLS; // web pages
	string *input = new string[2];		// input to read, only reads 2 stringat a time
	string in = "";							// input to cin
	int lines;							// number of lines to read from input
	int pwerit;							// number of power iterations
	float **internet;						// the internet
	float *ranks;						// vector of page ranks 
	int SIZE;							// size of internet and vector of page ranks

	// First line of input
	// Line 1 contains the number of lines (n) that will follow 
	// and the number of power iterations you need to perform
	getline(cin, in);
	input = split(in, ' ');
	lines = atoi(input[0].c_str());
	pwerit = atoi(input[1].c_str());

	fill_n(input, 2, 0);  // clear the input so we can reuse vector

	int i = 0;			// counter for loops

	// Read in the urls
	while (i < lines) {
		getline(cin, in);

		//split the string into two urls
		input = split(in, ' ');
	
		URLS[input[0]].push_back(input[1]);
		
		auto it = URLS.find(input[1]);

		if (it == URLS.end()) {
			URLS[input[1]];
		}

		//clear input to be used again
		fill_n(input, 2, 0);

		i++;
	}

	SIZE = URLS.size();

	internet = initGraph(SIZE); // initialize the internet 

	adjMatrix(URLS, internet); // add values to matrix using map 

	ranks = r(SIZE); 	// Vx1 matrix for power iterations

	// Power iterations to compute page ranks 
	ranks = powerIteration(internet, ranks, pwerit, SIZE);

	i = 0;

	// Fill map of urls with their page ranks 
	for (auto &x : URLS) {
		pageRanks[x.first] = ranks[i];
		i++;
	}

	// Print out urls and page ranks 
	for (auto &x : pageRanks) {
		cout << fixed << setprecision(2);
		cout << x.first << " " << x.second << endl;
	}

	return 0;
}

// create the internet
float **initGraph(int size) {
	float **graph = new float*[size];
	for (int i = 0; i < size; i++) {
		graph[i] = new float[size];
	}

	// populate matrix with 0's
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			graph[i][j] = 0.0f;
		}
	}

	return graph;
}

// Adjacency Matrix for out degrees 
void adjMatrix(unordered_map<string, vector<string>> &map, float **graph) {
	string url = ""; // url in value vector of map
	int j = 0; // row iterator
	int k = 0; // column iterator

	// iterate through the map
	for (auto &x : map) {
		// if the url, x, doesn't point to any other urls
		// then populate it's column with 0's
		if (x.second.empty()) {

			for (int i = 0; i < map.size() ; i++) 
				graph[i][j] = 0.0f;

		// If the url, x, points to other urls
		// iterate through them
		}else{
			// iterate through the urls that x points to
			for (int i = 0; i < x.second.size(); i++) {
				k = 0;
				url = x.second.at(i);
				// iterate through the map again
				// compare each url that x points to, to the other urls 
				// in the map
				for (auto &y : map) {
					// if the urls match, place the outdegree 
					// in the proper column/row
					if (url == y.first) {
						graph[k][j] = 1.0f / x.second.size();
					}
					k++;
				}
			}
		}

		j++;
	}
}

// Initialize Vx1 matrix for page ranks 
float *r(int size) {
	float *arr = new float[size];

	for (int i = 0; i < size; i++) {
		arr[i] = 1.0f / size;
	}

	return arr;
}

// Power iterations to compute page ranks 
float *powerIteration(float **matrix, float *v, int p, int size) {
	int it = 0; // iterator for power iterations
	float *ranks = new float[size];	// page ranks

	// Power iterations are from 2..n
	// So we iteratate while it <= p-2
	while (it <= p - 2) {
		//iterate through the matrix 
		for (int i = 0; i < size; i++) {
			ranks[i] = 0.0f;
			for (int j = 0; j < size; j++) {
				// Matrix/vector multiplication
				ranks[i] += matrix[i][j]*v[j];
			}
		}

		// populate your vector with the new ranks
		// new values will be used to multiply matrix 
		// if there power iteration value > 2
		for (int i = 0; i < size; i++)
			v[i] = ranks[i];

		it++;
	}

	return v;
}

// split the input 
string *split(string s, char dlim) {
	std::istringstream ss(s);
	string line;
	string *splitStrings = new string[2];
	int i = 0;

	while (getline(ss, line, dlim)) {
		splitStrings[i] = line;
		i++;
	}

	return splitStrings;
}