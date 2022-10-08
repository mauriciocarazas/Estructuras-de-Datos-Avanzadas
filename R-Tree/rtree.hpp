#pragma once
#ifndef RTREE_HPP_
#define RTREE_HPP_
#include "Shapes.hpp"
#include <utility>
#include <stack>


template<class Data, unsigned int Dim>
class Entry
{
public:
	HyperRect<Dim> Rect;
	vector<Entry<Data, Dim>>* Child;
	Data* tuple;

	explicit Entry() {
		tuple = nullptr;
		Child = nullptr;
	}
	explicit Entry(const HyperRect<Dim>& _Rect, vector<Entry<Data, Dim>>* _Child = nullptr, Data* _tuple = nullptr) {
		Rect = _Rect;
		tuple = _tuple;
		Child = _Child;
	}
	Entry(const Entry<Data, Dim>& _entry) {
		Rect = _entry.Rect;
		tuple = _entry.tuple;
		Child = _entry.Child;
	}

	void setValues(const HyperRect<Dim>& _Rect, vector<Entry<Data, Dim>>* _Child = nullptr, Data* _tuple = nullptr) {
		Rect = _Rect;
		tuple = _tuple;
		Child = _Child;
	}

	// si es hoja, la entrada no posee mas hijos
	bool IsLeaf() {
		return !Child;
	}
	~Entry() {
		if (Child) {// solo si mi puntero hijo no es nulo
			Child->clear();
			delete Child;
		}
	}
};


template<class Data, unsigned int Dim>
class RTree
{
public:
	// Variables
	vector<Entry<Data, Dim>> *root;
	unsigned int M, m;

	explicit RTree() {
		M = 0;
		m = 0;
		root = nullptr;
	}
	
	explicit RTree(unsigned int _M, unsigned int _m, vector<Entry<Data, Dim>>* _root = nullptr) {
		M = max<unsigned int>(2, _M); // si M=1 entonces m<=0 (error), por eso siempre M>=2
		m = min<unsigned int>(max<unsigned int>(1, _m), _M / 2);// para validar que siempre se cumpla 1<=m<=(M/2)
		// si _root contiene algo, hago una copia de este en mi ra�z, sino creamos un nuevo nodo
		root = (_root)? new vector<Entry<Data, Dim>>((*_root)) : _root;
	}

	// dado un vector de entradas, genero su m�nimo rect�ngulo contenedor
	HyperRect<Dim> genHyperRect(vector<Entry<Data,Dim>>& RNode) {
		size_t size_set = RNode.size();
		// primero agregamos cada hiper-rect�ngulo del conjunto a un vector
		// de hiper-rect�ngulos, todo esto con el fin de usarlo en la normalizaci�n
		// del paso dos en el algoritmo LinearPickSeeds del art�culo
		vector<HyperRect<Dim>> SetBoxes;
		for (size_t e = 0; e < size_set; ++e)
			SetBoxes.push_back(RNode[e].Rect);

		return MBR<Dim>(SetBoxes);
	}

	// Entradas:
	// Results -> Donde se guardar�n las entradas del resultado de b�squeda
	// S -> El hiper-rect�ngulo de b�squeda
	// T -> El nodo en el que buscamos, empezamos con la ra�z
	// Retorno:
	// retornamos true si al menos una entrada se superpone con el (rect�ngulo query: S)
	// en otras palabras true si el vector Results no est� vac�o, de lo contrario false
	bool Search(vector<Entry<Data, Dim>>& Results, HyperRect<Dim>& S, vector<Entry<Data, Dim>>* T = nullptr) {
		if(!T) T = root;// si T no recibe ning�n par�metro, empezamos desde la ra�z
		// si la ra�z es nula, o el nodo de �sta est� vac�a, no hay ninguna entrada en el �rbol
		if (!root || root->empty()) return false;

		size_t size_set = T->size();
		// si T es un nodo hoja, o no contiene ning�n nodo
		if (T->empty() || (*T)[0].IsLeaf())
		{
			for (size_t i = 0; i < size_set; ++i) {
				if (Overlap(S, (*T)[i].Rect))
					Results.push_back((*T)[i]); // agregamos la i�sima entrada, porque hubo resultado
			}
		}
		else { // el nodo no es una hoja
			for (size_t i = 0; i < size_set; ++i)
				if (Overlap(S, (*T)[i].Rect))
					Search(Results, S, (*T)[i].Child);// aplicacmos la b�squeda recursivamente
			
		}
		return !Results.empty(); // si el vector est� vac�o false, si no true
	}

