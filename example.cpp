#include <vector>
#include <algorithm>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <Variable.hpp>
using namespace std;

class Card{
private:
	int number;
	string suit;
public:
	Card(const int & number, const string & suit)
		:number(number), suit(suit) {;}
	
	string name() const{
		return to_string(number) + " of " + suit;
	}
};

class Deck{
private:
	Variable< vector<Card>* > cards;
	
	void insert_cards(vector<Card>* const & my_cards){
		if( cards.get()->empty() ){
			for( int i = 1; i <= 12; i++){
				cards.get()->push_back( Card(i, "clubs") );
				cards.get()->push_back( Card(i, "diamonds") );
				cards.get()->push_back( Card(i, "hearts") );
				cards.get()->push_back( Card(i, "spades") );
			}
		
			random_shuffle( cards.get()->begin(), cards.get()->end() );
			cards.send();
		}
	}
public:
	Deck(){
		srand( time(NULL) );
		cards.subscribe<Deck, &Deck::insert_cards>(this);
		cards = new vector<Card>;
	}
	Card draw_card(){
		Card drawn(cards.get()->back());
		cards.get()->pop_back();
		cards.send();
		return drawn;
	}
};

int main(){
	Deck deck;
	
	for(int i = 1; i <= 49; i++){
		cout << "You draw the card (" << i << "): " << deck.draw_card().name() << endl;
	}
}
