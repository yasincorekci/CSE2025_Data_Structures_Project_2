// yasincorekci 150119858 - asaftalhagultekin 150119678 - alicetinkaya 150119606 //

/*
REFERENCES
https://www.geeksforgeeks.org/dijkstras-algorithm-for-adjacency-list-representation-greedy-algo-8/
https://www.codingame.com/playgrounds/1608/shortest-paths-with-dijkstras-algorithm/keeping-track-of-paths
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>

#define INF 9999

struct node
{
    char source_node_id;
    char dest_node_id;
    int weight;
    struct node *next_node;
    struct node *next_list;
};
typedef struct node Node;

struct heap
{
    int size;
    int *distance;
    char *min_heap;
};
typedef struct heap Heap;

//Heap variables
Heap *min_heap;
int heap_size;

//Adj List
Node *head;

//number of verticices
int verticies;


Node *readInputFile();
void dijkstra(Node *head , char src , char dst , Node* paths[]);
void updateHeap(Heap *heap_ptr , char dest);
int findIndex(Heap *heap_ptr,Node *node_ptr ,char v);
char extractMin(int final_dist[]);
void heapify(int index);
int isEmpty();
Node *copyList(Node* path_ptr);

int main(){

    setlocale(LC_ALL , "Turkish");


    int menu_option = 0;
    while(menu_option != -1){
        printf("1.Read File\n2.Show adjacency matrix/list\n3.Find shortest path\n4.Exit\n");
        printf("Please choose an option: ");
        scanf("%d" , &menu_option);

        switch (menu_option)
        {
        case 1: //1.Read File
        {
            //Read Input file and create Adj List
            head = readInputFile();

            //Calculating number of verticies in the graph
            Node *node_ptr = head;
            verticies = 0;
            while (node_ptr != NULL)
            {
                verticies++;
                node_ptr = node_ptr->next_list;
            }

            break;
        }
        case 2: //2.Show adjacency matrix/list
        {
            printf("\nAdjaceny List -----------------\n");
            Node *node = head;
            Node *list = head;
            while(list != NULL){

                char source_id = list->source_node_id;
                source_id = toupper(source_id);
                printf("%c: " , source_id);

                node = list;
                while(node != NULL){

                    char dest_id = node->dest_node_id;
                    dest_id = toupper(dest_id);
                    int weight = node->weight;
                    printf("%c,%d " , dest_id ,weight );

                    node = node->next_node;
                }
                list = list->next_list;
                printf("\n");
            }
            printf("---------------------------------\n");
            break;
        }
        case 3: //3.Find shortest path
        {

            char src , dst;
            printf("\nEnter the Source vertex: ");
            scanf(" %c" , &src );
            printf("Enter the Destination vertex: ");
            scanf(" %c" , &dst);

            printf("-------------------------------\n");


            Node* paths[verticies]; //Paths array to hold the list for path to be taken to reach specified vertex
            /*Keeps all the shortest paths from src vertex to all verticies
              Same order as Adj list
            */
            dijkstra(head , src , dst , paths);

            Node *node_iter;
            int node_index = findIndex(NULL , head , dst); //Path List to be taken , Since its in same order as Adj List
            int cost = 0; // Total Cost to go from src to dst vertex
            node_iter = paths[node_index];
            printf("Path from %c to %c \n" , toupper(src) , toupper(dst));
            while(node_iter != NULL){
                printf("%c" , toupper(node_iter->dest_node_id));
                if(node_iter->next_node != NULL){
                    printf("->");
                }
                cost = cost + node_iter->weight;
                node_iter = node_iter->next_node;
                if(node_iter == NULL){
                    printf("    Cost: %d\n" , cost);
                }
            }

            printf("-------------------------------\n");

            break;
        }
        case 4: //4.exit
        {
            menu_option = -1;
            break;
        }
        default:
            printf("Please enter correctly...");
            break;
        }

    }


}

