# A-star-pathfinding
A* algorithm simulation

Nakon prevodjenja komandom "make", program se moze pokrenuti na sledeci nacin:

./a-star-simulation	 nodes_number	 graph_sparsity	 [-p (plane graph)]

gde je:

 "a-star-simulation" - naziv prevedenog programa (pogledati Makefile);
 
 "nodes_number" - broj cvorova grafa;
 
 "graph_sparsity" - predstavlja koeficijent gustine grana random kreiranog grafa; 
 
 [-p] - opcioni parametar koji sugerise da graf bude ravanski tj da svi cvorovi leze u jednoj ravni (Z = 0).



Primer pokretanja:

./a-star-simulation 30 0.2
