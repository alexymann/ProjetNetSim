#include "network.h"
#include "random.h"

//les nodes sont reliées entre elles par les indices/positions et le tableau values contient les valeurs de ces nodes

/*! Resizes the list of nodes (\ref values) */
void Network::resize (const size_t& t) {
	std::vector <double> new_tab (t);
	
	RandomNumbers* n = new RandomNumbers();
	
	n -> normal(new_tab);
	if (not(values.empty())) {
		values.clear();
	}
	values = new_tab;
	
	for (std::multimap <size_t, size_t>::const_iterator it =links.begin(); it != links.end(); ++it) {
		if (it -> first > values.size() or it -> second > values.size()) {
			links.erase (it);
		}
	}
}

/*! Adds a bidirectional link between two nodes
  @param[in] a,b the indexes if the two nodes 
  @param[out] success the link was succesfully inserted
 */
bool Network::add_link (const size_t& t1, const size_t& t2) {
	
	std::pair <std::multimap <size_t, size_t>::const_iterator, std::multimap <size_t, size_t>::const_iterator> ret;
	ret = links.equal_range(t1);
	
	for (std::multimap <size_t, size_t>::const_iterator it =ret.first; it != ret.second; ++it) {
		if (it->second == t2) {
			return false;
		}
	}
	
	ret = links.equal_range (t2);
	for (std::multimap <size_t, size_t>::const_iterator it=ret.first; it != ret.second; ++it) {
		if (it -> second == t1) {
			return false;
		}
	}
	
	if ( (t1 < 0) or (t2 < 0) or (t1 > values.size()-1) or (t2 > values.size()-1) or (t1 == t2)) {
		return false;
	} else {
		links.insert (std::pair <size_t, size_t> (t1, t2));
		links.insert (std::pair <size_t, size_t> (t2, t1));
		return true;
	}
}

/*! Creates random connections between nodes: each node *n* will be linked with *degree(n)* other nodes (randomly chosen) where *degree(n)* is Poisson-distributed.

  All previous links are cleared first.
  @param mean_deg the average of the Poisson distribution.
 */

size_t Network::random_connect(const double& mean_deg) {
	
	links.clear();
	
	RandomNumbers* n = new RandomNumbers();
	size_t Degree (n -> poisson (mean_deg));
	size_t link_nb(0);
	
	std::vector <int> indices (values.size());
		
	for (size_t i(0); i < values.size(); ++i) {
		n -> uniform_int (indices, 0, indices.size()-1);
		for (size_t j(0); j < Degree; ++j) {
			if (add_link (i, indices[j])) {
				++ link_nb;
			}
		}
	}
	
	return link_nb;
}

/*! Resets all node values.
  @param[in] vector of new node values
  @param[out] number of nodes succesfully reset
 */
size_t Network::set_values (const std::vector <double>& table) {
	
	if (table.size() < values.size()) {
		for (size_t i(0); i < table.size(); ++i) {
			values[i] = table[i];
		}
		return table.size();
	} else {
		for (size_t i(0); i < values.size(); ++i) {
			values[i] = table[i];
		}
		return values.size();
	}
}


/*! Number of nodes */
size_t Network::size() const {
	if (!(values.empty())) { 
		return values.size();
	} else {
		std::cerr << "Le tableau values ne contient pas de valeurs." << std::endl;
		return 1;
	}
}
    
/*! Degree (number of links) of node no *n* */
size_t Network::degree(const size_t &_n) const {
	
	size_t nbNodes (0);
	
	for (std::multimap <size_t, size_t>::const_iterator I = links.begin(); I != links.end(); ++I) {
		if ((*I).first == _n) {
			++nbNodes;
		}
	}
	
	if (nbNodes == 0) {
		std::cerr << "This node does not have any links." << std::endl;
	} 
	return nbNodes;
}
    
    
/*! Value of node no *n* */
double Network::value(const size_t &_n) const {
    return values[_n];
}
    
/*! All node values in descending order */
std::vector<double> Network::sorted_values() const {
	std::vector <double> sorted (values);
	
	std::sort (sorted.begin() , sorted.end());
	std::reverse (sorted.begin(), sorted.end());
	return sorted;
}
    
    
/*! All neighbors (linked) nodes of node no *n* */
std::vector<size_t> Network::neighbors(const size_t& n) const {
	
	std::vector <size_t> resultats;
	
	for (std::multimap <size_t, size_t>::const_iterator I = links.begin(); I != links.end(); ++I) {
		if ((*I).first == n) {
			resultats.push_back((*I).second);
		}
	}
	
	if (resultats.empty()) {
		std::cerr << "This node did not link with any other nodes." << std::endl;
	}
	return resultats;
}
		
		
