# netmodel: c++ social network model

The NETMODEL is designed to simulate large scale social networks. The main feature of the model is its ability to code specific ‘aspects’ that influence the way agents create and destroy relationships.

The NETMODEL implements attribute-oriented-aspects (such as the agents’ color, or the agents ‘educational level’) as well as geographical distance, offering different ways of calculating distance in the 2-dimensional space. 

# Setup

The model is implemented in C++ (tested on Windows (vc & gcc) and Linux (gcc)). 

The parameters of the model are configured at the netmodel.xml, where startup attributes, resource creation aspect parameters, as well as output columns and intervals can be specified.
