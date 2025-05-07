#include <iostream>
#include "Country.h"
#include <utility>


int main() {

//examples in CSV_PARSER.CPP

	Country polska;
	//code creates shared_ptr to classes that derive from Node. setName() is a method defined in Node, holds a string with name 
	auto pubMentzena = polska.createPub("fafaf",1, 2, 4);
	pubMentzena->setName("pubMentzena");
	auto lysePole = polska.createField("fafafa",1, 1, 4, 4);
	lysePole->setName("lysePole");
	auto pubULecha = polska.createPub("aaaa",2, 3, 5);
	pubULecha->setName("pubULecha");
	auto pubPodKogutem = polska.createPub("ccc",4, 1, 4);
	pubPodKogutem->setName("pubPodKogutem");
	auto zlotePole = polska.createField("zzzz",0, 0, 5, 5);
	zlotePole->setName("zlotePole");
	auto las = polska.createField("fafafafafa",3, 2, 41, 5);
	las->setName("las");
	auto browarWarka = polska.createBrewery("aaa",1, 4, 5, 155);
	browarWarka->setName("browarWarka");
	auto browarZywiec = polska.createBrewery("fafafaaaaaa",5, 2, 1, 41);
	browarZywiec->setName("browarZywiec");
	auto browareczekLidla = polska.createBrewery("papa",3, 3, 3, 3);
	browareczekLidla->setName("browareczekLidla");
	auto rondo = polska.createIntersection(3, 3);
	rondo->setName("rondo");
	auto rynek = polska.createIntersection(0, 0);
	rynek->setName("rynek");
	auto starowka = polska.createIntersection(9, 9);
	starowka->setName("starowka");
	auto ryneczekLidla = polska.createIntersection(0, 0);
	ryneczekLidla->setName("ryneczekLidla");
	auto browarBarka = polska.createBrewery("papapapapa",24, 24, 24,24);
	ryneczekLidla->setName("browarBarka");
	
	//example from: https://pl.wikipedia.org/wiki/Algorytm_Edmondsa-Karpa
	//code is making instances of class Lane(shared_ptr from,shared_ptr to,int flow) and addsRelationship to adjacency list adjList(shared_ptr from, Lane)
	//Source = las, pubPodKogutem Sink = ryneczekLidla,browarBarka
	Lane lane1(las, zlotePole, 3);
	polska.addRelationship(lane1);//A B

	Lane lane1_1(pubPodKogutem,zlotePole,3);
	polska.addRelationship(lane1_1);

	Lane lane2(zlotePole, rynek, 4);
	polska.addRelationship(lane2);//B C
	
	Lane lane3(rynek, las, 3);
	polska.addRelationship(lane3);//C A
	
	Lane lane4(las, rondo, 3);
	polska.addRelationship(lane4);// A D
	
	Lane lane5(rynek, rondo, 1);
	polska.addRelationship(lane5);// C D
	
	Lane lane6(rynek, browarWarka, 2);
	polska.addRelationship(lane6);// C E
	
	Lane lane7(rondo, pubULecha, 6);
	polska.addRelationship(lane7);//D F
	
	Lane lane8(rondo, browarWarka, 2);
	polska.addRelationship(lane8);//D E
	
	Lane lane9(browarWarka, ryneczekLidla, 1);
	polska.addRelationship(lane9);// E G
	
	Lane lane10(browarWarka, zlotePole, 1);
	polska.addRelationship(lane10);// E B
	
	Lane lane11(pubULecha, ryneczekLidla, 9);
	polska.addRelationship(lane11);//F G
	
	Lane lane11_1(pubULecha,browarBarka,9 );
	polska.addRelationship(lane11_1);


	//multiSink and multiSource algorithm test
	std::vector<std::shared_ptr<Node>> sinks;
	std::vector<std::shared_ptr<Node>> sources;
	sinks.push_back(ryneczekLidla);
	sinks.push_back(browarBarka);
	sources.push_back(las);
	sources.push_back(pubPodKogutem);
	std::cout<<polska.edmondsKarpManyToMany(sources,sinks);
	std::cout<<std::endl;
	std::cout<<polska.dinic(sources,sinks)<<std::endl;
	//g++ ./main.cpp ./Country.cpp ./Node.cpp ./Lane.cpp ./Brewery.cpp ./Pub.cpp ./Field.cpp .\Intersection.cpp
	
	/*Country kingdom;
	kingdom.createBrewery("Brewery_1", 69, 420, 10, 55);
	kingdom.createField("Field_1", 2137, 466, 450, 10);
	kingdom.createPub("Pub_1", 420, 156, 10);
*/
	//kingdom.find("Pub_1")->print();
	return 0;
}
