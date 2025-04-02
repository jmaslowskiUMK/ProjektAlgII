#include <iostream>
#include "Country.h"

int main() {
	/*Country polska;
	Pub pubMentzena(1,1,5,1);
	polska.insertPub(pubMentzena);
	std::optional <Pub> pubMentzenaKopia = polska.getPub(1);
	if ( pubMentzenaKopia.has_value() ) {
		pubMentzenaKopia->print();
	}
	std::optional <Pub> pubMentzenaKopia2 = polska.getPub(5);
	if (pubMentzenaKopia2.has_value()) {
		pubMentzenaKopia2->print();
	}
	else {
		std::cout << "ni ma" << std::endl;
	}

	Brewery staryKruk(2,3,5,10,2);
	polska.insertBrewery(staryKruk);
	std::optional <Brewery> staryKrukKopia = polska.getBrewery(2);
	if (staryKrukKopia.has_value()) {
		staryKrukKopia->print();
	}

	Field lysePole(15,3,4,5,3);
	polska.insertField(lysePole);
	std::optional <Field> lysePoleKopia = polska.getField(3);
	if (lysePoleKopia.has_value()) {
		lysePoleKopia->print();
	}

	Intersection polna(6, 7, 4,5);
	polska.insertIntersection(polna);
	std::optional <Intersection> polnaKopia = polska.getIntersection(4);
	if (polnaKopia.has_value()) {
		polnaKopia->print();
	}

	polska.makeConnectionById(1,4);
	polska.makeConnectionById(4, 3);
	polska.makeConnectionById(4, 2);
	*/

	Country polska;
	auto pubMentzena = polska.createPub(1,2,4);
	auto lysePole = polska.createField(1,1,4,4);
	auto pubULecha = polska.createPub(2, 3, 5);
	auto pubPodKogutem = polska.createPub(4,1,4); 
	auto zlotePole = polska.createField(0, 0, 5, 5);
	auto las = polska.createField(3, 2, 41,5);      
	auto browarWarka = polska.createBrewery(1, 4,5,155,1424);  
	auto browarZywiec = polska.createBrewery(5,2,1,41,141); 
	auto rondo = polska.createIntersection(3, 3,4);  
	auto rynek = polska.createIntersection(0, 0, 8);  
	polska.addRelationship(pubMentzena,rondo);
	polska.addRelationship(pubMentzena, rynek);
	polska.addRelationship(browarWarka, rondo);
	polska.addRelationship(browarZywiec, rondo);
	polska.addRelationship(las, rondo);
	polska.addRelationship(zlotePole, rondo);
	
	polska.bfs();




	return 0;
}