	void ChooseLeaf(Entry<Data, Dim>& E, stack<vector<Entry<Data, Dim>>**>& Path, stack<int>& PathEntryPos) {
		vector<Entry<Data, Dim>>* N = root;
		// si la ra�z es nula, o est� vac�a, o es el nodo hoja, simplemente agregamos la direcci�n de la ra�z al Path
		if (!N || N->empty() || (*N)[0].IsLeaf()) {
			Path.push(&root);
			return;
		}
		HyperRect<Dim> FRect;
		// mientras el nodo N no sea hoja, continuamos la selecci�n de la hoja
		while (!(*N)[0].IsLeaf()) {
			// primero agregamos el nodo N actual a la pila
			Path.push(&N);

			size_t size_set = (*N).size();
			int min_idx = 0;
			// escogemos el rect�ngulo del primer nodo, como el sub�rbol al que debe dirigirse E
			FRect = (*N)[min_idx].Rect;
			vector<HyperRect<Dim>> Boxes;
			Boxes.push_back(FRect);
			Boxes.push_back(E.Rect);

			// el m�nimo alargamiento al principio
			double min_enlargement = MBR<Dim>(Boxes).HyperVolume() - FRect.HyperVolume(), curr_enlargement;
			double min_hypervolume = FRect.HyperVolume(), curr_hypervolume;
			// empezamos desde el comienzo
			for (int i = 1; i < size_set; ++i) {
				Boxes.clear();
				Boxes.push_back((*N)[i].Rect);
				Boxes.push_back(E.Rect);

				curr_enlargement = MBR<Dim>(Boxes).HyperVolume() - (*N)[i].Rect.HyperVolume();
				curr_hypervolume = (*N)[i].Rect.HyperVolume();

				if (curr_enlargement < min_enlargement) {
					min_enlargement = curr_enlargement;
					min_hypervolume = curr_hypervolume;
					FRect = (*N)[i].Rect;
					min_idx = i;
				}
				else if (curr_enlargement == min_enlargement) {
					if (curr_hypervolume < min_hypervolume) {
						min_enlargement = curr_enlargement;
						min_hypervolume = curr_hypervolume;
						FRect = (*N)[i].Rect;
						min_idx = i;
					}
				}
			}

			// agregamos la direcci�n al puntero del nodo rama al Path
			// Path.push(&((*N)[min_idx].Child));
			// ahora N es la entrada cuyo rect�ngulo necesita alargarse lo menos para encerrar a E
			N = (*N)[min_idx].Child;
			// agregamos la posici�n del padre con el m�nimo enlargement, esto nos servir�
			// al momento de ejecutar la funci�n AdjustTree
			PathEntryPos.push(min_idx);
		}
		// terminamos de agreggar el �ltimo nodo, que si es hoja al camino
		Path.push(&N);
		// PathEntryPos no se llenar� cuando obtengamos una hoja, sino que solo contendr� la posici�n 
		// de las entradas de aquellos que no son hojas, debemos ser cuidadosos de eliminar el top
		// de la pila Path en la funci�n Insert, ya que esta es simplememente una hoja sin PathEntryPos
		return;
	}
	
