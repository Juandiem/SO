//Jorge Rodriguez Garcia
#include <algorithm>
#include <functional>
#include <iostream>
#include <stack>
#include <stdexcept>
#include <string>
#include <utility>


template <class Clave, class Valor, class Comparador = std::less<Clave>>
class map {
public:
	// parejas <clave, valor>
	struct clave_valor {
		const Clave clave;
		Valor valor;
		clave_valor(Clave const& c, Valor const& v = Valor()) : clave(c), valor(v) {};
	};
	// using clave_valor = std::pair<const Clave, Valor>;
protected:
	using map_t = map<Clave, Valor, Comparador>;

	/*
	 Clase nodo que almacena internamente la pareja <clave, valor>,
	 punteros al hijo izquierdo y al hijo derecho, y la altura.
	 */
	struct TreeNode;
	using Link = TreeNode * ;
	struct TreeNode {
		clave_valor cv;
		Link iz, dr;
		int altura;
		int tam_i;
		TreeNode(clave_valor const& e, Link i = nullptr, Link d = nullptr,
			int alt = 1, int tam = 1) : cv(e), iz(i), dr(d), altura(alt), tam_i(tam) {}
	};

	// puntero a la ra�z de la estructura jer�rquica de nodos
	Link raiz;

	// n�mero de parejas <clave, valor>
	int nelems;

	// objeto funci�n que compara elementos
	Comparador menor;

public:

	// constructor (diccionario vac�o)
	map(Comparador c = Comparador()) : raiz(nullptr), nelems(0), menor(c) {}

	// constructor por copia
	map(map_t const& other) {
		copia(other);
	}

	// operador de asignaci�n
	map_t & operator=(map_t const& that) {
		if (this != &that) {
			libera(raiz);
			copia(that);
		}
		return *this;
	}

	~map() {
		libera(raiz);
		raiz = nullptr;
	};

	int EncuentraKesimo(const int& k, const Link& raiz) {
		if (raiz == nullptr)return 0;
		else if (k < raiz->tam_i)return EncuentraKesimo(k, raiz->iz);
		else if (k > raiz->tam_i)return EncuentraKesimo(k - raiz->tam_i, raiz->dr);
		else return raiz->cv.clave;
	}

	// solamente se inserta si la clave no existe ya en el diccionario
	bool insert(clave_valor const& cv) {
		return inserta(cv, raiz);
	}

	bool empty() const {
		return raiz == nullptr;
	}

	int size() const {
		return nelems;
	}

	int count(Clave const& c) const {
		return (busca(c, raiz) != nullptr) ? 1 : 0;
	}

	Valor const& at(Clave const& c) const {
		Link p = busca(c, raiz);
		if (p == nullptr)
			throw std::out_of_range("La clave no se puede consultar");
		return p->cv.valor;
	}

	Valor & operator[](Clave const& c) {
		return corchete(c, raiz);
	}

	bool erase(Clave const& c) {
		return borra(c, raiz);
	}

	// solo para depurar (gira la cabeza)
	void print(std::ostream& o = std::cout) const {
		if (raiz != nullptr) {
			print(o, 0, raiz);
		}
		else
			o << "vacio\n";
	}

protected:

	void copia(map_t const& other) {
		raiz = copia(other.raiz);
		nelems = other.nelems;
		menor = other.menor;
	}

	static Link copia(Link a) {
		if (a == nullptr) return nullptr;
		else return new TreeNode(a->cv, copia(a->iz), copia(a->dr), a->altura);
	}

	static void libera(Link a) {
		if (a != nullptr) {
			libera(a->iz);
			libera(a->dr);
			delete a;
		}
	}

	Link busca(Clave const& c, Link a) const {
		if (a == nullptr) {
			return nullptr;
		}
		else if (menor(c, a->cv.clave)) {
			return busca(c, a->iz);
		}
		else if (menor(a->cv.clave, c)) {
			return busca(c, a->dr);
		}
		else { // c == a->cv.clave
			return a;
		}
	}

