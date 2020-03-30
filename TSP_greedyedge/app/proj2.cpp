#include "proj2.hpp"
#include <queue> // This is where std::priority_queue lives.
#include <stack> // you may want std::stack for your depth-first traversal.
#include<iostream>
#define MAX_NODES 290000

using namespace std;
// Your function MAY assume that this is a complete graph (as that is part of project specification).

//struct for storing subset mappings
struct Subset
{
    unsigned parent;
    unsigned rank;
};


unsigned Find(unsigned vertex, vector<Subset> &sets)
{
    if (sets[vertex].parent != vertex)
    sets[vertex].parent = Find(sets[vertex].parent, sets);
    return sets[vertex].parent;
}

//Used to do union of the sets who have vertices
void Union(unsigned vertex1, unsigned vertex2, vector<Subset> &Set)
{
    if (Set[vertex1].rank > Set[vertex2].rank)
    {
        Set[vertex2].parent = vertex1;
        Set[vertex1].rank += Set[vertex2].rank;
     }
    else
    {
        Set[vertex1].parent = vertex2;
        Set[vertex2].rank += Set[vertex1].rank;
    }
}


vector<Edge> compute_mst(vector< vector<unsigned>> & graph)
{
    vector<Edge> mst;

    //using priority queue to align based on weights
    std::priority_queue<Edge> edge;

    unsigned n = graph.size();

    //creating edge priority queue
    for (unsigned i = 0; i < n; ++i)
    {
        for (unsigned j = i + 1; j < n; ++j)
        {
        	//creates and edge with the weight graph[i][j]
            Edge e(i, j, graph[i][j]);

            // adding the edge into queue
            edge.push(e);
        }
    }
    //storing the edge in a variable
    std::priority_queue<Edge> edgeHeap = edge;

    vector<Subset> set;//creating disjoint sets for checking cycles
    for (unsigned k = 0; k < n; ++k)
    {
        Subset s;

        // making every vertice its own parent
        s.parent = k;

        // this determines the number of nodes whose this vertex is parent to.
        s.rank = 1;
        set.push_back(s);
    }
    vector<Subset> generated_set = set;

    unsigned edgecount = 0;
    unsigned vertices = graph.size();
    while (edgecount != vertices - 1)
    {
    	//taking the minimum edge
        Edge e = edgeHeap.top();
        edgeHeap.pop();
        unsigned parent_pt1 = Find(e.pt1, generated_set);
        unsigned parent_pt2  = Find(e.pt2, generated_set);

        //checking whether vertices belong to same set
        if (parent_pt1 != parent_pt2)
        {
            mst.push_back(e);

            //doing union if vertices in different sets
            Union(parent_pt1, parent_pt2, generated_set);
            edgecount++;
        }
    }
    return mst;
}

// Returns the cost of the edges in the given vector, though it does not confirm that it is an MST.
unsigned mstCost(const vector<Edge> & vE)
{
    unsigned sum =0;
    for(auto e: vE)
    {
        sum += e.weight;
    }
    return sum;
}

unsigned costOfJourney(const vector<vector<unsigned>> & graph, const vector<unsigned> & journey)
{
    unsigned cost = 0;
    for(int j=0; j<journey.size()-1 ;j++){
        cost+= graph[journey[j]][journey[j+1]];
    }
    cost+= graph[journey[journey.size()-1]][journey[0]];
    return cost;
}

unsigned waste(const vector<Edge> & tree, const vector<unsigned> & journey, const vector<vector<unsigned>> & graph)
{
	//subtracting cost of journey with mst cost
    return costOfJourney(graph,journey)-mstCost(tree);
}

//Recusrive helper for traversal
vector<unsigned> journeyHelper(unsigned vertex, vector<unsigned>& visited, vector<vector<unsigned>>& adjacency, vector<unsigned> &temp)
{
    visited[vertex] = 1;
    temp.push_back(vertex);
    for (const auto& curVertex : adjacency[vertex])
    {
        if (!visited[curVertex])
        {
            journeyHelper(curVertex, visited, adjacency, temp);
            temp.push_back(vertex);
        }
    }
    return temp;
}

vector<unsigned> computeTSPJourney(const vector< Edge > & mst, unsigned start)
{
    cout<<"infunction compute";
    vector<unsigned> journey;
    vector<std::vector<unsigned>> adjacency(1001, std::vector<unsigned>());

    vector<unsigned> temp;

    std::vector<unsigned> visited(1001, 0);

    for (const Edge& e : mst)
    {
        std::cout<<e.pt1<<" "<<e.pt2<<std::endl;
        adjacency[e.pt1].push_back(e.pt2);
        adjacency[e.pt2].push_back(e.pt1);
    }
    temp =  journeyHelper(start, visited, adjacency, temp);
    return temp;

}