Node *readInputFile(){

    int eof;
    Node *head = NULL;

    //Read file
    printf("Name of the input file to be read(including extension): ");
    char input_path[20] = {0};
    scanf("%s" , input_path);
    printf("\n");

    FILE *fp;
    fp = fopen(input_path , "r");
    //fp = fopen("input.txt" , "r");
    if(fp == NULL){
        printf("\nFile can not be found...\n");
    }

    Node *node_ptr , *list_ptr;
    char line[50] = {0};
    while(1){

        char source;
        char dest;
        int weight;
        eof = fscanf(fp , "%c,%c,%d\n" , &source , &dest , &weight );
        source = tolower(source);
        dest = tolower(dest);

        if(eof == EOF){
            break;
        }

        //Create new node to be added to the list
        Node *node = malloc(sizeof(Node));
        node->source_node_id = source;
        node->dest_node_id = dest;
        node->weight = weight;
        node->next_node = NULL;
        node->next_list = NULL;

        //Since graph is undirected add an edge from dst to src
        Node *node_2 = malloc(sizeof(Node));
        node_2->source_node_id = dest;
        node_2->dest_node_id = source;
        node_2->weight = weight;
        node_2->next_node = NULL;
        node_2->next_list = NULL;

        //If the list is empty make newly created node the head
        if(head == NULL){
            head = node;
            node->next_list = node_2;
        }
        else{ //If list is not empty...
            node_ptr = list_ptr = head;
            //Search the correct list to insert the new node
            while(list_ptr != NULL){
                if(list_ptr->source_node_id == source){
                    break;
                }
                list_ptr = list_ptr->next_list;
            }
            //if list does not exsists , make the new node head of the list and add it to master list
            if(list_ptr == NULL){
                list_ptr = head;
                while(list_ptr != NULL){
                    if(list_ptr->next_list == NULL){
                        break;
                    }
                    list_ptr = list_ptr->next_list;
                }
                list_ptr->next_list = node;
                list_ptr = list_ptr->next_list;
            }
            else{
                //go to the end of the list
                node_ptr = list_ptr;
                while(node_ptr != NULL){
                    if(node_ptr->next_node == NULL){
                        break;
                    }
                    node_ptr = node_ptr->next_node;
                }
                node_ptr->next_node = node;
            }



            //Do the same thing for other node , so if A,B,2 then A has connection to B wtih weight 2
            //B also has a connection to A with weight 2

            node_ptr = list_ptr = head;
            //Search the correct list to insert the new node
            while(list_ptr != NULL){
                if(list_ptr->source_node_id == dest){
                    break;
                }
                list_ptr = list_ptr->next_list;
            }
            //if list does not exsists , make the new node head of the list and add it to master list
            if(list_ptr == NULL){
                list_ptr = head;
                while(list_ptr != NULL){
                    if(list_ptr->next_list == NULL){
                        break;
                    }
                    list_ptr = list_ptr->next_list;
                }
                list_ptr->next_list = node_2;
                list_ptr = list_ptr->next_list;
            }
            else{
                //go to the end of the list
                node_ptr = list_ptr;
                while(node_ptr != NULL){
                    if(node_ptr->next_node == NULL){
                        break;
                    }
                    node_ptr = node_ptr->next_node;
                }
                node_ptr->next_node = node_2;
            }

        }

    }

    return head;

}