	bool inserta(clave_valor const& cv, Link & a) {
		bool crece;
		if (a == nullptr) {
			// se inserta el nuevo par <clave, valor>
			a = new TreeNode(cv);
			//a->tam_i = 1;
			++nelems;
			crece = true;
		}
		else if (menor(cv.clave, a->cv.clave)) {
			crece = inserta(cv, a->iz);
			if (crece) {
				reequilibraDer(a);
				//if (a->iz != nullptr)a->tam_i = a->iz->altura + 1;
				//else a->tam_i = 1;
			}
		}
		else if (menor(a->cv.clave, cv.clave)) {
			crece = inserta(cv, a->dr);
			if (crece) {
				reequilibraIzq(a);
				//if (a->iz != nullptr)a->tam_i = a->iz->altura + 1;
				//else a->tam_i = 1;
			}
		}
		else { // la clave ya estaba
			crece = false;
		}
		return crece;
	}

	Valor & corchete(Clave const& c, Link & a) {
		if (a == nullptr) {
			// se inserta la nueva clave, con un valor por defecto
			a = new TreeNode(clave_valor(c));
			++nelems;
			return a->cv.valor;
		}
		else if (menor(c, a->cv.clave)) {
			Valor & v = corchete(c, a->iz);
			reequilibraDer(a);
			return v;
		}
		else if (menor(a->cv.clave, c)) {
			Valor & v = corchete(c, a->dr);
			reequilibraIzq(a);
			return v;
		}
		else { // la clave ya est�, se devuelve el valor asociado
			return a->cv.valor;
		}
	}

	static int altura(Link a) {
		if (a == nullptr) return 0;
		else return a->altura;
	}

	static int getTam(Link a) {
		if (a == nullptr) return 0;
		else return a->tam_i;
	}

	static void rotaDer(Link & k2) {
		Link k1 = k2->iz;
		k2->iz = k1->dr;
		k1->dr = k2;
		k2->altura = std::max(altura(k2->iz), altura(k2->dr)) + 1;
		k1->altura = std::max(altura(k1->iz), altura(k1->dr)) + 1;
		k2->tam_i = k2->tam_i - k1->tam_i;
		k2 = k1;
	}

	static void rotaIzq(Link & k1) {
		Link k2 = k1->dr;
		k1->dr = k2->iz;
		k2->iz = k1;
		k1->altura = std::max(altura(k1->iz), altura(k1->dr)) + 1;
		k2->altura = std::max(altura(k2->iz), altura(k2->dr)) + 1;
		k2->tam_i = k2->tam_i + k1->tam_i;
		k1 = k2;
	}

	static void rotaIzqDer(Link & k3) {
		rotaIzq(k3->iz);
		rotaDer(k3);
	}

	static void rotaDerIzq(Link & k1) {
		rotaDer(k1->dr);
		rotaIzq(k1);
	}

	static void reequilibraIzq(Link & a) {
		if (altura(a->dr) - altura(a->iz) > 1) {
			if (altura(a->dr->iz) > altura(a->dr->dr))
				rotaDerIzq(a);
			else rotaIzq(a);
		}
		else {
			a->altura = std::max(altura(a->iz), altura(a->dr)) + 1;
			if (a->iz != nullptr)a->tam_i = a->iz->altura + 1;
			else a->tam_i = 1;
		}
	}

	static void reequilibraDer(Link & a) {
		if (altura(a->iz) - altura(a->dr) > 1) {
			if (altura(a->iz->dr) > altura(a->iz->iz))
				rotaIzqDer(a);
			else rotaDer(a);
		}
		else {
			a->altura = std::max(altura(a->iz), altura(a->dr)) + 1;
			if (a->iz != nullptr)a->tam_i = a->iz->altura + 1;
			else a->tam_i = 1;
		}
	}

	bool borra(Clave const& c, Link & a) {
		bool decrece = false;
		if (a != nullptr) {
			if (menor(c, a->cv.clave)) {
				decrece = borra(c, a->iz);
				if (decrece) reequilibraIzq(a);
			}
			else if (menor(a->cv.clave, c)) {
				decrece = borra(c, a->dr);
				if (decrece) reequilibraDer(a);
			}
			else { // c == a->cv.clave
				if (a->iz == nullptr || a->dr == nullptr) {
					Link aux = a;
					a = a->iz == nullptr ? a->dr : a->iz;
					--nelems;
					delete aux;
				}
				else { // tiene dos hijos
					subirMenor(a, a->dr, nullptr);
					--nelems;
				}
				decrece = true;
			}
			if (a->iz != nullptr)a->tam_i = a->iz->altura + 1;
			else a->tam_i = 1;
		}
		return decrece;
	}

