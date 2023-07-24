#include <chrono>
#include <climits>
#include <iostream>
#include <vector>
#include <list>
#include <fstream>
#include <string>
#include <cmath>
#include <queue>
#include <algorithm>


#define CLEAR   "\e[2J\e[3J\e[H"
#define RED     "\x1B[31;1m"
#define GREEN   "\x1B[32;1m"
#define RESET   "\x1B[0m"

// #define DEBUG

#define PATH ' '
#define WALL '0'

std::vector<std::string> getMaze(std::string fileName)
{

	std::fstream file(fileName);
	std::vector<std::string> maze;
	std::string line;


	while(std::getline(file,line))
	{
		maze.emplace_back(line);
	}
	file.close();
	return maze;
}

std::vector<std::list<std::pair<int,int>>> makeGraph(const std::vector<std::string>& maze)
{
	const int WEIGHT = 1;
	int actualIndex = 0;
	std::vector<std::list<std::pair<int,int>>> graph;

	for(int i = 0; i < maze.size(); i++)
	{
		for(int j = 0; j < maze[i].size(); j++)
		{
			std::list<std::pair<int,int>> temp;
			if(maze[i][j] == PATH)
			{
				if(i-1 >= 0 && maze[i-1][j] == PATH)
					temp.emplace_back(std::make_pair(actualIndex-maze[i].size(),1));
				if(i+1 < maze.size() && maze[i+1][j] == PATH)
					temp.emplace_back(std::make_pair(actualIndex+maze[i].size(),1));
				if(j-1 >= 0 && maze[i][j-1] == PATH)
					temp.emplace_back(std::make_pair(actualIndex-1,1));
				if(j+1 < maze[i].size() && maze[i][j+1] == PATH)
					temp.emplace_back(std::make_pair(actualIndex+1,1));
			}
			temp.sort();
			graph.emplace_back(temp);
			actualIndex++;
		}
	}
	return graph;
}

std::list<int> path_find(const int src, const int dest, const size_t size, std::vector<std::list<std::pair<int,int>>>& g, const std::vector<std::string>& m)
{
	std::list<int> path_taken;
	std::vector<int> dist(size,INT_MAX);
	std::vector<std::list<std::pair<int,int>>> prev(size,std::list<std::pair<int,int>>());
    std::priority_queue<std::pair<int,int>, std::vector<std::pair<int,int>>, std::greater<std::pair<int,int>>> pq;
	path_taken.emplace_back(src);
	dist[src]	= 0;
	prev[src].emplace_back(std::make_pair(src,0));
	pq.push(std::make_pair(0,src));

	while(!pq.empty())
	{
		int u = pq.top().second;
		pq.pop();
		std::list<std::pair<int,int>>::iterator it = g[u].begin();
		for(it = g[u].begin(); it != g[u].end(); it++)
		{
			int weight = it->second;

			if(dist[it->first] > dist[u] + weight) {
				dist[it->first] = dist[u] + weight;
				pq.push(std::make_pair(dist[it->first],it->first));
				prev[it->first].emplace_back(std::make_pair(u,0));
			}
		}
		 /* make everything below this better bozo */
		if(u == dest) 
		{
			int last = prev[dest].back().first;
			while(last != src) 
			{
				path_taken.emplace_back(last);
				last = prev[last].front().first;
			}
			path_taken.emplace_back(dest);
		}
	}
	path_taken.sort();
	path_taken.unique();
	return path_taken;
}

#ifdef DEBUG
inline void end(std::chrono::time_point<std::chrono::steady_clock>& start)
{
	std::cout << GREEN << "TIME: " << (std::chrono::steady_clock::now() - start).count() << RESET<< std::endl;
	std::cout << "==================" << std::endl;
}
#endif

int main(int argc, char* argv[])
{
	std::vector<std::string> maze						= {getMaze(argv[1])};
	std::vector<std::list<std::pair<int,int>>> graph	= {makeGraph(maze)};
	#ifdef DEBUG
	std::chrono::time_point start = std::chrono::steady_clock::now();
	#endif
	std::list<int> path 								= {path_find(0,graph.size()-1,graph.size(),graph,maze)};
	#ifdef DEBUG
	end(start);
	#endif

	std::list<int>::iterator it = path.begin();
	int pos = 0;

	for(auto& i : maze)
	{
		for(int j = 0; j < i.size(); j++)
		{
			if(*it == pos)
			{
				std::cout << "🟩";
				it++;
			}
			else if(i[j] == WALL)
			{
				std::cout << "🔳";
			}
			else if(i[j] == PATH)
			{
				std::cout << "⬜️";
			}
			pos++;
		}
		std::cout << std::endl;
	}
	return 0;
}