	// escoge las dos primeras semillas para la partici�n, retorna sus �ndices 
	// dentro del conjunto G de entradas
	void LinearPickSeeds(vector<Entry<Data, Dim>>& G, size_t& seed1 = 0, size_t& seed2 = 0) {
		size_t size_set = G.size();
		// primero generamos el hyper rect�ngulo que contiene a todo un vector de entradas
		HyperRect<Dim> SetBox = genHyperRect(G);

		// un array para guardar los �ndices de los pares obtenidos por cada dimensi�n
		pair<size_t, size_t> seeds[Dim];
		// un array para registrar las diferencias de los pares con la mayor separaci�n a lo largo
		// de todas las dimensiones
		double difference[Dim];

		double high_low_side, low_high_side;
		double curr_low_side, curr_high_side;

		// recorremos cada dimensi�n
		for (int d = 0; d < Dim; ++d) {
			high_low_side = LDBL_MIN, low_high_side = LDBL_MAX;
			// recorremos cada entrada del conjunto
			for (size_t e = 0; e < size_set; ++e) {
				curr_high_side = G[e].Rect.Ivals[d].upp;// la cota superior del intervalo d
				curr_low_side = G[e].Rect.Ivals[d].low;// la cota inferior del intervalo d
				if (high_low_side < curr_low_side) {
					high_low_side = curr_low_side;
					seeds[d].first = e; // en la dimensi�n d guardamos la e-�sima entrada(HLS: highest low side)
				}
				if (low_high_side > curr_high_side) {
					low_high_side = curr_high_side;
					seeds[d].second = e;// en la dimensi�n d guardamos la e-�sima entrada(LHS: lowest high side)
				}
			}
			// una vez escogidos ambos elementos del conjunto, lo que hacemos es
			// registrar la diferencia de ambos pivotes de semillas: HLS y LHS
			difference[d] = std::abs(high_low_side - low_high_side);
		}

		// consideraremos que las primeras semillas estar�n en la dimensi�n 0
		seed1 = seeds[0].first;
		seed2 = seeds[0].second;
		// normalizamos la diferencia de HLS y LHS por la separaci�n del MBR general de todos los M+1 rect�ngulos
		// al principio la m�nima normalizaci�n sucede en la dimensi�n 0
		double max_normalization = difference[0] / (SetBox.Ivals[0].upp - SetBox.Ivals[0].low), curr_normalization;

		// ahora
		for (int d = 1; d < Dim; ++d) {
			// a la diferencia la normalizamos por la separaci�n total que se d� en el MBR
			// de todos los nodos en esa dimensi�n
			curr_normalization = difference[d] / (SetBox.Ivals[d].upp - SetBox.Ivals[d].low);
			if (max_normalization < curr_normalization) {
				max_normalization = curr_normalization;
				// actualizamos el �ndice de ambas semillas
				seed1 = seeds[d].first;
				seed2 = seeds[d].second;
			}
		}
		return;
	}

	// Divide el conjunto G de M+1 entradas, y los reparte en los dos conjuntos L(derecha) y LL(izquierda)
	void SplitNode(vector<Entry<Data, Dim>>*& G, vector<Entry<Data, Dim>>*& L, vector<Entry<Data, Dim>>*& LL) {
		size_t seedL, seedLL;
		LinearPickSeeds((*G), seedL, seedLL);
		// ahora asignamos a cada grupo su semilla
		L->push_back((*G)[seedL]); // la semilla 1 para el grupo de la izquierda
		LL->push_back((*G)[seedLL]); // la semilla 2 para el de la derecha
		
		// un conjunto auxiliar para L y LL, al momento de calcular el enlargement necesario para
		// incluir una entrada del conjunto de nodos restantes en cualquiera de los os grupos 
		vector<Entry<Data, Dim>> aux_L;
		vector<Entry<Data, Dim>> aux_LL;
		aux_L.push_back((*G)[seedL]); // la semilla 1 para el grupo auxiliar de la izquierda
		aux_LL.push_back((*G)[seedLL]); // la semilla 2 para el auxiliar de la derecha

							
		// recorremos todo el conjunto de entradas, y vamos asignando cada una a un conjunto
		// esta ser�a la funci�n PickNext en el Split cuadr�tico, pero aqu� es m�s sencilla
		for (int i = 0; i < M + 1; ++i)
		{
			// primero verificamos que el �ndice de esa entradas no sea una semilla
			if (i != seedL && i != seedLL) {
				// siempre debemos verificar que alg�n conjunto no tenga tan pocas entradas como para violar
				// la regla de que en cada nodo debe existir entre m y M entradas
				if (L->size() == M - m + 1) {
					// significa que el conjunto L lleg� al tope, por lo que el conjunto LL
					// necesita del resto de nodos para cumplir la cuota m m�nima de nodos
					LL->push_back((*G)[i]);
					continue;
				}
				if (LL->size() == M - m + 1) {
					// significa que el conjunto LL lleg� al tope, por lo que el conjunto L
					// necesita del resto de nodos para cumplir la cuota m m�nima de nodos
					L->push_back((*G)[i]);
					continue;
				}

				// primero agregamos el cuadrante de la entrada a ambos grupos
				aux_L.push_back((*G)[i]);
				aux_LL.push_back((*G)[i]);

				double currAreaSetL = genHyperRect(*L).HyperVolume();
				double currAreaSetLL = genHyperRect(*LL).HyperVolume();

				// ahora obtenemos el enlargement de agregar la i-�sima entrada a ambos grupos
				// obtenemos todo el �rea de agregar la i-entrada en el conjunto L y LL cu., luego
				// restamos esa �rea por el �rea del conjunto L/LL cuando todav�a no se agrega esa entrada
				double enlargementSetL = genHyperRect(aux_L).HyperVolume() - currAreaSetL;
				double enlargementSetLL = genHyperRect(aux_LL).HyperVolume() - currAreaSetLL;

				if (enlargementSetL < enlargementSetLL) {
					aux_LL.pop_back();// eliminamos la entrada del conjunto LL, porque no se agregar� a �ste
					L->push_back((*G)[i]);// aqu� reci�n asignamos la i-�sima entrada al conjunto L
				}
				else if (enlargementSetL == enlargementSetLL) {
					// si ambos enlargements son iguales resolvemos el empate por el menor area del conjunto
					if (currAreaSetL < currAreaSetLL)
					{
						aux_LL.pop_back();// eliminamos la entrada del conjunto LL, porque no se agregar� a �ste
						L->push_back((*G)[i]);// aqu� reci�n asignamos la i-�sima entrada al conjunto L
					}
					else {
						aux_L.pop_back();// eliminamos la entrada del conjunto L, porque no se agregar� a �ste
						LL->push_back((*G)[i]);// aqu� reci�n asignamos la i-�sima entrada al conjunto LL
					}
				}
				else {
					aux_L.pop_back();// eliminamos la entrada del conjunto L, porque no se agregar� a �ste
					LL->push_back((*G)[i]);// aqu� reci�n asignamos la i-�sima entrada al conjunto LL
				}
			}
		}
		return;
	}

