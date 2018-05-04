#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <stdio.h>
#include <algorithm>

using std::vector;
using namespace std;

class Edge{
	private:
		vector<int> node1;
		vector<int> node2;
		int flow;
		int cap;
	public:
		Edge(int i1, int j1, int i2, int j2, int value){
			node1.push_back(i1);
			node1.push_back(j1);
			node2.push_back(i2);
			node2.push_back(j2);
			cap=value;
			flow=0;
		}
		Edge() { cap = -1; }
		int getCap() { return cap; }

		vector<int> getNode1() { return node1; }
		vector<int> getNode2() { return node2; }

		int getFlow() { return flow; }
		void setFlow(int i) { flow=i; }
};

class Vertice{
	private:
		vector<Edge*> edges;
		Edge* pred;
		int currentDfs;
		int positionSource;
	
	public:
		Vertice(int i, int j){
			currentDfs = -1;
			positionSource = -1;
			if(i == -1)
				pred = new Edge(-1,-1,0,0,-1);
			else
				pred = new Edge(0,0,0,0,-1);
			if(i != -1 && j != -1)
				edges.resize(5);//0-sink;1-esquerda;2-direita;3-cima;4-baixo
		}

		int getDfs() { return currentDfs; }
		void setDfs(int i) { currentDfs = i; }

		Edge* getPred() { return pred; }
		void setPred(Edge* p) { pred=p; }

		void addEdge(int i, Edge* edge) { edges[i] = edge; }
		void addSourceEdge(Edge* edge) { edges.push_back(edge); }

		vector<Edge*> getEdges() { return edges; }
		int getEdgesSize() { return edges.size(); }

		void setEdgeFlow(int pos, int valor) { edges[pos]->setFlow(edges[pos]->getFlow() + valor); }
		
		int getPositionSource() { return positionSource; }
		void setPositionSource(int i) { positionSource = i; }
};

class Grafo{
	private:
		vector<vector<Vertice> > matrix_vertices;
		int linhas;
		int colunas;
		vector<Vertice>  SourceSink;
		int totalCost;
	public:
		Grafo(int m, int n){
			totalCost = 0;
			matrix_vertices.resize(m);
			linhas=m;
			colunas=n;
			SourceSink.push_back(Vertice(-1,0));
			SourceSink.push_back(Vertice(0,-1));
		}
		int getPostionSource(int i, int j) { return matrix_vertices[i][j].getPositionSource(); }

		void addVertice(int i, int j){ matrix_vertices[i].push_back(Vertice(i,j)); }

		void addEdge(int i1, int j1, int i2, int j2, int value, int pos){
			if(i1 == -1){
				SourceSink[0].addSourceEdge(new Edge(i1,j1,i2,j2,value));
				matrix_vertices[i2][j2].setPositionSource(SourceSink[0].getEdgesSize()-1);
			}
			else
				matrix_vertices[i1][j1].addEdge(pos,new Edge(i1,j1,i2,j2,value));
		}
		int getLines(){return linhas;}
		int getColumns(){return colunas;}
		bool checkPredVertice(int i, int j, int currentDfs){
			if(i == -1)
				return SourceSink[0].getDfs() == currentDfs;
			if(j == -1)
				return SourceSink[1].getDfs() == currentDfs;
			else
				return matrix_vertices[i][j].getDfs() == currentDfs;
		}
		void setPredVertice(int i, int j, Edge* pred){
			if(i == -1)
				SourceSink[0].setPred(pred);
			else if(j == -1)
				SourceSink[1].setPred(pred);
			else
				matrix_vertices[i][j].setPred(pred);
		}
		Edge* getPredVertice(int i, int j){
			if(i == -1)
				return SourceSink[0].getPred();
			else if(j == -1)
				return SourceSink[1].getPred();
			else
				return matrix_vertices[i][j].getPred();
		}
		vector<Edge*> getEdgesVertice(int i, int j){
			if(i == -1)
				return SourceSink[0].getEdges();
			else if(j == -1)
				return SourceSink[1].getEdges();
			else
				return matrix_vertices[i][j].getEdges();
		}

