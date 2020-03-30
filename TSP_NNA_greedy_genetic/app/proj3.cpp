#include "proj3.hpp"
#include <vector>
#include <queue> // std::priority_queue lives here. 
#include <iostream>
//Function for finding the next node using NNA
unsigned next_node_Nna(const std::vector<std::vector<unsigned>>& graph, const std::vector<bool>& visited_nodes,unsigned start) {
    //to keep minimum cost
    int min_cost = INT_MAX;
    //to store the index of minimum
    unsigned min_index = UINT_MAX;
    
    for(unsigned i = 0; i < (unsigned)graph.size(); i++) 
    {
        if(!visited_nodes[i] && graph[start][i] != 0) 
        {
            if(graph[start][i] < min_cost) 
            {
                min_cost = graph[start][i];
         
                min_index = i;
            }
        }
    }

    return min_index;
}


std::vector<unsigned> TSP_via_NNA(const std::vector< std::vector<unsigned> > & graph, unsigned start)
{

    std::vector<unsigned> journey;
    
    int n = graph.size();
    //to store the list of visited nodes
    std::vector<bool> visited_nodes(n, false);
    
    int current_node = start;
    //adding the start to journey
    journey.push_back(start);
    
    visited_nodes[start] = true;

    for(int i = 0 ; i < n; i++) 
    {   
       unsigned next_node = next_node_Nna(graph, visited_nodes,current_node);//calling next_node_Nna to get the next node
       
       if(next_node == UINT_MAX)//if last node
           break;
       
       visited_nodes[next_node] = true;
       
       journey.push_back(next_node);
       //shifting to the next least node
       current_node = next_node;
    
    }
	
    return journey;
}


std::vector<unsigned> TSP_via_greedyEdge(const std::vector< std::vector<unsigned> > & graph)
{
	std::vector<unsigned> journey;
    
    unsigned n = graph.size();
    //to store the degree of nodes
    std::vector<unsigned> degree_nodes(n, 0);
    //to make the adjacency matrix of the TSP
    std::vector<unsigned> adjacency_matrix(n, UINT_MAX);
    //using priority queue for primes
    std::priority_queue<Edge> queue_for_prims;

    for(unsigned i = 0 ; i < n; i++) 
    {
        for(unsigned j = i + 1 ; j < n ; j++) 
        {
           
           Edge e(i, j, graph[i][j]);//initializing the edge
           queue_for_prims.push(e);
        
        }
    }
    //looping until queue is empty
    //constructing adjacency matrix for the TSP using greedyEdge
    while(!queue_for_prims.empty()) 
    {
        
        Edge e = queue_for_prims.top();
        
        queue_for_prims.pop();
        
        if(degree_nodes[e.pt1] >= 2 || degree_nodes[e.pt2] >= 2) 
            continue;

        if(adjacency_matrix[e.pt1] == UINT_MAX) 
        {
            adjacency_matrix[e.pt1] = e.pt2;
        } 
        else 
        {
            adjacency_matrix[e.pt2] = e.pt1;
        }
        
        degree_nodes[e.pt1]++;//incrementing the degree of node
        
        degree_nodes[e.pt2]++;//incrementing the degree of node
    }

    int counter = 0;//using counter to loop through and add the found path to the vector journey
    
    unsigned start = 0;
    
    while(counter < n) 
    {
        
        journey.push_back(start);
        
        counter++;
        
        start = adjacency_matrix[start];
    }

    return journey; 
}