	static void subirMenor(Link & a, Link b, Link padre) {
		if (b->iz != nullptr) {
			subirMenor(a, b->iz, b);
			reequilibraIzq(b);
		}
		else {
			if (padre != nullptr) {
				padre->iz = b->dr;
				b->dr = a->dr;
			}
			b->iz = a->iz;
			delete a;
			a = b;
			a->altura = std::max(altura(a->iz), altura(a->dr)) + 1;
		}
	}

	// imprime el �rbol (solo para depurar)
	static void print(std::ostream & o, int indent, Link r) {
		if (r != nullptr) {
			print(o, indent + 2, r->dr);
			o << std::string(indent, ' ');
			o << '(' << r->cv.clave << ',' << r->cv.valor << ")\n";
			print(o, indent + 2, r->iz);
		}
	}

protected:
	// iteradores que recorren los pares <clave, valor> de menor a mayor clave
	template <class Apuntado>
	class Iterador {
	public:
		Apuntado & operator*() const {
			if (act == nullptr)
				throw std::out_of_range("No hay elemento a consultar");
			return act->cv;
		}

		Apuntado * operator->() const {
			return &operator*();
		}

		Iterador & operator++() {  // ++ prefijo
			next();
			return *this;
		}

		bool operator==(Iterador const& that) const {
			return act == that.act;
		}
		bool operator!=(Iterador const& that) const {
			return !(this->operator==(that));
		}

	protected:
		friend class map;
		Link act;
		std::stack<Link> ancestros;  // antecesores no visitados

		// construye el iterador al primero
		Iterador(Link raiz) { act = first(raiz); }

		// construye el iterador al �ltimo
		Iterador() : act(nullptr) {}

		// construye el iterador a una clave concreta (para find)
		Iterador(map_t const* m, Clave const& c) {
			act = m->raiz;
			bool encontrado = false;
			while (act != nullptr && !encontrado) {
				if (m->menor(c, act->cv.clave)) {
					ancestros.push(act);
					act = act->iz;
				}
				else if (m->menor(act->cv.clave, c)) {
					act = act->dr;
				}
				else
					encontrado = true;
			}
			if (!encontrado) { // vaciamos la pila
			   // act == nullptr
				ancestros = std::stack<Link>();
			}
		}

		Link first(Link ptr) {
			if (ptr == nullptr) {
				return nullptr;
			}
			else { // buscamos el nodo m�s a la izquierda
				while (ptr->iz != nullptr) {
					ancestros.push(ptr);
					ptr = ptr->iz;
				}
				return ptr;
			}
		}

		void next() {
			if (act == nullptr) {
				throw std::out_of_range("El iterador no puede avanzar");
			}
			else if (act->dr != nullptr) { // primero del hijo derecho
				act = first(act->dr);
			}
			else if (ancestros.empty()) { // hemos llegado al final
				act = nullptr;
			}
			else { // podemos retroceder
				act = ancestros.top();
				ancestros.pop();
			}
		}
	};

public:
	// iterador que no permite modificar el elemento apuntado
	using const_iterator = Iterador<clave_valor const>;

	const_iterator cbegin() const {
		return const_iterator(raiz);
	}
	const_iterator begin() const {
		return cbegin();
	}

	const_iterator cend() const {
		return const_iterator();
	}
	const_iterator end() const {
		return cend();
	}

	const_iterator find(Clave const& c) const {
		return const_iterator(this, c);
	}

	// iterador que s� permite modificar el elemento apuntado (su valor)
	using iterator = Iterador<clave_valor>;

	iterator begin() {
		return iterator(raiz);
	}

	iterator end() {
		return iterator();
	}

	iterator find(Clave const& c) {
		return iterator(this, c);
	}

	Link getRoot() { return raiz; }
};


template <class Clave, class Valor, class Comparador>
inline std::ostream& operator<<(std::ostream & o, map<Clave, Valor, Comparador> const& a) {
	a.print(o);
	return o;
}

using namespace std;

int main() {
	int N, clave, M, consulta, resultado;
	cin >> N;

	while (N != 0) {
		map<int, int> tree;
		for (int i = 0; i < N; i++) {
			cin >> clave;
			tree.insert(clave);
		}
		cin >> M;
		for (int i = 0; i < M; i++) {
			cin >> consulta;
			resultado = tree.EncuentraKesimo(consulta, tree.getRoot());
			if (resultado != 0)cout << resultado << endl;
			else cout << "??" << endl;
		}
		cout << "----" << endl;
		cin >> N;
	}

	return 0;
}