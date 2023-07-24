/* just a test graph */
#include <iostream>
#include <list>
#include <vector>
#include <utility>

class graph
{
	private:
	size_t size;
	std::vector<std::list<std::pair<int,int>>> g;
	public:
	graph(std::size_t _size)
	:	g{_size,std::list<std::pair<int,int>>()}
	,	size(_size)
	{}
	void add(int src, int dest, int weight = 1)
	{
		g[src].emplace_back(std::make_pair(dest,weight));
		g[dest].emplace_back(std::make_pair(src,weight));
		g[src].sort();
		g[dest].sort();

	}
	void print()
	{
		int index = 0;
		for(auto& i : g)
		{
			std::cout << index++ << " | ";
			if(i.empty())
				continue;
			for(auto& j : i)
			{
				
				std::cout << j.first << " ";
			}
			std::cout << std::endl;
		}
	}
	void printGraph()
	{

		for(int i = 0; i < size; i++)
		{
			std::list<std::pair<int,int>>::iterator cu = g[i].begin();
			for(int j = 0; j < size; j++)
			{
				if(cu->first == j)
				{
					// std::cout << cu->second << " ";
					std::cout << "🟪";

					cu++;
				}
				else
				{
					std::cout << "⬜️";

				}

			}
			std::cout << std::endl;
		}
	}
};


int main()
{
	graph g(5);

	g.add(0, 2);
	g.add(0, 1);
	g.add(0, 3);

	g.add(1, 2);

	g.add(2, 3);
	g.add(2, 4);

	g.add(3, 4);
	g.print();

	g.printGraph();






	return 0;
}