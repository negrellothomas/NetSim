#include "network.h" 
#include "random.h"
#include <iostream>


void Network::resize(const size_t& new_size){
		values.clear();
		values.resize(new_size);
		RandomNumbers RNG(0);
		RNG.uniform_double(values, 0, values.size());
	
}

bool Network::add_link(const size_t& a, const size_t& b){
	
	if(a==b or a>=values.size() or b>=values.size() or a<0 or b<0){ //conditions pour qu'un lien soit créé
		return false;
	}
	
	for(auto element : neighbors(a)){
		if(element==b){
			return false;
		}
	}	
			
	links.insert({a,b}); //creation des liens
	links.insert({b,a});
	bool alink(false);
	bool blink(false);
			
	for(auto element : neighbors(a)){ //verifie si les liens ont été créés
		if(element==b){
			alink=true;
		}
	}	
	for(auto element : neighbors(b)){
		if(element==a){
			blink=true;
		}
	}
	
	if(alink and blink){
		return true;
	}
	else{
		return false;
	}
}

size_t Network::random_connect(const double& mean_deg){
	
	for(auto it = links.begin(); it != links.end(); ){ //erase les liens dans la multimap
		it = links.erase(it);
    }
		
	RandomNumbers RNG(0);
	std::vector<int> nodes_links(values.size()); //nombre de liens que crée chaque noeuds
	RNG.poisson(nodes_links,mean_deg);
	int all_links(0); //compte le nombre de liens
	
	for(size_t i(0); i < nodes_links.size();++i){
		bool created_all_links(nodes_links[i] == 0);
		int links_iterator(0);
				
		while (!created_all_links){//crée un lien tant que le noeud n'a pas créé le nombre de noeuds qu'il doit avoir
			if(add_link(i,RNG.uniform_double(0,values.size()-1))){
				++links_iterator;
				++all_links;
				if	(nodes_links[i] == links_iterator){
					created_all_links = true;
				}
			}
		}
	}
	
	return all_links;
}	

size_t Network::set_values(const std::vector<double>& vecteur){
	
	
	resize(vecteur.size());
	for(size_t i(0); i < vecteur.size(); ++i){
		values[i]=vecteur[i];
	}
	return values.size();
}

size_t Network::size() const{
		return values.size();
}

size_t Network::degree(const size_t & _n) const{
		return links.count(_n);
}

double Network::value(const size_t &_n) const{
	if(_n<= values.size()){
		return values[_n];
	}
	else{
		return 666; //error
	}
}

std::vector<double> Network::sorted_values() const{

	std::vector<double> sorted(values);
	std::sort(sorted.begin(), sorted.end(), std::greater<double>());
	return sorted;
	
}

std::vector<size_t> Network::neighbors(const size_t& a) const{
	
	std::vector<size_t> linked_to_a;
	
	for (auto element: links){
			if (element.first == a){
					linked_to_a.push_back(element.second);
			} 
	}
	
	return linked_to_a;
}

