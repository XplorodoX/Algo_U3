#include <iostream>
#include <string>
using namespace std;

#include "graph.h"

// Knotentyp.
using V = string;

// Feld mit Testgraphen.
// (Für eigene Tests können beliebige weitere Graphen hinzugefügt werden.)
// Damit Graphen der Typen Graph<V> und WeightedGraph<V> im selben Feld
// gespeichert werden können, werden Zeiger auf den Basistyp Graph<V>
// gespeichert, die bei Bedarf in Zeiger auf WeightedGraph<V>
// umgewandelt werden.
Graph<V>* graphs [] = {
        // Beispiel eines ungewichteten Graphen.
        new Graph<string>({
                                  { "A", { "B", "C" } },	// Knoten A hat Nachfolger B und C.
                                  { "B", { } },		// Knoten B hat keine Nachfolger.
                                  { "C", { "D" } },	// Knoten C hat sich selbst als Nachfolger.
                                  { "D", { "E" } },	// Knoten D hat Nachfolger E.
                                  { "E", { } }		// Knoten E hat keine Nachfolger.
                          }),

        // Beispiel eines gewichteten Graphen.
        new WeightedGraph<string>({
                                          { "A", { { "B", 2 }, { "C", 3 } } },
                                          // Kanten A -> B mit Gewicht 2 und A -> C mit Gewicht 3.
                                          { "B", { } },		// Keine Kanten B -> ...
                                          { "C", { { "C", 4 } } }	// Kante C -> C mit Gewicht 4.
                                  }),
        new WeightedGraph<string>({
                                          { "A", { { "B", 3 }, { "E", 1 }, { "F", 5 } } },
                                          // Kanten A -> B mit Gewicht 2 und A -> C mit Gewicht 3.
                                          { "B", { { "A", 3}, { "C",8}, { "D", 7 }, { "E", 2 }, { "F", 5 }} },        // Keine Kanten B -> ...
                                          { "C", { { "B", 8 }, { "D", 5 }, { "E", 7 } } },    // Kante C -> C mit Gewicht 4.
                                          { "D", { { "B", 7 }, { "C", 5 }, { "E", 8 } } },
                                          { "E", { { "A", 1 }, { "B", 2 }, { "C", 7 }, { "D", 8 }, { "F", 4 } } },
                                          { "F", { { "A", 5 }, { "B", 5 }, { "E", 4 } } },
                                  }),
};

// Weg vom Startknoten s zum Knoten v anhand der Vorgängerinformation
// in res ausgeben.
void path (V s, V v, Pred<V>& res) {
    if (s != v && res.pred[v] != res.NIL) {
        path(s, res.pred[v], res);
        cout << " -> ";
    }
    cout << v;
}

// Hauptprogramm.
// Auswahl des Algorithmus durch das erste Kommandozeilenargument:
// bfs -> breadth first search
// dfs -> depth first search
// sort -> topological sort
// scc -> strongly connected components
// prim -> Prim
// bell -> Bellman-Ford
// dijk -> Dijkstra
// Auswahl des Testgraphen durch das zweite Kommandozeilenargument.
// (Bei den Algorithmen prim, bell und dijk muss ein gewichteter
// Graph ausgewählt werden.)
// Auswahl des Startknotens durch das optionale dritte
// Kommandozeilenargument (Standardwert ist "A").
int main (int argc, char* argv []) {
    // Kommandozeilenargumente.
    //string a = argv[1];				// Algorithmus.
    string a = argv[1];				// Algorithmus.
    Graph<V>* g = graphs[stoi( argv[2])];	// Graph.
    V s = argc > 3 ? argv[3] : "A";		// Startknoten.

    // Gewünschten Algorithmus ausführen und sein Ergebnis ausgeben.
    if (a == "bfs") {
        BFS<V> res;
        bfs(*g, s, res);
        for (V v : g->vertices()) {
            path(s, v, res);
            uint d = res.dist[v];
            if (d == res.INF) cout << " inf" << endl;
            else cout << " " << d << endl;
        }
    }
    else if (a == "dfs") {
        DFS<V> res;
        dfs(*g, res);
        for (V v : res.seq) {
            cout << v << " " << res.det[v] << " " << res.fin[v] << endl;
        }
    }
    else if (a == "sort") {
        list<V> res;
        if (topsort(*g, res)) {
            for (V v : res) cout << v << endl;
        }
        else {
            cout << "cycle" << endl;
        }
    }
    else if (a == "scc") {
        list<list<V>> res;
        scc(*g, res);
        for (list<V> c : res) {
            c.sort();
            for (V v : c) cout << v << " ";
            cout << endl;
        }
    }
    else if (a == "prim") {
        Pred<V> res;
        prim(*(WeightedGraph<V>*)g, s, res);
        for (V v : g->vertices()) {
            path(s, v, res);
            cout << endl;
        }
    }
    else if (a == "bell") {
        SP<V> res;
        if (bellmanFord(*(WeightedGraph<V>*)g, s, res)) {
            for (V v : g->vertices()) {
                path(s, v, res);
                cout << " " << res.dist[v] << endl;
            }
        }
        else {
            cout << "negative cycle" << endl;
        }
    }
    else if (a == "dijk") {
        SP<V> res;
        dijkstra(*(WeightedGraph<V>*)g, s, res);
        for (V v : g->vertices()) {
            path(s, v, res);
            cout << " " << res.dist[v] << endl;
        }
    }
    else {
        cout << "unknown algorithm: " << a << endl;
    }
}