	// retorna verdadero si la ra�z fu� dividida por el ajuste del �rbol
	bool AdjustTree(stack<vector<Entry<Data, Dim>>**>& Path, stack<int>& PathEntryPos, vector<Entry<Data, Dim>>*& L, vector<Entry<Data, Dim>>*& LL) {
		vector<Entry<Data, Dim>>* N = L, *NN = LL;
		// el primer caso es verdadero �nicamente cuando a la ra�z se le asigno un nodo y no hubo split
		// el segundo cuando la ra�z es hoja y fu� partida en la inserci�n del elemento en Insert
		if (N == root || (*Path.top()) == root) return false;

		// los padres del nodo N y NN si hubiese divisi�n
		vector<Entry<Data, Dim>>* P, * PP;

		// un flag que me indique si el ajuste hizo que la ra�z se partiera
		bool splitted_root = false;

		// mientras el camino no se acabe continuamos con el ajuste del �rbol
		while (!Path.empty()) {
			// P ser� el padre de N, y PP por ahora ser� nulo
			P = (*Path.top());
			PP = nullptr;
			// ahora debemos hacer que el rect�ngulo contenedor de la entrada E en P que contiene a N
			// contenga nuevamente a N y el resto de sus entradas divididas
			(*P)[PathEntryPos.top()].Rect = genHyperRect(*N);

			// si el puntero NN no es nulo, tenemos que agregar una nueva entrada al nodo padre P, cuyo MBR
			// encierre al nodo NN, y que tenga de hijo a NN
			if (NN) {
				Entry<Data, Dim> newEntry(genHyperRect(*NN), NN);
				P->push_back(newEntry);// aqu� se hace una copia de newEntry
				newEntry.Child = nullptr;// esto es para evitar que el destructor de la instancia new Entry no borre el puntero a NN
			}
			// si despu�s de agregar una nueva entrada, esta caus� al padre tener mas de M hijos, debemos realizar
			// la funci�n SplitNode en Path.top()
			if (P->size() == M + 1) {
				// P ya no es igual a *Path.top(), pasar� a ser un nuevo nodo resultante del split de Path.top()
				P = new vector<Entry<Data, Dim>>;// P ser� conjunto de la izquierda
				PP = new vector<Entry<Data, Dim>>;// PP ser� el conjunto de la derecha
				SplitNode(*Path.top(), P, PP);
				// verificamos si el split se hizo en la ra�z
				if ((*Path.top()) == root) splitted_root = true;
				// El paso que debemos hacer ahora es un poco tramposo, o de mala pr�ctica, al hacer el SplitNode
				// (*Path.top()) no cambia, P y PP simplemente copian sus entradas dependiendo del grupo al que
				// pertenezcan, es obvio que tambi�n copian las direcciones de sus punteros hijos, en memoria a�n
				// tenemos reservado (*Path.top()), pero este debe eliminarse, y como la eliminaci�n es recursiva 
				// (es decir tambi�n se eliminar�n sus hijos, y los hijos de sus hijos), debemos asignar a los punteros
				// hijos de las entradas de (*Path.top()) el valor nulo, para que al momento de liberar la memoria
				// que apunta (*Path.top()) no se eliminen los punteros hijos de las entradas de P y PP, esto se puede
				// evitar con el uso de smart pointers, ya que un objeto al ser apuntado por varios punteros, contabiliza
				// el n�mero de apuntaciones que se le hacen.
				for (int i = 0; i < (*Path.top())->size(); ++i)
					(**Path.top())[i].Child = nullptr;

				delete (*Path.top());// liberamos la memoria que apunta *Path.top();
				(*Path.top()) = P;// debemos cambiar el valor de (*Path.top()), ya que N ser� igual a P
			}
			Path.pop();
			PathEntryPos.pop();
			// N pasa a ser P, y NN a ser PP,si no hubo divisi�n NN ser� nulo
			N = P;
			NN = PP;
		}
		// igualamos L y LL con el objetivo de usarlos en la creaci�n de una nueva ra�z
		// si la ra�z fu� dividida por el algoritmo AdjustTree
		L = N;
		LL = NN;
		return splitted_root;
	}
	
