#include <iostream>
#include "Country.h"

int main() {
	Country polska;	

	std::vector<std::shared_ptr<Node>> sinks;
	std::vector<std::shared_ptr<Node>> sources;
	for(auto el:polska.nodeVector){
		if (auto derived = std::dynamic_pointer_cast<Field>(el)) {
			sources.push_back(el);	
		}
	}
	for(auto el:polska.nodeVector){
		if (auto derived = std::dynamic_pointer_cast<Pub>(el)) {
			sinks.push_back(el);	
		}
	}
	
	std::cout<<polska.edmondsKarpManyToMany(sources,sinks)<<std::endl;;
	std::cout<<polska.dinic(sources,sinks)<<std::endl;
	
	return 0;
}
