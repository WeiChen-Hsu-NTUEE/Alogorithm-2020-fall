#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<algorithm>

using namespace std;

void help_message() {
    cout << "how to compile: make" << '\n';
    cout << "how to run: " << '\n'
    << "./bin/cb inputs/public_case_1.in outputs/public_case_1.out" << '\n'
    << "./bin/cb inputs/public_case_2.in outputs/public_case_2.out" << '\n'
    << "./bin/cb inputs/public_case_3.in outputs/public_case_3.out" << '\n'
    << "./bin/cb inputs/public_case_4.in outputs/public_case_4.out" << '\n'
    << "./bin/cb inputs/public_case_5.in outputs/public_case_5.out" << '\n'
    << "./bin/cb inputs/public_case_7.in outputs/public_case_7.out" << '\n'
    << "./bin/cb inputs/public_case_8.in outputs/public_case_8.out" << '\n';
    cout << "how to clean: make clean" << '\n';
}

//======class======//

class Edge {
public:
    //===variable===//
    int from_vertex, to_vertex, weight;

    //===function===//
    Edge() {};
    Edge(int u, int v, int w) {
        from_vertex = u;
        to_vertex = v;
        weight = w;
    }
    ~Edge() {};
};

class Vertex {
public:
    //===variable===//
    int index, rank, color;
    Vertex *root;
    vector<Vertex*> adj; // store each v if find an edge from this vertex to v

    //===function===//
    Vertex() {};
    Vertex(int v) {
        index = v;
        root = this;
        rank = 0;
        color = 0;
    }
    ~Vertex() {};
};

//======function declaration======//

Vertex* find_set(Vertex *u);
void union_sets (Vertex *u, Vertex *v);
bool sort_descending_compare(Edge e1, Edge e2);
bool dfs_detect_cycle(vector<Vertex*> &vertex, Vertex *v);
bool detect_cycle (vector<Vertex*> &vertex, Vertex *u, Vertex *v);
void undirected_cb(int num_edge, int num_vertex, vector<Edge> &edge, vector<Vertex*> &vertex, vector<Edge> &removed_edge, int *cost);
void directed_cb(int num_edge, int num_vertex, vector<Edge> &edge, vector<Vertex*> &vertex, vector<Edge> &removed_edge, int *cost);

//======main function======//

int main(int argc, char *argv[]) {

    //===help message===//

    if(argc != 3) {
       help_message();
       return 0;
    }

    //===read file===//

    fstream fin(argv[1]);
    fstream fout;
    fout.open(argv[2], ios::out);

    string graph_type;
    fin >> graph_type;
    int num_vertex, num_edge;
    fin >> num_vertex >> num_edge;

    vector<Edge> edge; // store all the edges
    for (int i = 0; i < num_edge; i++) {
        int u, v, w;
        fin >> u >> v >> w;
        Edge new_edge(u, v, w);
        edge.push_back(new_edge);
    }

    vector<Vertex*> vertex; // store all the vertices
    for (int i = 0; i < num_vertex; i++) {
        Vertex *new_vertex = new Vertex(i);
        vertex.push_back(new_vertex);
    }

    sort(edge.begin(), edge.end(), sort_descending_compare);

    vector<Edge> removed_edge; // store the edge that should be removed
    int cost = 0;

    //===undirected===//

    if (graph_type == "u") {
        undirected_cb(num_edge, num_vertex, edge, vertex, removed_edge, &cost);
    }

    //===directed===//

    if (graph_type == "d") {
        directed_cb(num_edge, num_vertex, edge, vertex, removed_edge, &cost);
    }

    //===write the result===//

    if (cost == 0) {
        fout << cost;
    }
    if (cost != 0) {
        fout << cost << '\n';
        for (int i = 0; i < removed_edge.size() - 1; i++) {
        fout << removed_edge[i].from_vertex << " " << removed_edge[i].to_vertex << " " << removed_edge[i].weight << '\n';
        }
        fout << removed_edge[removed_edge.size() - 1].from_vertex << " " << removed_edge[removed_edge.size() - 1].to_vertex << " " << removed_edge[removed_edge.size() - 1].weight;
    }
    fin.close();
    fout.close();
    return 0;
}

//======function======//

bool sort_descending_compare(Edge e1, Edge e2) {
    return e1.weight > e2.weight; // cannot use >=, or case_3 would have segmentation fault
}

Vertex* find_set(Vertex *u) {
    if (u != u->root) {
        u->root = find_set(u->root); // find the root + path compression
    }
    return u->root;
}

void union_sets (Vertex *u, Vertex *v) {
    if (u->rank > v->rank) {
        v->root = u;
    }
    else {
        u->root = v;
        if (u->rank == v->rank) {
            v->rank++;
        }
    }
}

void undirected_cb(int num_edge, int num_vertex, vector<Edge> &edge, vector<Vertex*> &vertex, vector<Edge> &removed_edge, int *cost) {
    for (int i = 0; i < num_edge; i++) {
        Vertex *u = vertex[edge[i].from_vertex];
        Vertex *v = vertex[edge[i].to_vertex];
        if (find_set(u) != find_set(v)) {
            union_sets(find_set(u), find_set(v));
        }
        else {
            *cost += edge[i].weight;
            removed_edge.push_back(edge[i]);
        }
    }
}

void directed_cb(int num_edge, int num_vertex, vector<Edge> &edge, vector<Vertex*> &vertex, vector<Edge> &removed_edge, int *cost) {
    for (int i = 0; i < num_edge; i++) {
        Vertex *u = vertex[edge[i].from_vertex];
        Vertex *v = vertex[edge[i].to_vertex];
        if (find_set(u) != find_set(v)) { // not even belong to the same set => definitely no cycle
            union_sets(find_set(u), find_set(v));
            u->adj.push_back(v); // we need this adlList, since u and v in the same set not necessarily means (u, v) in directed graph
        }
        else {
            if (edge[i].weight >= 0 && detect_cycle(vertex, u, v) == 0) { // edge weight >= 0 and no cycle
                u->adj.push_back(v);
            }
            else {
                *cost += edge[i].weight;
                removed_edge.push_back(edge[i]);
            }
        }
    }
}

bool detect_cycle (vector<Vertex*> &vertex, Vertex *u, Vertex *v) {
    u->color = 1; // 1 stands for already traversed
    //somehow, v->color = 1; cannot be put here, or the case_8 would have bad::alloc
    bool is_cycle = dfs_detect_cycle(vertex, v); // 1 stands for there are some cycles exist
    for (int i = 0; i < vertex.size(); i++) {
        vertex[i]->color = 0;
    }
    return is_cycle;
}

bool dfs_detect_cycle(vector<Vertex*> &vertex, Vertex *v) {
    v->color = 1;
    for (int i = 0; i < v->adj.size(); i++) {
        if (v->adj[i]->color == 1) { // color = 1, v->adj[i] is already traversed
            return true;
        }
        else if (v->adj[i]->color == 0) { // not traversed before, continue to find if there is a vertex with color == 1
            if (dfs_detect_cycle(vertex, v->adj[i]) == true) {
                return true;
            }
        }
    }
    v->color = 2; // 2 stands for explored
    return false;
}