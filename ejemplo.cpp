#include <vector>
#include <algorithm>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <Variable.hpp>
using namespace std;

class Carta{
private:
	int numero;
	string palo;
public:
	Carta(const int & numero, const string & palo)
		:numero(numero), palo(palo) {;}
	
	string nombre() const{
		return to_string(numero) + " de " + palo;
	}
};

class Baraja{
private:
	Variable< vector<Carta>* > cartas;
	
	void meter_cartas(vector<Carta>* const & mis_cartas){
		if( cartas.get()->empty() ){
			for( int i = 1; i <= 12; i++){
				if(i == 8) i = 10;
				cartas.get()->push_back( Carta(i, "oros") );
				cartas.get()->push_back( Carta(i, "bastos") );
				cartas.get()->push_back( Carta(i, "espadas") );
				cartas.get()->push_back( Carta(i, "copas") );
			}
		
			random_shuffle( cartas.get()->begin(), cartas.get()->end() );
			cartas.send();
		}
	}
public:
	Baraja(){
		srand( time(NULL) );
		cartas.subscribe<Baraja, &Baraja::meter_cartas>(this);
		cartas = new vector<Carta>;
	}
	Carta robar_carta(){
		Carta robada(cartas.get()->back());
		cartas.get()->pop_back();
		cartas.send();
		return robada;
	}
};

int main(){
	Baraja baraja;
	
	for(int i = 1; i <= 41; i++){
		cout << "Coges la carta (" << i << "): " << baraja.robar_carta().nombre() << endl;
	}
}
