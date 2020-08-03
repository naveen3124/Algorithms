/*

Consider n - by - n grid of cells where each cell(aka site) is a person with or without an infectious disease.
Each person is directly connected to either 2 or 3 or 4 persons.
If p is the probability of a person being infectious then 1 - p is the probability that the person is not infectious.
How many persons need to be infectious for the infection to get transferred from the top row to the bottom row ?
If p is low(0.4) we can say for sure that the infection will not get transferred from the top row to the bottom row.
If p is very high(0.8) we can say for sure that the infection will get transferred from the top row to the bottom row.
But if p is medium(0.6) we cannot say for sure.
The task is to find the threshold value of p when the infection will get transferred from the top row to the bottom row.


*/

/*solution
question is unclear whether the people in the top row are the only ones infected or any grid in the cell can be infected so assuming the later
we can use any 2d array to run the simulation of the grid. if the grid is sparsely connected any Graph with adjacency list is good enough.

algorithm:

1. initialize N x N grid of cells as uninfected•
2. Randomly infect the cells
3. check unti system percolates. 
4. to check whether system percolated we use UNION FIND with size optimization algorithm to check whether top row is connected to bottom .
5. no of infected cells divided by the total no of cells gives the p_threshold.
6. repeat this simulation some emperical X times  and take a mean to obtain accurate estimate of p_threshold 
*/

#include <iostream>
#include <stdlib.h>
#include <algorithm>
using namespace std;

class Simulation
{
	char** grid ;
	int size;
	bool** visited ;
	int* parent;
	int* psize;
	bool ispercolating;
	int infected_sites;
public:
	bool issafe(int r, int c)
	{
		return ((r >= 0) && (c >= 0) && (c < size) && (r < size));
	}
	bool issafeandopen(int r, int c)
	{
		return ((r >= 0) && (c >= 0) && (c < size)&& (r < size)&& (visited[r][c]));

	}
	int find_set(int v) {
		if (v == parent[v])
			return v;
		return parent[v] = find_set(parent[v]);
	}
	void make_set(int v) {
		parent[v] = v;
		psize[v] = 1;
	}
	// small tree is connected to large tree
	void union_sets(int a, int b) {
		a = find_set(a);
		b = find_set(b);
		if (a != b) {
			if (psize[a] < psize[b])
				swap(a, b);
			parent[b] = a;
			psize[a] += psize[b];
		}
	}
	//to convert 2d index to 1 ;
	int findId(int row, int col) {
		return size * row + col;
	};
	Simulation(int n)
	{
		infected_sites = 0;
		size = n;
		ispercolating = false;
		grid = new char* [n];
		visited = new bool* [n];
		int tmp = n * n;
		parent = new int [tmp];
		psize = new int [tmp];
		for (int i = 0; i < n; i++)
		{
			grid[i] = new char[n];
			visited[i] = new bool[n];
		}
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				grid[i][j] = 0;
				visited[i][j] = false;
				
			}
		}
		//each of them connected to themselves and parent of themselves
		for (int i = 0; i < n * n; i++) {
			make_set(i);
		};
	}

	//this infects the current cell and also all adjacent cells 
	void  infect(int r, int c)
	{
		int cur_id = findId(r, c);
		if (!issafe(r, c))
			return;
		if(!visited[r][c])
			infected_sites++;

		visited[r][c] = true;
		//top
		if (issafeandopen(r-1,c)) {
			int top = findId(r - 1, c);
			union_sets(cur_id, top);
		};
		//right
		if (issafeandopen(r, c+1)) {
			int right = findId(r , c+1);
			union_sets(cur_id, right);
		};
		//bottom
		if (issafeandopen(r + 1, c)) {
			int bottom = findId(r + 1, c);
			union_sets(cur_id, bottom);
		};
		//left
		if (issafeandopen(r, c - 1)) {
			int left = findId(r, c - 1);
			union_sets(cur_id, left);
		};

	}
	void display()
	{
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				cout << visited[i][j] << " ";
			}
			cout << endl;
		}
	}
	bool isconnected(int row, int col) {
		int id = findId(row, col);
		return psize[id] > 1;
	};
	int noofinfected()
	{
		return infected_sites;
	}
	bool ispercolates() {
		int toprow;
		int bottomrow;
		for (int i = 0; i < size; i++) {
			toprow = findId(0, i);
			for (int j = 0; j < size; j++) {
				bottomrow = findId(size - 1, j);
				if (find_set(toprow) == find_set(bottomrow)) {
					ispercolating = true;
					break;
				}
			}
		}
		return ispercolating;
	};
	~Simulation()
	{
		for (int i = 0; i < size; i++)
		{
			delete[] grid[i];
			delete[] visited[i];
		}
		delete[]parent;
		delete[]psize;
		delete[] grid;
		delete[] visited;
	}


};

int main()
{
	int simulations = 50;
	cout << "enter no of trails" << endl;
	//cin >> simulations;
	double* percolate_threshold_array = new double[simulations];
	int n = 100;
	cout << "enter square grid size" << endl;
	//cin >> n;
	for (int i = 0; i < simulations; i++) {
		Simulation* sim = new Simulation(n);
		// we infect until we have percolation
		while (!sim->ispercolates()) {
				int row = rand()% n;
				int col = rand()% n;
				//infect a random cell
				sim->infect(row, col);
		}
		int infected_sites = sim->noofinfected();
		double res = (double)infected_sites / (double) (n * n);
		percolate_threshold_array[i] = res;
		delete sim;
	}
	//now mean value of the simulation is the appropriate p_threshold (central limit theorem)
	double p_thres = 0.0;
	for (int i = 0; i < simulations; i++)
	{
		p_thres += percolate_threshold_array[i];
	}
	p_thres = (double)(p_thres / simulations); 
	cout <<"pthreshold is" << p_thres << endl;
	delete[]percolate_threshold_array;
	return 0;
}


	