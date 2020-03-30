#include "proj1.hpp"
#include <iostream> // for debug purposes, see project description for a suggestion.
#include <limits> // for UINT_MAX
#include "proj1.hpp"
#include <iostream> // for debug purposes, see project description for a suggestion.
#include <limits> // for UINT_MAX

// Solve TSP exactly, using dynamic programming.
// The grading script will use this exact function signature, so do not change it!
// The graph is given as an adjacency matrix, HOWEVER, no edges have a cost of '0'.
// The value graph[i][j] tells you the cost of an edge from i TO j.  This graph is directed.
//      However, if graph[i][j]==0, this means there is no edge from i to j, instead of having one of that cost.
std::vector<unsigned> tsp_dynamic_program(const std::vector<std::vector<long>> & graph, unsigned n)
{
    std::vector<unsigned> journey;


    // fix journey to start at 0.
    // This code segment assumes that the vector has a 0 somewhere
    //      and "rotates" the journey so that the zero is at the front.
    // Feel free to use, change, disregard, or delete as you see fit.
    // It is commented out right now because journey is empty

    long maskNum = 1<<n;
    maskNum--;

    std::cout<<maskNum<<std::endl;
    // maintains the min cost
    std::vector<std::vector<long>> dp(n, std::vector<long>(maskNum + 1, LONG_MAX));
    //maintains the next city to visit
    std::vector<std::vector<int>> pathDp(n, std::vector<int>(maskNum + 1, -1));

    dp[0][1] = LONG_MAX;


      for(int i=1;i< n;i++)
      {
          if(graph[i][0] != 0)
              dp[i][maskNum] = graph[i][0];
      }

    // For every mask value that is possible
 for(int i = maskNum;i >= 1;i--)
    {
        for(int j = 1; j < n; j++)
        {
            if( ((i & (1 << j)) != 0))
                continue;

            int jMask = i | (1 << j);
            for(int k = 1; k< n; k++)
            {
                if(graph[j][k] == 0 || (i & (1 << k)) != 0) continue;

                int newMask = jMask | (1 << k);
                long cost = LONG_MAX;

                if (dp[k][newMask] == LONG_MAX)
                    cost = LONG_MAX;
                else
                    cost = dp[k][newMask] + graph[j][k];

                if(dp[j][jMask] > cost)
                {
                    dp[j][jMask] = cost;
                    pathDp[j][jMask] = k;
                }
            }
        }
    }

    int temp = 1;
    for(int i = 1;i < n; i++)
    {
        temp = (1 | (1 << i));
        if(graph[0][i] != 0 && dp[i][temp] != LONG_MAX)
        {
            if(dp[0][1] > dp[i][temp] + graph[0][i])
            {
                dp[0][1] = dp[i][temp] + graph[0][i];
                pathDp[0][1] = i;
            }
        }
    }

   //start of journey
    journey.push_back(0);
    findPath(n,1,journey,pathDp);

    return journey;
}

void findPath(int n,int startMask,std::vector<unsigned> &journey,std::vector<std::vector<int>> &pathDp)
{
    int count = 1;
    int start = 0;
    while(count != n)
    {
        int city = pathDp[start][startMask];
        journey.push_back(city);
        startMask = startMask | (1 << city);

        start = city;
        count++;
    }

    return;
}
/*
// Solve TSP exactly, using dynamic programming.
// The grading script will use this exact function signature, so do not change it!
// The graph is given as an adjacency matrix, HOWEVER, no edges have a cost of '0'.
// The value graph[i][j] tells you the cost of an edge from i TO j.  This graph is directed.
// 		However, if graph[i][j]==0, this means there is no edge from i to j, instead of having one of that cost.
std::vector<unsigned> tsp_dynamic_program(const std::vector<std::vector<long>> & graph, unsigned n)
{
	std::vector<unsigned> journey; 

	// fix journey to start at 0.
	// This code segment assumes that the vector has a 0 somewhere
	//		and "rotates" the journey so that the zero is at the front.
	// Feel free to use, change, disregard, or delete as you see fit.
	// It is commented out right now because journey is empty.

	while( journey[0] != 0 )
	{
		journey.push_back(journey[0]);
		journey.erase( journey.begin() );
	}

	return journey; 
}


// Returns the cost of the journey described here.
long costOfJourney(const std::vector<std::vector<long>> & graph, const std::vector<unsigned> & journey)
{
	return 5; // not the answer.
}
*/