void dijkstra(Node *head , char src , char dst , Node* path_array[]){

    int i;

    //Path list , to keep track of the shortest path to be taken
    Node *temp = head;
    Node *paths[verticies];
    //Initialize paths array
    for(i = 0; i < verticies ; i++){

        Node *new_node = malloc(sizeof(Node));
        new_node->source_node_id = src;
        new_node->dest_node_id = src;
        new_node->next_node = NULL;
        new_node->next_list = NULL;
        new_node->weight = 0;
        paths[i] = new_node;
    }


    min_heap = malloc(sizeof(Heap)); // Create min heap pointer
    int array_dist[verticies];
    char array_minHeap[verticies];
    //int final_dist[verticies];

    min_heap->distance = array_dist;
    min_heap->size = verticies;
    min_heap->min_heap = array_minHeap;

    Node *node_ptr = head;
    for(i = 0; i < verticies ; i++){
        min_heap->distance[i] = INF; //Dıstances from src vertex is INF
        min_heap->min_heap[i] = node_ptr->source_node_id;

        if(node_ptr->source_node_id == src){
            min_heap->distance[i] = 0; //Distance from src vertex to itself is 0
        }

        node_ptr = node_ptr->next_list;
    }

    updateHeap(min_heap , src);

    while(!isEmpty(min_heap->min_heap)){

        //Extreact the vertex with minimum distance value , that is root
        //int src_index = findIndex(min_heap , NULL , src);
        int extracted_vertex_distance = min_heap->distance[0];
        char extracted = extractMin(min_heap->distance);

        //Find the node in Adj List and Traverse all the neighbor nodes of extracted node (AdjList)
        node_ptr = head;
        while(node_ptr != NULL){
            if(node_ptr->source_node_id == extracted){
                break;
            }
            node_ptr = node_ptr->next_list;
        }
        //List of the neighbors of extracted vertex is found , iterate through it
        while(node_ptr != NULL){

            char dest = node_ptr->dest_node_id;
            int  dest_index = findIndex(min_heap , NULL, dest);
            //If dest is not in heap it will return -1
            if(dest_index == -1){
                node_ptr = node_ptr->next_node;
                continue;
            }
            //If shortest distance to extracted vertex not finalized yet, and distance to dest through extracted vertex is
            //less than its previously calculated distance
            if(extracted_vertex_distance != INF && node_ptr->weight + extracted_vertex_distance < min_heap->distance[dest_index])
            {
                min_heap->distance[dest_index] = extracted_vertex_distance + node_ptr->weight;
                updateHeap(min_heap , dest);

                //Path updating
                //replace the current path to dest_vertex with the path to the current vertex that we are checking neighbors of
                //plus path to dest_vertex
                int current_vertex_index = findIndex(NULL , head , extracted);
                //Copy the path to the current vertex that we are checking neighbors of
                Node *new_path = copyList(paths[current_vertex_index]);
                //Create the new node
                Node *new_path_node = malloc(sizeof(Node));
                new_path_node->dest_node_id = dest;
                new_path_node->next_list = NULL;
                new_path_node->next_node = NULL;
                new_path_node->source_node_id = extracted;
                new_path_node->weight = node_ptr->weight;

                //So lets say the previous path A to E was A->E , but we found out that A to E though B is costs less then A->E
                //So new path becomes path A to B that is A->B plus E , final path becomes A->B->E

                Node *iter = new_path;
                while(iter != NULL){
                    if(iter->next_node == NULL){
                        break;
                    }
                    iter = iter->next_node;
                }

                iter->next_node = new_path_node;

                //Find the path list for dest vertex
                dest_index = findIndex(NULL , head , dest);
                free(paths[dest_index]);
                //Replace the path
                paths[dest_index] = new_path;

            }

            node_ptr = node_ptr->next_node;
        }


    }

    for(i = 0; i < verticies ; i++){
        path_array[i] = paths[i];
    }

}

//Makes a copy of a given linked list , Used for calculating path taken
Node *copyList(Node* path_ptr){

    Node *dest_head , *dest_current , *src_current;
    dest_head = dest_current = src_current = NULL;
    src_current = path_ptr;

    while(src_current != NULL){
        Node *new_node  = malloc(sizeof(Node));

        new_node->dest_node_id = src_current->dest_node_id;
        new_node->source_node_id = src_current->source_node_id;
        new_node->weight = src_current->weight;
        new_node->next_list = NULL;
        new_node->next_node = NULL;

        if(dest_head == NULL){
            dest_head = new_node;
            dest_current = dest_head;
        }
        else{
            dest_current = dest_head;
            while(dest_current != NULL){
                if(dest_current->next_node == NULL){
                    break;
                }
                dest_current = dest_current->next_node;
            }
            dest_current->next_node = new_node;
        }


        src_current = src_current->next_node;

    }

    return dest_head;

}

