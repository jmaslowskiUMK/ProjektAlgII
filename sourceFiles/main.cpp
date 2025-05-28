#include <iostream>
#include "Country.h"

int main() {
	Country polska;	

	std::vector<std::shared_ptr<Node>> sinks;
	std::vector<std::shared_ptr<Node>> sources;
/*	for(auto el:polska.nodeVector){
		if (auto derived = std::dynamic_pointer_cast<Field>(el)) {
			sources.push_back(el);	
		}
	}
	for(auto el:polska.nodeVector){
		if (auto derived = std::dynamic_pointer_cast<Pub>(el)) {
			sinks.push_back(el);	
			} 

	} 
	*/


	auto pubMentzena = polska.createPub(1,1, 2, 4);//A
	pubMentzena->setName("pubMentzena");
	auto lysePole = polska.createField(2,1, 1, 4, 4);//B 
	lysePole->setName("lysePole");
	auto pubULecha = polska.createPub(3,2, 3, 5);//C
	pubULecha->setName("pubULecha"); 
	auto pubPodKogutem = polska.createPub(4,4, 1, 4);//D
	pubPodKogutem->setName("pubPodKogutem");
	auto zlotePole = polska.createField(5,0, 0, 5, 5);//E
	zlotePole->setName("zlotePole");
	auto las = polska.createField(6,3, 2, 41, 5);//F
	las->setName("las");
	auto browarWarka = polska.createBrewery(7,1, 4, 5, 155);//G
	browarWarka->setName("browarWarka");

	Lane lane1(pubMentzena, lysePole, 10, 1);
	polska.addRelationship(lane1);//A B

	Lane lane2(lysePole, pubULecha,10,1);
	polska.addRelationship(lane2);//B C
	
	Lane lane3(lysePole, pubPodKogutem, 5,1);
	polska.addRelationship(lane3);//B D 
	
	Lane lane4(pubULecha, zlotePole, 7,4);
	polska.addRelationship(lane4);//C E 
	
	Lane lane5(lysePole, zlotePole, 5,3);
	polska.addRelationship(lane5);//D E 
	
	Lane lane6(zlotePole,las,15,1);
	polska.addRelationship(lane6);//E F

	Lane lane7(pubULecha,browarWarka, 10,1);
	polska.addRelationship(lane7);//C G
	
	Lane lane8(pubPodKogutem, browarWarka,10,1);
	polska.addRelationship(lane8);//D G
	
	Lane lane9(browarWarka, zlotePole, 10,1);
	polska.addRelationship(lane9);//G E
	
	sources.push_back(pubMentzena);
	sinks.push_back(las);

	std::cout<<polska.edmondsKarpManyToMany(sources,sinks)<<std::endl;;
	std::cout<<polska.dinic(sources,sinks)<<std::endl;
	polska.cycleCancelling(sinks,sources);
	std::cout<<"peax";
	return 0;
}