		void setEdgeFlow(int pos, int valor, int i, int j){
			if(i == -1){
				SourceSink[0].setEdgeFlow(j, valor);
			}else{
				matrix_vertices[i][j].setEdgeFlow(pos, valor);
			}
		}

		void addTotalValue(int value){totalCost += value;}
		int getTotalValue(){return totalCost;}

		void free(){
			for(int i=0;i<linhas;i++){
				matrix_vertices[i].clear();
			}
			matrix_vertices.clear();
		}
		int getDfsVertice(int i, int j){
			if(i == -1)
				return SourceSink[0].getDfs();
			else if(j == -1)
				return SourceSink[1].getDfs();
				else
					return matrix_vertices[i][j].getDfs();
		}
		void setDfsVertice(int i, int j, int value){
			if(i == -1)
				SourceSink[0].setDfs(value);
			else if(j == -1)
				SourceSink[1].setDfs(value);
			else
				matrix_vertices[i][j].setDfs(value);
		}
};

int Edmond_karp_algorithmn(Grafo *g){
	int flow = 0;
	
	int currentDfs = 0;

	do{//retorna false sse tiver predecessor
		currentDfs++;
		//i*NumColunas + j
		int pos = -1;

		queue<int> q;
		q.push(pos);

		int flag=0;

		while(not q.empty()){
			int cur = q.front();
			q.pop();
			int i,j;
			if(cur == -1){
				i=-1;
				j=0;
			}else{
				i = cur/g->getColumns();
				j = cur%g->getColumns();
			}

			vector<Edge*> edges = g->getEdgesVertice(i, j);
			
			int tamanho_edges = edges.size();
			for(int i=0;i<tamanho_edges;i++){
				Edge* e = edges[i];
				if(e != NULL){
					if(e->getCap() != -1){
						vector<int> node = e->getNode2();

						if(g->getDfsVertice(node[0],node[1]) != currentDfs && e->getCap() > e->getFlow()){	
							q.push(node[0]*g->getColumns() + node[1]);
							g->setDfsVertice(node[0], node[1], currentDfs);
							g->setPredVertice(node[0], node[1], e);

							if(node[1] == -1){
								flag = 1;
								break;
							}
						}
					}
				}
			}
			if(flag)
				break;
		}

		if(g->checkPredVertice(0,-1,currentDfs)){

			int df = -1;
			for(Edge* node = g->getPredVertice(0,-1);node->getNode1()[1] != -1 ;node = g->getPredVertice(node->getNode1()[0],node->getNode1()[1])) {
				int count = node->getCap() - node->getFlow();

				if(df == -1)
					df = count;

				if(df > count)
					df = count;
			}

			for(Edge* node = g->getPredVertice(0,-1);node->getNode1()[1] != -1;node = g->getPredVertice(node->getNode1()[0],node->getNode1()[1])) {
				/*CODE*/ /* 0-para sink */ /* 1-para esquerda */ /* 2-para direita */ /* 3-para cima */ /* 4-para baixo */
				vector<int> node1 = node->getNode1();
				vector<int> node2 = node->getNode2();
				
				//node.setFlow(df);


				
				if(node2[1] == -1){//para o sink
					g->setEdgeFlow(0, df, node1[0], node1[1]);
				}else{
					if(node1[0] == node2[0]){//mesma linha
						if(node1[1] < node2[1]){//direita
							//g->setEdgeFlow(1, df, node2[0], node2[1]);
							g->setEdgeFlow(2, df, node1[0], node1[1]);
						}else{//esquerda
							//g->setEdgeFlow(2, df, node2[0], node2[1]);
							g->setEdgeFlow(1, df, node1[0], node1[1]);
						}
					}else{
						if(node1[0] == -1){//do source
							g->setEdgeFlow(1, df, -1, g->getPostionSource(node2[0],node2[1]));
						}else{//mesma coluna
							if(node1[0] < node2[0]){//baixo
								//g->setEdgeFlow(3, df, node2[0], node2[1]);
								g->setEdgeFlow(4, df, node1[0], node1[1]);
							}else{//esquerda
								//g->setEdgeFlow(4, df, node2[0], node2[1]);
								g->setEdgeFlow(3, df, node1[0], node1[1]);
							}
						}
					}
				}
			}
			flow = flow + df;			
		}

	}while(g->checkPredVertice(0,-1,currentDfs));
	g->addTotalValue(flow);
	return currentDfs;
}