//Updates the heap and sitances of verticices to keep the list in heap form
void updateHeap(Heap *heap_ptr , char dest){

    //Find in heap
    int dest_index;
    int i;
    for(i = 0 ; i < heap_ptr->size ; i++ ){
        if(dest == heap_ptr->min_heap[i]){
            dest_index = i;
            break;
        }
    }

    while(heap_ptr->distance[dest_index] < heap_ptr->distance[(dest_index - 1) / 2]){

        //Swap with parent
        char temp_char;
        int temp_dist;

        //Save parent
        temp_char = heap_ptr->min_heap[(dest_index - 1) / 2];
        temp_dist = heap_ptr->distance[(dest_index - 1) / 2];

        heap_ptr->min_heap[(dest_index - 1) / 2] = heap_ptr->min_heap[dest_index ];
        heap_ptr->distance[(dest_index - 1) / 2] = heap_ptr->distance[dest_index ];

        heap_ptr->min_heap[dest_index] = temp_char;
        heap_ptr->distance[dest_index] = temp_dist;

        //Move to parent index
        dest_index = (dest_index - 1) / 2;

    }



}

//Finds the index of a given vertex in a list
int findIndex(Heap *heap_ptr, Node * node_ptr, char v)
{

    int vertex_index = -1;
    int i;

    if (node_ptr == NULL)
    {
        for (i = 0; i < heap_ptr->size; i++)
        {
            if (heap_ptr->min_heap[i] == v)
            {
                vertex_index = i;
                break;
            }
        }
    }
    else if (heap_ptr == NULL)
    {
        i = 0;
        Node *temp_ptr = head;
        while(temp_ptr != NULL){
            if(temp_ptr->source_node_id == v){
                vertex_index = i;
                break;
            }
            i++;
            temp_ptr = temp_ptr->next_list;
        }
    }

    return vertex_index;
}

//Checks if min heap is empty
int isEmpty(){

    int empty = 1;
    int i;

    for(i = 0; i < verticies ; i++){
        if(min_heap->min_heap[i] != NULL){
            empty = 0;
        }
    }

    return empty;


}

//Extracts min from min heap
char extractMin(int final_dist[]){

    //Save root & distance
    char root = min_heap->min_heap[0];
    int dist = min_heap->distance[0];

    //Replace root with last node in heap
    char last_node = min_heap->min_heap[min_heap->size - 1];
    min_heap->min_heap[0] = last_node;
    min_heap->min_heap[min_heap->size - 1] = NULL;

    min_heap->distance[0] = min_heap->distance[min_heap->size - 1];
    min_heap->distance[min_heap->size - 1] = NULL;

    min_heap->size--;
    heapify(0);

    return root;

}

//to keep the heap in heap from after extracted a node
void heapify(int index){

    int smallest = index;
    int left = (2*index) + 1;
    int right = (2*index) + 2;


    //Check wheter left or right node is smaller that the extracted node(root)
    if(left < min_heap->size && min_heap->distance[left] < min_heap->distance[smallest]){
        smallest = left;
    }
    if(right < min_heap->size && min_heap->distance[right] < min_heap->distance[smallest]){
        smallest = right;
    }


    if(smallest != index){

        int temp_distance;
        char temp_char;

        temp_distance = min_heap->distance[index];
        temp_char = min_heap->min_heap[index];

        min_heap->distance[index] = min_heap->distance[smallest];
        min_heap->min_heap[index] = min_heap->min_heap[smallest];

        min_heap->distance[smallest] = temp_distance;
        min_heap->min_heap[smallest] = temp_char;

        heapify(smallest);

    }




}
