#include <iostream>
#include "Country.h"

int main() {
	Country polska;	

	std::vector<std::shared_ptr<Node>> sinks;
	std::vector<std::shared_ptr<Node>> sources;

	auto a = polska.createField(2,4, 1, 4, 4);
	a->setName("a");
	auto b = polska.createField(2,3, 1, 4, 4);
	b->setName("b");
	auto c = polska.createField(2,2, 1, 4, 4);
	c->setName("c");

	auto e = polska.createBrewery(7,1, 4, 5, 155);
	e->setName("e");
	auto f = polska.createBrewery(7,1, 4, 5, 155);
	f->setName("f");
	auto g = polska.createBrewery(7,1, 4, 5, 155);
	g->setName("g");

	auto h = polska.createPub(1,1, 2, 4);
	h->setName("h");
	auto i = polska.createPub(3,2, 3, 5);
	i->setName("i"); 
	
	Lane lane1(a,e ,1, 1);
	polska.addRelationship(lane1);

	Lane lane2(b, f,1,1);
	polska.addRelationship(lane2);
	
	Lane lane3(b, g, 3,1);
	polska.addRelationship(lane3); 
	
	Lane lane4(c, f, 1,4);
	polska.addRelationship(lane4); 
	
	Lane lane5(e, i, 1,1);
	polska.addRelationship(lane5); 
	
	Lane lane6(f,h,2,1);
	polska.addRelationship(lane6);

	Lane lane7(f,i, 3,1);
	polska.addRelationship(lane7);
	
	Lane lane8(g, i,4,1);
	polska.addRelationship(lane8);
	
//	sources.push_back(pubMentzena);
//	sinks.push_back(pubPodKogutem);

//	std::cout<<polska.dinic(sources,sinks)<<std::endl;
//	polska.cycleCancelling(sinks,sources);

//	std::cout<<polska.mcmf(sources,sinks);


	for(auto el:polska.nodeVector){
		if (auto derived = std::dynamic_pointer_cast<Field>(el)) {
			sources.push_back(el);	
		}
	}
	for(auto el:polska.nodeVector){
		if (auto derived = std::dynamic_pointer_cast<Brewery>(el)) {
			sinks.push_back(el);	
			} 

	}
	std::cout<<polska.edmondsKarpManyToMany(sources,sinks)<<std::endl;

	sources.clear();
	sinks.clear();

	for(auto el:polska.nodeVector){
		if (auto derived = std::dynamic_pointer_cast<Brewery>(el)) {
			sources.push_back(el);	
		}
	}
	for(auto el:polska.nodeVector){
		if (auto derived = std::dynamic_pointer_cast<Pub>(el)) {
			sinks.push_back(el);	
			} 

	}
	std::cout<<polska.edmondsKarpManyToMany(sources,sinks)<<std::endl;
	
	return 0;
}