	void Insert(Entry<Data, Dim> E) {
		// si la ra�z es nula o est� vac�a, simplemente agregamos la nueva entrada
		if (!root) {
			root = new vector<Entry<Data,Dim>>;
			root->push_back(E);
			return;
		}
		if (root->empty()) {
			root->push_back(E);
			return;
		}

		stack<vector<Entry<Data, Dim>>**> Path;
		stack<int> PathEntryPos;
		// llenamos la variable Path con el camino a seguir para insertar la nueva entrada E
		// y tambi�n con los �ndices de las entradas de los nodos padres en el camino definido
		// Path.top() ser� el nodo hoja en el que se insertar� E
		ChooseLeaf(E, Path, PathEntryPos);

		// creamos los dos conjuntos nuevos, por si necesit�semos realizar un Split
		vector<Entry<Data, Dim>>* L, * LL;
		L = (*Path.top());
		LL = nullptr;

		//agregamos la entrada E al nodo hoja seleccionado
		(*Path.top())->push_back(E);

		// me indica si la ra�z fu� dividida
		bool splitted_root = false;
		
		// si hay mas de M entradas, aplicamos el algoritmo SplitNode
		if ((*Path.top())->size() == M + 1) {
			// creamos los dos nuevos nodos para el split
			L = new vector<Entry<Data, Dim>>; // L ya no es el nodo hoja, si no su conjunto izquierdo
			LL = new vector<Entry<Data, Dim>>;

			SplitNode((*Path.top()), L, LL);
			// si la hoja partida fu� la ra�z indicamos que �sta fue dividida
			if ((*Path.top()) == root) splitted_root = true;
			
			// retiramos el puntero al puntero del nodo top, que vendr�a a ser la hoja encontrada
			// el que sigue en el camino vendr�a a ser su padre, tambi�n liberamos la memoria siendo apuntada
			// por ese nodo; ya que ha sido dividido, no necesitamos reservar memoria en este nodo
			delete (*Path.top());

			// despu�s ponemos como padre del nodo L al puntero que antes ten�a como hijo al nodo
			// que se ha dividido, es decir a *Path.top()
			(*Path.top()) = L;

			Path.pop();


			// Si el nodo ra�z fu� partido, o el ajuste del �rbol hizo que �ste se partiera, creamos una nueva
			// ra�z y ponemos como sus dos hijos a los dos nodos resultantes de la partici�n de la ra�z
			// EN AdjustTree enviamos el camino, las posiciones del cuadrante perteneciente al ancestro de la entrada E
			// el nodo L y LL, los dos nodos resultantes del split nodo izquierdo y derecho respectivamente
			if (splitted_root || AdjustTree(Path, PathEntryPos, L, LL)) {
				root = new vector<Entry<Data, Dim>>(2);// nuevo vector de tama�o 2
				// creamos una entrada que tenga como hijo a L y cuyo MBR lo contenga, lo mismo para LL
				// agregamos los dos nuevos hijos
				(*root)[0].Rect = genHyperRect(*L);
				(*root)[0].Child = L;
				(*root)[1].Rect = genHyperRect(*LL);
				(*root)[1].Child = LL;
			}
			return;
		}
		
		L = (*Path.top());
		// retiramos el puntero al puntero del nodo top, que vendr�a a ser la hoja encontrada
		// el que sigue en el camino vendr�a a ser su padre
		Path.pop();
		// Enviaremos el camino, las posiciones del cuadrante perteneciente al ancestro de la entrada E
		// el nodo L, que vendr�a a ser igual a la hoja en que insertamos E, el nodo LL ser�a simplemente nullptr
		AdjustTree(Path, PathEntryPos, L, LL);
		
		return;
	}/**/


	~RTree() {
		if (root) {
			root->clear();
			delete root;
		}
	}

};




#endif // RTREE_HPP_;