std::vector< std::vector<unsigned> > orderCrossover(const std::vector<unsigned> & j1, const std::vector<unsigned> & j2, 
														unsigned pickStart, unsigned pickLen)
{
	std::vector< std::vector<unsigned> > children; // (2, std::vector<unsigned>());

	// I found it easier to start with the child of the right size
	// and use indexing to place the right elements.
	// Feel free to do something different.
    
    std::vector<unsigned> first;//the first crossover child
	std::vector<unsigned> second;//the second crossover child
    std::unordered_set<unsigned> first_template;//template for the first child
    std::unordered_set<unsigned> second_template;//template for the second child
    //initializing the length of gene which needs to be inherited
    unsigned gene_length = pickLen;
    //creating the templates of both children
    for(unsigned i = pickStart ; i < pickStart + pickLen; i++) {
        first_template.insert(j1[i]);
        second_template.insert(j2[i]);
    }
    //starting for second child
    unsigned counter_j1 = 0;
    unsigned iterator_j1 = pickStart;
    
    gene_length = pickStart;
    //iterating through the first gene to get the second child
    while(counter_j1 < (unsigned)j1.size()) {
        if(gene_length == 0) {
            for(unsigned i = pickStart; i < pickStart + pickLen; i++) {
                second.push_back(j2[i]);//inserting the part which is inherited
            }
            gene_length = -1;//to avoid coming back into the loop
        }
        //adding the part of first which is crossovered to second
        if(second_template.find(j1[iterator_j1]) == second_template.end()) {
            second.push_back(j1[iterator_j1]);
            gene_length--;
        }

        iterator_j1 = (iterator_j1+1)%(j1.size());//incrementing the iterator, modulus so that it reaches the first index too which is not yet checked
        counter_j1++;//incrementing the counter
    }

    //starting for first child
    unsigned counter_j2 = 0;
    unsigned iterator_j2 = pickStart;
    
    gene_length = pickStart;
    //iterating through the first gene to get the second child
    while(counter_j2 < (unsigned)j2.size()) {
        if(gene_length == 0) {
            for(unsigned i = pickStart; i < pickStart + pickLen; i++) {
                first.push_back(j1[i]);//inserting the part which is inherited
            }
            gene_length = -1;//to avoid coming back into the loop
        }
        //adding the part of first which is crossovered to second
        if(first_template.find(j2[iterator_j2]) == first_template.end()) {
            first.push_back(j2[iterator_j2]);
            gene_length--;
        }
        
        iterator_j2 = (iterator_j2+1)%(j2.size());//incrementing the iterator, modulus so that it reaches the first index too which is not yet checked
        counter_j2++;//incrementing the counter
    }
	children.push_back(first);//adding the first child to children vector
	
    children.push_back(second);//adding the second child to children vector


	return children;
}

std::vector<unsigned> mutation(const std::vector<unsigned> & journey, unsigned mutateStart, unsigned mutateLength)
{
	// This starts with result as a copy of the initial journey.
	// As always, feel free to start differently (as long as your result is correct)
	std::vector<unsigned> result(journey);
    std::reverse(result.begin() + mutateStart, result.begin() + mutateStart + mutateLength);//reversing the part of journey from mutateStart to mutateStart+mutationlength
	return result; 
}

//function to update the graph using union both for j1 and j2
std::vector<std::vector<unsigned>> update_newgraph(std::vector<std::vector<unsigned>>& newgraph, const std::vector< std::vector<unsigned> > & graph, const std::vector<unsigned> & j){
    //takes the updated graph and adds all the paths of original to the new graph
	for(unsigned i = 0 ; i < (unsigned)j.size() -1; i++) {
        newgraph[j[i]][j[i+1]] = graph[j[i]][j[i+1]];
    }

    return newgraph;
}



std::vector<unsigned> unionGraphReproduce(const std::vector<unsigned> & j1, const std::vector<unsigned> & j2, 
										const std::vector< std::vector<unsigned> > & graph, unsigned start, unsigned numFromUnion)
{

	std::vector< unsigned> journey;
    std::vector<bool> visited_nodes(graph.size(), false);//initializing all nodes as not visited
    int n = graph.size();
    std::vector<std::vector<unsigned>> newgraph(n, std::vector<unsigned>(n, 0));

    newgraph = update_newgraph(newgraph,graph,j1);
    newgraph[j1[j1.size() - 1]][j1[0]] = graph[j1[j1.size()-1]][j1[0]];//adding the final to first edge

    newgraph = update_newgraph(newgraph,graph,j2);
    newgraph[j2[j2.size() - 1]][j2[0]] = graph[j2[j2.size()-1]][j2[0]];//adding the final to first edge

    visited_nodes[start] = true;
    journey.push_back(start);

    //using NNA for the newgraph till numFromUnion to calculate next node and adding it to journey
    for(unsigned i = 0 ; i < numFromUnion; i++) {
        unsigned next_node = next_node_Nna(newgraph,visited_nodes,start);
        if(next_node == UINT_MAX) {
            next_node = next_node_Nna(graph, visited_nodes,start);
        }

        journey.push_back(next_node);
        visited_nodes[next_node] = true;
        start = next_node;
    }

    //adding the rest from the original graph
    for(unsigned i = numFromUnion; i < graph.size(); i++) {
        unsigned next_node = next_node_Nna(graph, visited_nodes,start);
        if(next_node == UINT_MAX) 
          break;
        journey.push_back(next_node);
        
        start = next_node;
        visited_nodes[next_node] = true;
    }

	return journey; 

}

