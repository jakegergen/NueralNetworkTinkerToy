#include <vector>
#include <iostream>

class Node;
class Edge{
public:
	float w;
	Node * to;
	Node * from;

	Edge(){
		to = 0;
		from = 0;
		w=0;
	}
	Edge(float weight)
	{
	    w = weight;
		to = 0;
		from = 0;
	}
	Edge(float weight, Node * fromm, Node* too)
	{
	   w = weight;
	   to = too;
	   from = fromm;
	}
	
};


class Node {
public:
	Node(){
		a=0;
		b=0;
	}

	float activate(){
		float sum = 0.0;

		for(int i =0;i<in.size();i++){
			sum += in[i]->w * in[i]->from->a;
		}

		a = sum + b;
	}

	 void addIn(Edge* e){
	 	in.push_back(e);
	 }
	 void addOut(Edge* e){
	 	out.push_back(e);
	 }

	 float a;
	 float b;
	 std::vector<Edge*> in;
	 std::vector<Edge*> out;
};

class Layer{
public:
	Layer(){

		next = 0;
		size = 0;
	}

	void addNode(Node* n){
		nodes.push_back(n);
	}

	void setActivation(float input[], int inputsize){
		for(int i =0; i < inputsize; i++){
			nodes.at(i)->a = input[i]; //manually set the value of the input layer 
		}
	}

	void ActivateLayer(){
		for(int i =0; i < nodes.size(); i++){
			nodes[i]->activate();
		}
	}

	void printLayerActivation(){
		for(int i =0; i < nodes.size(); i++){
			std::cout << "a: "<<nodes.at(i)->a << " w1: " <<  nodes.at(i)->in.at(0)->w << " w2: "<<nodes.at(i)->in.at(1)->w <<" b:"<<nodes.at(i)->b <<'\n';
		}
	}

	void trainLayer(float actual[],int actsize){
		//if activation is pos and actual is pos then correct, if activation is neg and act is neg then correct, else incorrect, 0 neg
		Node* n;
		float incomingActivation = 0.0;
		for(int i =0; i<actsize;i++)
		{
			n = nodes.at(i);
			if(n->a*actual[i] <= 0){
				//incorrect answer change weights and bias
				for(int j =0;j<n->in.size();j++){
					//cylce through each incomming edge weight of n and adjust according w = w_ + x*y
					Edge *tmp = n->in.at(j);
					
					//tmp is the current incoming edge that is to be adjusted

					incomingActivation = (tmp->from->a);
					//my weight is equal to my previous weight sumed with ( the incoming activation * what the activation should have been)
					tmp->w = tmp->w + incomingActivation*actual[i];
				}
				//adjust the bias to the previous bias + what this nodes activation SHOULD have been
				n->b = n->b + actual[i];
			}
		}
		
	}

	Layer *next;
	std::vector<Node*> nodes;
	int size;
};




class Network{
	Network(){
		layer_count = 0;
	}
	//connects all outgoing edges from L1 to the incoming edges of L2
	void connectLayers(Layer* L1, Layer* L2){
		Node* left, *right;
		Edge* eptr;//points to the current edge being developed
		for(int i = 0;i<L1->nodes.size();i++){
			left = L1->nodes[i];
			for(int j = 0; j<L2->nodes.size();j++){

				right = L2->nodes.at(j);

				//1. Get an un-used edge, and initilize it
				eptr = getEdge();//getEdge is a utility function, that keeps track of the book keeping of used/un-used nodes
				eptr->to = right;
				eptr->from = left;
				//eptr->w = random_float()

				//2. give the edge ptr to the left nodes out container. give the edge ptr to the right nodes in container
				left->out.push_back(eptr);
				right->in.push_back(eptr);
				
			}
		}
	}
	
	void addLayer(Layer *n){
		//1. connect last layer in "layers" TO n, 
		//2. add n to "layers"
		//3. increment layer_count

		//if we already have an input layer then we connect our new layer to the input layer, or to what ever the last layer is in "layers"
		//basically this if protects from the input layer case
		if(layers.size() > 0){
			Layer *last = layers.at(layers.size() - 1);
			connectLayers(last, n);
		}
		layers.push_back(n);
		layer_count++;
	}
	//feed input to the input layer, then activate the hidden layers on the input layer activation
	void activateNetwork(float input[],int inputsize){
		Layer *i = layers.at(0);
		for(int i =0; i < inputsize; i++){
			i->nodes.at(i)->a = input[i]; //manually set the value of the input layer 
		}
		int index = 1;
		while(index != layer_count){
			i = layers.at(i);
			i->ActivateLayer();
			index++;
		}
		i = layers.at(i-1);
		i->printLayerActivation();

	}
	std::vector<Layer *> layers;
	int layer_count;
}
// CLASS'S


// MEMORY MANAGEMENT
Layer global_layers[3];
int layer_index = 0;

Node global_nodes[10];
int node_index = 0;

Edge global_edges[10];
int edge_index = 0;

Layer* getLayer(){
	//returns a pointer to an un-used layer 
	return global_layers + layer_index++;
}

Node * getNode(){
	//returns a pointer to an un-used node
	return global_nodes + node_index++;
}

Edge * getEdge(){
	//returns a pointer to an un-used edge
	return global_edges + edge_index++;
}
// MEMORY MANAGEMENT



int main(){
	Layer *input = getLayer();
	Layer *perceptron = getLayer();


	//create input layer and add two nodes
	input->addNode(getNode());
	input->addNode(getNode());

	//this is our computation layer with only one node... a.k.a   a perceptron
	perceptron->addNode(getNode());
	
	//connect our computation layer with the input layer so the computation has access to data
	connectLayers(input,perceptron);

	

}