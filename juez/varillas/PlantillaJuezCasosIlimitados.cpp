// Jorge Rodriguez Garcia
// VJ24
// Varillas con mejora de espacio

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

const int INF = 100000000;

bool esPosible(const vector<pair<int, int>>& v, const int& L) {
	int n = v.size() - 1;
	vector<bool> varillas(L + 1, false);
	varillas[0] = true;                                                // matriz[0][0]=true

	for (int i = 1; i <= n; i++) {
		for (int j = L; j >= 1; j--) {                                 // se recorre de drch a izq para poder acceder a los elementos anteriores
			if (v[i].first <= j)                                       // si no se pasa de la longitud
				varillas[j] = varillas[j] || varillas[j - v[i].first]; // igual que cuando teniamos matriz, pero solo haciendo caso de las j
		}
	}

	return varillas[L];
}

int formasPosibles(const vector<pair<int, int>>& v, const int& L) {
	int n = v.size() - 1;
	vector<int> varillas(L + 1, 0);
	varillas[0] = 1;

	for (int i = 1; i <= n; i++) {
		for (int j = L; j >= 1; j--) {
			if (v[i].first <= j)
				varillas[j] = varillas[j] + varillas[j - v[i].first];
		}
	}

	return varillas[L];
}

int numeroMinimo(const vector<pair<int, int>>& v, const int& L) {
	int n = v.size() - 1;
	vector<int> varillas(L + 1, INF); // todo igualado a INF para calcular el minimo
	varillas[0] = 0;

	for (int i = 1; i <= n; i++) {
		for (int j = L; j >= 1; j--) {
			if (v[i].first <= j)
				varillas[j] = min(varillas[j], varillas[j - v[i].first] + 1);
		}
	}

	return varillas[L];
}

int costeMinimo(const vector<pair<int, int>>& v, const int& L) {
	int n = v.size() - 1;
	vector<int> varillas(L + 1, INF); // todo igualado a INF para calcular el minimo
	varillas[0] = 0;

	for (int i = 1; i <= n; i++) {
		for (int j = L; j >= 1; j--) {
			if (v[i].first <= j) 
				varillas[j] = min(varillas[j], varillas[j - v[i].first] + v[i].second);
		}
	}

	return varillas[L];
}

// Resuelve un caso de prueba, leyendo de la entrada la
// configuración, y escribiendo la respuesta
bool resuelveCaso() {
	int N, L;
	cin >> N >> L;
    
    if (! std::cin)
        return false;
    
	vector<pair<int, int>> v;
	v.push_back({ 0, 0 });     // NO OLVIDAR!! luego empezamos a comprobar a partir de la posicion 1
	for (int i = 0; i < N; i++) {
		int longitud, coste;
		cin >> longitud >> coste;
		v.push_back({ longitud, coste });
	}

	bool posible = esPosible(v, L);
	if (posible)cout << "SI" << " " << formasPosibles(v, L) << " " << numeroMinimo(v, L) << " " << costeMinimo(v, L) << endl;
	else cout << "NO" << endl;
    
    return true;
}

int main() {
    // Para la entrada por fichero.
    // Comentar para acepta el reto
    #ifndef DOMJUDGE
     std::ifstream in("datos.txt");
     auto cinbuf = std::cin.rdbuf(in.rdbuf()); //save old buf and redirect std::cin to casos.txt
     #endif 
    
    
    while (resuelveCaso())
        ;

    
    // Para restablecer entrada. Comentar para acepta el reto
     #ifndef DOMJUDGE // para dejar todo como estaba al principio
     std::cin.rdbuf(cinbuf);
     system("PAUSE");
     #endif
    
    return 0;
}