#include "Huffman.h"

// Debug tool: ridefinizione dell'operatore << per stampare ogni singolo HNode, qual'ora necessario
ostream &operator<<(ostream &out, HNode &h)
{
	out << "Stringa: \"" << h.getString() << "\"; Grado: " << h.getGrade() << "; " << endl;
	return out;
}

// Restituisce una stringa contenente i caratteri unici della stringa che prende in input
string getUniqueChars(string input)
{
	if (input == "") 	return NULL;
	
	string str;
	int index = 1;
	str += input[0];
	
	for (int i = 1; i < input.length(); i++)
	{
		bool check = true;
		
		for (int j = 0; j < i; j++)
		{
			if (input[j] == input[i])
			{
				check = false;
				break;
			}
		}
		
		if (check)
		{
			str += input[i];
			index++;
		}
	}
	
	return str;
}

// Prende una stringa "str" e un'altra "uniqueChars" che contiene i caratteri unici della prima,
// restituisce un array contenente la frequenza di ogni carattere unico della stringa str
int* getCharFrequency(string str, string uniqueChars)
{
	int len = uniqueChars.size();
	int *freq = new int[len];
	
	for (int i = 0; i < len; i++)
		freq[i] = 0;
	
	for (int i = 0; i < len; i++)
		for (int j = 0; j < str.size(); j++)
			if (uniqueChars[i] == str[j])
				freq[i]++;
	
	return freq;
}

// Prende in input un array di HNode "nodes" e ne ordina gli elementi che vanno da x a y(escluso)
// in base al grado del nodo (frequenza della sottostringa)
void SortByGrade(HNode *nodes, int x, int y)
{
	for (int index = x+1; index < y; index++)
	{
		HNode key = nodes[index];
		int position = index;

		while (position > x && key.getGrade() < nodes[position-1].getGrade())
		{
			nodes[position] = nodes[position-1];
			position--;
		}

		nodes[position] = key;
	}
}

// Funzione ricorsiva.
// Prende in input la radice *n di un albero di nodi HNode e scrive il path binario di ogni nodo
void encoding(HNode *n)
{	
	if (n->getLeft() != NULL)
	{
		n->getLeft()->setPath(n->getPath()+"0");
		encoding(n->getLeft());
	}
		
	if (n->getRight() != NULL)
	{
		n->getRight()->setPath(n->getPath()+"1");
		encoding(n->getRight());
	}
}

// Teorma di Shannon:
// I dati possono essere rappresentati senza perdere informazione (lossless)
// usando almeno un numero di bit pari a: N*E
// dove N è il numero di caratteri mentre E è l’entropia.
float Shannon(HNode *nodes, int charsCount, float N)
{
	float E = 0.0f;

	for (int i = 0; i < charsCount; i++)
		E += nodes[i].getGrade()/N * log2(nodes[i].getGrade()/N);

	E = -E;

	return N*E;
}

// Codifica e stampa i risultati
void HuffmanCoding(string input)
{
	string uniqueChars = getUniqueChars(input);
	float inputCount = input.size();
	int charsCount = uniqueChars.size();
	int *charFreq = getCharFrequency(input, uniqueChars);
	
	HNode *nodes = new HNode[charsCount*2 - 1]; // conterra' tutti i nodi dell'albero
	
	int index = 0;
	int index2 = 0;
	string tmp;
	
	// inserimento delle foglie, ovvero i nodi contenenti come sottostringhe i singoli caratteri unici)
	for (; index < charsCount; index++)
		nodes[index] = *(new HNode(tmp = uniqueChars[index], charFreq[index]));

	// ordinamento delle foglie in base alla frequenza del carattere contenuto
	SortByGrade(nodes, 0, charsCount);
	
	// costruzione dell'albero
	for (int count = 0; count < charsCount*2 - 1 - charsCount; count++)
	{
		// i nodi di posto "index2" e "index2+1" vengono accoppiati
		// il nodo padre viene collocato nella posizione "index" dell'array
		nodes[index] = *(new HNode(nodes[index2].getString()+nodes[index2+1].getString(), &nodes[index2], &nodes[index2+1]));
		index2+=2;
		index++;
		
		// il sottoarray contenente i nuovi nodi che non sono ancora stati accoppiati viene riordinato
		SortByGrade(nodes, index2, index);
	}
	
	// riferimento alla radice dell'albero
	HNode *root = &nodes[charsCount*2 - 2];
	
	// scrittura dei path per ogni nodo (ricorsivamete)
	encoding(root);
	
	HNode *leaves = new HNode[charsCount]; // conterra' le sole foglie dell'albero
	
	// recupero di tutte le foglie dell'albero
	index2 = 0;
	for (int x = 0; x < charsCount*2 - 1; x++)
		if (nodes[x].getString().size() == 1)
			leaves[index2++] = nodes[x];
	
	// stampa della codifica
	cout << endl << "Huffman Encoding for string \"" << input << "\" [length: " << inputCount << "; unique character: " << charsCount << "]:" << endl << endl;

	int totBits = 0;
	
	for (int x = 0; x < charsCount; x++)
	{
		cout << leaves[x].getString() << ": " << leaves[x].getPath() << " (" << leaves[x].getGrade() << ") --> " << leaves[x].getBitsCount() << " bits" << endl;
		totBits += leaves[x].getBitsCount();
	}
	
	// calcolo se stampa dei limiti inferiori e superiori tollerati
	// dal Teorema di Shannon (rispettivamente N*E e N*E + N)
	float shannon = Shannon(leaves, charsCount, inputCount);

	cout << endl << "Using " << totBits << " bits." << endl << endl;
	cout << "MIN: " << shannon << " bits (Shannon)" << endl;
	cout << "MAX: " << shannon+inputCount << " bits (Shannon + length)" << endl;

	// il numero di bits utilizzati deve rientrare nel range [MIN, MAX]
	// tendendo generalmente a MIN(approssimato per eccesso) con input di piccole dimensioni
}


int main(int argc, const char* argv[])
{
	// controllo correttezza parametri
	if (argc != 2)
	{
		cout << "Usage: " << argv[0] << " \"string to encode\"" << endl;
		return 0;
	}
	
	// codifica e stampa
	HuffmanCoding(argv[1]);
	
	// credits
	cout << endl << endl << "Coded by Francesco Borzi'" << endl << endl;
	
	return 0;
}
