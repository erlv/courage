// TOPCODE TCO 2013-R1-A
// 2013-5-18
// TODO: 0. Solve a min-cost max flow problem first to fully understand this
// TODO: 1. I have found out that, the result is to make some cycles in the matrics by minimize the moves.  But do not know how to get a minimum move.
// TODO: 2. what is bipartite matching?
// TODO: 3. Why this problem is a minimum cost bipartite matching problem?
// TODO: 4. why a minimum cost bipartite matching problem can be solved using
//       the min-cost max flow algorithm  
// TODO: This is only a main function of the problem. 
//       min-cost max flow solver is still needed. that is:
//       1. network::addVertex(): adds a vertex to the graph and return
//                                a unique integer id for it.
//       2. network::addEdge(x,y,capacity, cost): Adds an  edge from x to y,
//                                                with a capacity and cost.
//       3. network::minCostMaxFlow(flow,cost): finds the max flow and its
//                                              cost.
int getMinimum(vector<string> board) {
  int w = board.size(), h = board[0].size();

  network* G = new network();

  int in[w][h];
  int out[w][h];
  G->source = G->addVertex();
  G->sink = G->addVertex();
  for( int i = 0; i < w; i++) {
    for(int j = 0 ; j < h; j++) {
      in[i][j] = G->addVertex();
      out[i][j] = G->addVertex();
      G->addEdge(G->source, in[i][j], 1, 0);
      G->addEdge(out[i][j], G->sink, 1, 0);
    }
  }

  const string NAME="ULDR";
  const int dx[4] = {0, -1, 0, 1};
  const int dy[4] = {-1, 0, 1, 0};

  for( int x = 0; x < h; x++)
    for( int y=0; y < w; y++) {
      for( int d=0; d < 4; d++) {
        int nx = (x + dx[d] + h) % h;
        int ny = (y + dy[d] + w) % w;
        if(NAME[d] == board[y][x]) {
          G->addEdge( in[y][x], out[ny][nx], 1, 0);
        } else {
          G->addEdge( in[y][x], out[ny][nx], 1, 1);
        }
      }
    }

  int flow, cost;
  G->minCostMaxFlow(flow, cost);

  return (int)cost;
}
