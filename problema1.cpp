#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9;

int main() {
    int C;
    cin >> C;

    while (C--) {
        int P;
        cin >> P;

        vector<string> cidades(P);
        map<string, int> indice;

        for (int i = 0; i < P; i++) {
            cin >> cidades[i];
            indice[cidades[i]] = i;
        }

        vector<vector<int>> dist(P, vector<int>(P, INF));
        vector<vector<int>> prox(P, vector<int>(P, -1));

        for (int i = 0; i < P; i++) {
            for (int j = 0; j < P; j++) {
                int custo;
                cin >> custo;

                if (custo != -1) {
                    dist[i][j] = custo;
                    prox[i][j] = j;
                }
            }
        }

        for (int k = 0; k < P; k++) {
            for (int i = 0; i < P; i++) {
                for (int j = 0; j < P; j++) {
                    if (dist[i][k] != INF &&
                        dist[k][j] != INF &&
                        dist[i][k] + dist[k][j] < dist[i][j]) {

                        dist[i][j] = dist[i][k] + dist[k][j];
                        prox[i][j] = prox[i][k];
                    }
                }
            }
        }

        int R;
        cin >> R;

        while (R--) {
            string funcionario, origemNome, destinoNome;
            cin >> funcionario >> origemNome >> destinoNome;

            int origem = indice[origemNome];
            int destino = indice[destinoNome];

            if (dist[origem][destino] == INF) {
                cout << "Sorry Mr " << funcionario
                     << " you can not go from " << origemNome
                     << " to " << destinoNome << '\n';
            } else {
                cout << "Mr " << funcionario
                     << " to go from " << origemNome
                     << " to " << destinoNome
                     << ", you will receive "
                     << dist[origem][destino] << " euros\n";

                cout << "Path:" << cidades[origem];

                int atual = origem;

                if (origem == destino) {
                    cout << " " << cidades[destino];
                } else {
                    while (atual != destino) {
                        atual = prox[atual][destino];
                        cout << " " << cidades[atual];
                    }
                }

                cout << '\n';
            }
        }
    }

    return 0;
}