void printOutput(Grafo *g, int aux){
	cout << g->getTotalValue() << endl << endl;
	int lines = g->getLines();
	int columns = g->getColumns();
	for(int i=0; i<lines; i++){
		for(int j=0; j<columns; j++){
			if(g->getDfsVertice(i,j) == aux){
				cout << "C ";
			}else{
				cout << "P ";
			}
		}
		cout << endl;
	}
}
int main(){
	int m,n,value;
	scanf("%d %d\n",&m,&n);

	Grafo *g = new Grafo(m,n);
	vector<vector<int> > tmp_matrix;
	tmp_matrix.resize(m);
	
	for(int i=0;i<m;i++){
		for(int j=0;j<n;j++){
			scanf("%d ",&value);
			g->addVertice(i,j);
			tmp_matrix[i].push_back(value);
		}
	}

	for(int i=0;i<m;i++){
		for(int j=0;j<n;j++){
			scanf("%d ",&value);
			int valor_preto = tmp_matrix[i][j];
			
			if(value == 0){//nao ha (i,j) -> sink
				if(valor_preto == 0)// nao ha source -> (i,j)
					continue;
				else// ha apenas source -> (i,j)
					g->addEdge(-1,0,i,j,valor_preto,0);//ultimo 0 nao interessa
			}else{// ha (i,j) -> sink
				if(valor_preto == 0)// nao ha source -> (i,j)
					g->addEdge(i,j,0,-1,value,0);
				else{// ha ambos os caminhos
					if(valor_preto == value){// Ex: 5 e 5, sabemos que um caminho de aumento tem custo 5, logo nao sao preciso estas edges.
						g->addTotalValue(value);
					}
					else{
						if(valor_preto > value){// Ex: 8 e 5, sabemos que um caminho de aumento tem custo 5, logo basta (8-5) e 0 = 3 e 0.
							g->addTotalValue(value);
							g->addEdge(-1,0,i,j,valor_preto - value,0);//ultimo 0 nao interessa
						}
						else{// Ex: 5 e 8, sabemos que um caminho de aumento tem custo 5, logo basta 0 e (8-5) = 0 e 3.
							g->addTotalValue(valor_preto);
							g->addEdge(i,j,0,-1,value - valor_preto,0);
						}
					}
				}
			}
		}
	}
	//free memory
	tmp_matrix.clear();

	for(int i=0;i<m;i++){
		for(int j=0;j<n-1;j++){
			scanf("%d ",&value);
			if(value == 0)
				continue;
			g->addEdge(i,j+1,i,j,value,1);
			g->addEdge(i,j,i,j+1,value,2);
		}
		scanf("\n");
	}
	scanf("\n");
	for(int i=0;i<m-1;i++){
		for(int j=0;j<n;j++){
			scanf("%d ",&value);
			if(value == 0)
				continue;
			g->addEdge(i+1,j,i,j,value,3);
			g->addEdge(i,j,i+1,j,value,4);
		}
		scanf("\n");
	}
	int aux = Edmond_karp_algorithmn(g);

	printOutput(g,aux);

	g->free();
	return 0;
}