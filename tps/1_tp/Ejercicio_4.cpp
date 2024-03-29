#include<iostream>
#include<vector>

using namespace std;

const int INF = 1e9;

struct datos {
    int sig_i;
    int sig_k;
    int sig_u;
    datos(int i, int k, int u): sig_i(i), sig_k(k), sig_u(u) {}
};

int n, cant_prov;
vector<int> puestos;
vector<int> proveedurias;
vector<vector<vector<int> > > memo;
vector<vector<vector<datos> > > padres;

/// @param d desde que puesto calcular
/// @return el costo de los puestos que estan adelante de la proveeduria d
int costos_restantes(int d) {
    int suma = 0;
    for(int i = d + 1; i < n; i++)
        suma += abs(puestos[i] - puestos[d]);
    return suma;
}

/// @param d Desde este puesto
/// @param h Hasta este puesto
/// @return los costos minimos de las distancias de los puestsos entre las proveedurias d y h
int costos_intermedios(int d, int h) {
    int suma = 0;
    for(int i = d + 1; i < h; i++)
        suma += min(abs(puestos[i] - puestos[d]), abs(puestos[i] - puestos[h]));
    return suma;
}

/// @param i el i-esimo puesto
/// @param k la cantidad restante de proveedurias
/// @param u la posicion de la última proveeduria
/// @return el minimo costo de poner las k proveeduras
int min_costo(int i, int k, int u) {
    if (i <= n and k == cant_prov)  return costos_restantes(u);
    if (i == n and k < cant_prov)   return INF;

    if (memo[i][k][u] == -1) {
        int poner;

        if (i == 0) {
            //Fuerzo la proveeduria virtual en el -INF pero sin agregar cantidad de proveedurias
            poner = min_costo(i + 1, k, i);
        } else {
            // Pongo la proveeduria en i y calculo los costos entre i y la ultima proveeduria
            poner = costos_intermedios(u, i) + min_costo(i + 1, k + 1, i);
        }

        // Salteo la posicion i
        int saltear = i > 0 ? min_costo(i + 1, k, u) : INF;

        // Me quedo con el minimo de poner y saltear
        // En caso de empate el menor lexicograficamente va a ser poner
        if(poner <= saltear) {
            memo[i][k][u] = poner;
            if (i == 0) {
                // Si es la proveeduria virtual no quiero sumarle a la k
                padres[i][k][u] = datos(i + 1, k, i);
            } else {
                padres[i][k][u] = datos(i + 1, k + 1, i);
            }
        } else {
            memo[i][k][u]   = saltear;
            padres[i][k][u] = datos(i+1, k, u);
        }
    }

    return memo[i][k][u];
}

// Vamos a reconstruir la solucion siguiendo los pasos del padre
void reconstruir_solucion(int i, int k, int u) {
    // Para no pasarse del tamaño de los arreglos
    if (i == n or k == cant_prov or u == n) return;

    // Si entramos en un caso invalido, no hay solucion
    if (padres[i][k][u].sig_i == -1) return;

    datos padre = padres[i][k][u];

    // Si la siguiente proveeduria es distinta a la actual, la guardamos
    if (i > 0 and padre.sig_u != u) proveedurias[k] = puestos[padre.sig_u];

    reconstruir_solucion(padre.sig_i, padre.sig_k, padre.sig_u);
}

int main() {
    int c;cin >> c;
    while(c--) {
        cin >> n >> cant_prov;
        n++; // Aumneto el n para poner una proveduria 0 en el -INF

        puestos      = vector<int>(n);
        proveedurias = vector<int>(cant_prov, -1);

        memo   = vector<vector<vector<int> > >(n, 
                 vector<vector<int> >(cant_prov, 
                 vector<int>(n, -1)));

        padres = vector<vector<vector<datos> > >(n, 
                 vector<vector<datos> >(cant_prov, 
                 vector<datos>(n, datos(-1,-1,-1))));
        
        puestos[0] = -INF; // Agrego el puesto 0

        int i = 1;
        while(i < n) {
            int e; cin >> e;
            puestos[i] = e;
            i++;
        }

        int costo = min_costo(0,0, 0);
        reconstruir_solucion(0,0,0);
        
        cout << costo << endl;
        for(int j = 0; j < cant_prov; j++) cout << proveedurias[j] << " ";
        cout << endl;
    }
}
