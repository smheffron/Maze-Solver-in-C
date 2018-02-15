//CS3050:FINAL PROJECT
//Shelby Heffron, Jason Tersreau, Christian Caldwell
//Completed 12/6/17

#include "../include/graph.h"
#include "../include/input_error.h"
#include "../include/vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef int bool;
#define true 1
#define false 0


//this struct stores array notation size of the maze double array
typedef struct point{
    int col;
    int row;
}point;

//this struct is used for queue operations
typedef struct node
{
	struct vertex* vertex;
	struct node* next;
}node;

//this struct is used to store array coordinates of a certain element in the maze array
typedef struct coord{
    int x;
    int y;
}coord;

void freeStack(node* stack);

void freeArray(char** maze, point* rowcolInfo);

int getDistances(point* rowcolInfo, char** maze);

bool isAdj(char letter, char** maze, point* rowcolInfo, coord* coordinates);

void dfs(struct graph* grph, char** maze, point* rowcolInfo, char** sol, coord* coord, int robot);

node* add_to_front(node* head, struct vertex* vertex);

//prototypes
struct vertex* remove_from_front(node** head);

node* add_to_back(node* head, struct vertex* vertex);

void printDistances(struct graph* grph, FILE* fp);

void bfs(struct graph* grph,char** maze, point* rowcolInfo, char** sol, coord* coordinates);

char** parseInput(FILE* fp, point* rowcolInfo);

coord* findCoordS(coord* coordinates, char** maze, point* rowcolInfo);

void printSolution(char** sol, point* rowcolInfo);

void matrixToGraph(struct graph* grph, char** maze, point* rowcolInfo, int robot);

bool isReachableE(struct graph* grph, point* rowcolInfo, char** maze);

coord* findCoordE(coord* coordinates, char** maze, point* rowcolInfo);

coord* findCoordF(coord* coordinates, char** maze, point* rowcolInfo);

coord* findCoordL(coord* coordinates, char** maze, point* rowcolInfo);

bool isReachableL(struct graph* grph, point* rowcolInfo, char** maze);

	
int main(int argc, char** argv) 
{
	//check for correct command line args

	if(argc!=2)
	{
		printf("\nWrong command line args: <./final inputfile>\n");
		exit(INCORRECT_NUMBER_OF_COMMAND_LINE_ARGUMENTS);
	}

	int file_check = 1;

	char* inputFile = *(argv+1);

    FILE* fp;

    //check for correct file opening

    if((fp=fopen(inputFile, "r"))==NULL)
    {
    	printf("\nInput file failed to open\n");
        exit(INPUT_FILE_FAILED_TO_OPEN);
    }

	struct graph grph;
    struct graph grph2;

	point* rowcolInfo = malloc(sizeof(point));

	//read input file maze into a double array 
	char** maze = parseInput(fp, rowcolInfo);

	//convert input matrix (double array) into a graph
    matrixToGraph(&grph, maze, rowcolInfo,1);
    matrixToGraph(&grph2, maze, rowcolInfo,2);
	
	//check for correct file closure	  
	file_check = fclose(fp);

	if(file_check!=0)
    {
       // free_graph(&grph);
        printf("\nInput file failed to close\n");
        exit(INPUT_FILE_FAILED_TO_CLOSE);
    }

    int x = 0;

    //holds the solution maze for robot 1
    char **sol = malloc(rowcolInfo->row * sizeof(char *));
    for (x=0; x<rowcolInfo->row; x++)
    {
         sol[x] = malloc(rowcolInfo->col * sizeof(char));
    }

    //holds the solution maze for robot 2
    char **sol2 = malloc(rowcolInfo->row * sizeof(char *));
    for (x=0; x<rowcolInfo->row; x++)
    {
         sol2[x] = malloc(rowcolInfo->col * sizeof(char));
    }

    int i, j;

    //init solution mazes
    for(j=0; j<rowcolInfo->row; j++)
    {
        for(i=0; i<rowcolInfo->col; i++)
        {
            sol[j][i] = maze[j][i];
            sol2[j][i] = maze[j][i];
        }
    }


    coord* coord = malloc(sizeof(coord));
    coord = findCoordS(coord, maze, rowcolInfo);

    printf("\nRobot 1\n");

    //run bfs with coord S as the starting point
	//bfs(&grph, maze, rowcolInfo, sol, coord);

	dfs(&grph, maze, rowcolInfo, sol, coord,1);

	printSolution(sol, rowcolInfo);

	//this tells us if E is reachable from S
	bool flag = 0;

    flag = isReachableE(&grph, rowcolInfo, maze);

    int distance = -1;

    if(flag==true)
    {
        distance = getDistances(rowcolInfo, sol);
        printf("\nExit found!\n");
    }
    else
    {
        printf("\nExit not found!\n");
    }


    coord = findCoordE(coord, maze, rowcolInfo);

    int xCo = coord->x;
    int yCo = coord->y;

    

    //converting the coordinates of E in the matrix to the index of E in the graph
    int indexCo = ((rowcolInfo->col*yCo) + xCo);

    

    printf("\nRobot 1 took %d moves from S to E\n", distance);




	coord = findCoordF(coord, maze, rowcolInfo);

	printf("\n\nRobot 2\n");

	//run bfs on robot 2 with F as the starting point
	//bfs(&grph, maze, rowcolInfo, sol2, coord);

	dfs(&grph2, maze, rowcolInfo, sol2, coord,2);

	printSolution(sol2, rowcolInfo);


	flag = false;

	flag = isReachableL(&grph2, rowcolInfo, maze);

    distance = -1;

    if(flag==true)
    {
        distance = getDistances(rowcolInfo, sol2);
        printf("\nExit found!\n");
    }
    else
    {
        printf("\nExit not found!\n");
    }

    coord = findCoordL(coord, maze, rowcolInfo);

    xCo = coord->x;
    yCo = coord->y;

    indexCo = ((rowcolInfo->col*yCo) + xCo);

    printf("\nRobot 2 took %d moves from F to L\n", distance);

	//free graph and vectors inside
    free(coord);
    freeArray(sol, rowcolInfo);
    freeArray(sol2, rowcolInfo);
    freeArray(maze, rowcolInfo);
    free(rowcolInfo);
	free_graph(&grph);
	free_graph(&grph2);

	return 0;
}

char** parseInput(FILE* fp, point* rowcolInfo)
{
    char c;
    int size = 1;

    //check for empty input file

    if (fp!=NULL) 
    {
        fseek(fp, 0, SEEK_END);
        size = ftell(fp);
        if (0 == size) 
        {
            printf("\nparsing empty input file\n");
            fclose(fp);
            exit(3);
        }
        else
        {
        //if not empty put fp back 
        rewind(fp);
        }
    }

    int counter = 0;
    int row = 1;
    int col = 1;


    //this loop tells us how many rows and columns are in the input file (MAX)
    while ((c = getc(fp)) != EOF) 
    {
        if (c!='\n') 
        {
            counter++;
        }
        else
        {
        	if(counter>col)
        	{
        		col = counter;
        	}
        	counter = 0;
        	row++;
        } 
    }

    //using this we can create the dimensions of our double array matrix
    rowcolInfo->row = row;
    rowcolInfo->col = col;

    rewind(fp);

    int x = 0;
 	char **maze = malloc((row) * sizeof(char *));
    for (x=0; x<row; x++)
    {
         maze[x] = malloc((col) * sizeof(char));
    }

    x = 0;
    int y = 0;

    //init matrix with defualt space values
    for(y=0; y<rowcolInfo->row; y++)
    {
        for(x=0; x<rowcolInfo->col; x++)
        {
            maze[y][x] = ' ';
        }
    }

    x=0;
    y=0;

    int check1 = 0;
    int check2 = 0;
    int check3 = 0;
    int check4 = 0;
    //actual reading in of the input maze
    while ((c = getc(fp)) != EOF) 
    {     
    	//valid characters   
    	if (c==' ' || c=='S' || c=='E'|| c=='L'|| c=='F' || c=='#') 
        {
        	if(c=='S')
        	{
        		check1++;
        	}
        	if(c=='E')
        	{
        		check2++;
        	}
        	if(c=='L')
        	{
        		check3++;
        	}
        	if(c=='F')
        	{
        		check4++;
        	}
            maze[y][x] = c;
            x++;
        }
        //we know we need to move onto the next row in our matrix if a newline is hit
        else if(c=='\n')
        {
            x=0;
        	y++;
        }
        else
        {
            freeArray(maze,rowcolInfo);
        	printf("\nInvalid char encountered in the input file\n");
            exit(100);
        }

    }

    if(check1!=1||check2!=1||check3!=1||check4!=1)
    {
    	printf("\nNeed one and only one of each start/exit in input file\n");
    	exit(2);
    }

return maze;
}


void dfs(struct graph* grph, char** maze, point* rowcolInfo, char** sol, coord* coord, int robot)
{
	int i = 0;
	int j =0;

	struct vertex* current;

	unsigned int vectorSize;

    int xCo = coord->x;
    int yCo = coord->y;

    int indexCo = ((rowcolInfo->col*yCo) + xCo);

	//init all vert except 1 to -1 (infinity)

	for(i=0; i<grph->num_vertices; i++)
	{
		grph->vertices[i].color = grey;
	}

	//queue to hold vertices looking at

	node* stack = NULL;
	stack = add_to_front(stack, &grph->vertices[indexCo]);
	current = remove_from_front(&stack);

	int vertexNum = 0;
	
	int d = 0;
	int e = 0;

	while(current!=NULL)
	{

				if(current->color == grey)
				{
					//give a valid distance
					current->color = black;
                    
                    vectorSize = size_vector(&current->adj);
                    j=0;

                    while(j<vectorSize)
                    {
                        if(grph->vertices[current->adj.data[j]].color == grey)
                        {
                            stack = add_to_front(stack, &grph->vertices[current->adj.data[j]]);
                        }
                        j++;
                    }

					vertexNum = current->number;
					
					d = vertexNum/(rowcolInfo->col);
					vertexNum -= (d* rowcolInfo->col);
					e =  vertexNum;

					if(sol[d][e]!= 'S' && sol[d][e]!='E'&& sol[d][e]!='F'&& sol[d][e]!='L')
					{
                            sol[d][e] = '.';	
					}

                    if(isReachableE(grph,rowcolInfo,maze) && robot==1)
                    {
                    	freeStack(stack);
                        return;
                    }

                    if(isReachableL(grph,rowcolInfo,maze) && robot==2)
                    {
                    	freeStack(stack);
                        return;
                    }


                }

                else
                {
                    vertexNum = current->number;
                    
                    d = vertexNum/(rowcolInfo->col);
                    vertexNum -= (d* rowcolInfo->col);
                    e =  vertexNum;

                    if(sol[d][e]!= 'S' && sol[d][e]!='E'&& sol[d][e]!='F'&& sol[d][e]!='L')
                    {
                            sol[d][e] = ' ';    
                    }

                }

		current = remove_from_front(&stack);
	}

	freeStack(stack);
	return;

}

//queue operations

struct vertex* remove_from_front(node** head)
{
    node* current = *head;
    struct vertex* temp = NULL;

    if(*head==NULL || head==NULL)
    {
        return NULL;
    }
    else
    {
        *head = current->next;
        temp = current->vertex;
        free(current);
        return temp;
    }
}

node* add_to_front(node* head, struct vertex* vertex)
{

    node* current;

    if(head==NULL)
    {
        head=malloc(sizeof(node));
        head->vertex=vertex;
        head->next =NULL;
    }
    else
    {
    	current = malloc(sizeof(node));
    	current->vertex=vertex;
    	current->next = head;
    	head = current;
    }

return head;

}

node* add_to_back(node* head, struct vertex* vertex)
{

    node* current = head;

    if(head==NULL)
    {
        head=malloc(sizeof(node));
        head->vertex=vertex;
        head->next =NULL;
    }
    else
    {
        while(current->next!=NULL)
        {
            current = current->next;
        }
            current->next = malloc(sizeof(node));
            current=current->next;
            current->vertex = vertex;
            current->next=NULL;
    }

return head;

}

void printDistances(struct graph* grph, FILE* fp)
{
	int i = 0;

	for(i=0;i<grph->num_vertices; i++)
	{
		fprintf(fp,"vertex %d: distance: %d\n",i, grph->vertices[i].d);
	}

	return;
}

void printSolution(char** sol, point* rowcolInfo)
{
	printf("\n\n");

    int i = 0;
    int j = 0;

    for (i = 0; i < rowcolInfo->row; i++)
    {
        for (j = 0; j < rowcolInfo->col; j++)
        {
            printf("%c", sol[i][j]);
        }
            printf("\n");
    }
}

void matrixToGraph(struct graph* grph, char** maze, point* rowcolInfo, int robot)
{
    //this function converts the input matrix into a graph, with edges ONLY between spaces or robot start/exits

    int num_vertices = rowcolInfo->row * rowcolInfo -> col;

    init_graph(grph, num_vertices);

    int i, j, src, dest;

    src = -1;

    coord* coordinates = malloc(sizeof(coord));

    //this loop check all adjacent spaces to the character we are looking at in the maze matrix and sees if they are valid to 
    //insert an edge to

    //we have to check all directions (N, S, E, W, NW, NE, SE, SW), this is done by manipulating array coordinates

    for(i=0; i<rowcolInfo->row; i++)
    {
        for(j=0; j<rowcolInfo->col; j++)
        {
            if(maze[i][j]==' ' || maze[i][j]=='S' || maze[i][j]=='E'|| maze[i][j]=='L'|| maze[i][j]=='F')
            {

            	coordinates->x = j;
            	coordinates->y = i;

            	if(robot==1 && isAdj('F', maze, rowcolInfo, coordinates)==true)	
            	{
            		src=-1;
            	}
            	else if(robot==2 && isAdj('E', maze, rowcolInfo, coordinates)==true)
            	{
            		src = -1;
            	}
            	else
            	{

                	src = ((rowcolInfo->col)*i)+j;
                }
            }

            if(i+1 < rowcolInfo->row)
            {    

                if(maze[i+1][j]==' '|| maze[i+1][j]=='S' || maze[i+1][j]=='E'|| maze[i+1][j]=='L'|| maze[i+1][j]=='F')
                {
                    coordinates->x = j;
	            	coordinates->y = i+1;

	            	if(robot==1 && isAdj('F', maze, rowcolInfo, coordinates)==true)	
	            	{
	            		src=-1;
	            	}
	            	else if(robot==2 && isAdj('E', maze, rowcolInfo, coordinates)==true)
	            	{
	            		src = -1;
	            	}
	            	else
	            	{

	                	dest = ((rowcolInfo->col*(i+1)) + j);
	                } 
                    
                    if(src != -1)
                    {
                        insert_edge (grph, src, dest);
                    }
                }
            }

            if(i-1 >=0)
            {
                if(maze[i-1][j]==' '|| maze[i-1][j]=='S' || maze[i-1][j]=='E'|| maze[i-1][j]=='L'|| maze[i-1][j]=='F')
                {
                	coordinates->x = j;
	            	coordinates->y = i-1;

	            	if(robot==1 && isAdj('F', maze, rowcolInfo, coordinates)==true)	
	            	{
	            		src=-1;
	            	}
	            	else if(robot==2 && isAdj('E', maze, rowcolInfo, coordinates)==true)
	            	{
	            		src = -1;
	            	}
	            	else
	            	{

	                	dest = (rowcolInfo->col*(i-1) + j);
	                } 

                    if(src != -1)
                    {
                        insert_edge (grph, src, dest);
                    }
                }
            }

            if(j+1 <rowcolInfo->col)
            {
                if(maze[i][j+1]==' '|| maze[i][j+1]=='S' || maze[i][j+1]=='E'|| maze[i][j+1]=='L'|| maze[i][j+1]=='F')
                {
                	coordinates->x = j+1;
	            	coordinates->y = i;

	            	if(robot==1 && isAdj('F', maze, rowcolInfo, coordinates)==true)	
	            	{
	            		src=-1;
	            	}
	            	else if(robot==2 && isAdj('E', maze, rowcolInfo, coordinates)==true)
	            	{
	            		src = -1;
	            	}
	            	else
	            	{

	                	dest = ((j+1) + (rowcolInfo->col*i));
	                } 

                    
                     if(src != -1)
                    {
                        insert_edge (grph, src, dest);
                    }
                }
            }

            if(j-1 >=0)
            {
                if(maze[i][j-1]==' '|| maze[i][j-1]=='S' || maze[i][j-1]=='E'|| maze[i][j-1]=='L'|| maze[i][j-1]=='F')
                {

                	coordinates->x = j-1;
	            	coordinates->y = i;

	            	if(robot==1 && isAdj('F', maze, rowcolInfo, coordinates)==true)	
	            	{
	            		src=-1;
	            	}
	            	else if(robot==2 && isAdj('E', maze, rowcolInfo, coordinates)==true)
	            	{
	            		src = -1;
	            	}
	            	else
	            	{

	                	dest = ((j-1) + (rowcolInfo->col*i));
	                } 

                    
                     if(src != -1)
                    {
                        insert_edge (grph, src, dest);
                    }
                }
            }

            if(i+1 < rowcolInfo->row && j+1 <rowcolInfo->col)
            {

                if(maze[i+1][j+1]==' '|| maze[i+1][j+1]=='S' || maze[i+1][j+1]=='E'|| maze[i+1][j+1]=='L'|| maze[i+1][j+1]=='F')
                {

                	coordinates->x = j+1;
	            	coordinates->y = i+1;

	            	if(robot==1 && isAdj('F', maze, rowcolInfo, coordinates)==true)	
	            	{
	            		src=-1;
	            	}
	            	else if(robot==2 && isAdj('E', maze, rowcolInfo, coordinates)==true)
	            	{
	            		src = -1;
	            	}
	            	else
	            	{

	                	dest = (rowcolInfo->col*(i+1) + (j+1));
	                } 

                    
                     if(src != -1)
                    {
                        insert_edge (grph, src, dest);
                    }
                }
            }

            if(i-1 >=0 && j-1 >=0)
            {

                if(maze[i-1][j-1]==' '|| maze[i-1][j-1]=='S' || maze[i-1][j-1]=='E'|| maze[i-1][j-1]=='L' || maze[i-1][j-1]=='F')
                {

                	coordinates->x = j-1;
	            	coordinates->y = i-1;

	            	if(robot==1 && isAdj('F', maze, rowcolInfo, coordinates)==true)	
	            	{
	            		src=-1;
	            	}
	            	else if(robot==2 && isAdj('E', maze, rowcolInfo, coordinates)==true)
	            	{
	            		src = -1;
	            	}
	            	else
	            	{

	                	dest = (rowcolInfo->col*(i-1) + (j-1));
	                } 
                    
                     if(src != -1)
                    {
                        insert_edge (grph, src, dest);
                    }
                }
            }

            if(i+1<rowcolInfo->row && j-1 >=0)
            {
                if(maze[i+1][j-1]==' '|| maze[i+1][j-1]=='S' || maze[i+1][j-1]=='E'|| maze[i+1][j-1]=='L'|| maze[i+1][j-1]=='F')
                {

                	coordinates->x = j-1;
	            	coordinates->y = i+1;

	            	if(robot==1 && isAdj('F', maze, rowcolInfo, coordinates)==true)	
	            	{
	            		src=-1;
	            	}
	            	else if(robot==2 && isAdj('E', maze, rowcolInfo, coordinates)==true)
	            	{
	            		src = -1;
	            	}
	            	else
	            	{

	                	dest = (rowcolInfo->col*(i+1) + (j-1));
	                }
                    
                     if(src != -1)
                    {
                        insert_edge (grph, src, dest);
                    }
                }
            }

            if(i-1>=0 && j+1 <rowcolInfo->col)
            {

                if(maze[i-1][j+1]==' '|| maze[i-1][j+1]=='S' || maze[i-1][j+1]=='E'|| maze[i-1][j+1]=='L'|| maze[i-1][j+1]=='F')
                {

                	coordinates->x = j+1;
	            	coordinates->y = i-1;

	            	if(robot==1 && isAdj('F', maze, rowcolInfo, coordinates)==true)	
	            	{
	            		src=-1;
	            	}
	            	else if(robot==2 && isAdj('E', maze, rowcolInfo, coordinates)==true)
	            	{
	            		src = -1;
	            	}
	            	else
	            	{

	                	dest = (rowcolInfo->col*(i-1) + (j+1));
	                }

                    
                     if(src != -1)
                    {
                        insert_edge (grph, src, dest);
                    }
                }
            }

            src = -1;

        }
    }
    
    free(coordinates);
}

coord* findCoordS(coord* coordinates, char** maze, point* rowcolInfo)
{
	//simply returns the matrix coordinates of S in the matrix array

    int i;
    int j;

    for(i=0; i<rowcolInfo->row; i++)
    {
        for(j=0; j<rowcolInfo->col; j++)
        {
            if(maze[i][j] == 'S')
            {
                coordinates->x = j;
                coordinates->y = i;
                return coordinates;
            }
        }
    }

    return NULL;

}

coord* findCoordE(coord* coordinates, char** maze, point* rowcolInfo)
{
	//simply returns the matrix coordinates of E in the matrix array

    int i;
    int j;

    for(i=0; i<rowcolInfo->row; i++)
    {
        for(j=0; j<rowcolInfo->col; j++)
        {
            if(maze[i][j] == 'E')
            {
                coordinates->x = j;
                coordinates->y = i;
                return coordinates;
            }
        }
    }

    return NULL;

}

coord* findCoordL(coord* coordinates, char** maze, point* rowcolInfo)
{
	//simply returns the matrix coordinates of L in the matrix array

    int i;
    int j;

    for(i=0; i<rowcolInfo->row; i++)
    {
        for(j=0; j<rowcolInfo->col; j++)
        {
            if(maze[i][j] == 'L')
            {
                coordinates->x = j;
                coordinates->y = i;
                return coordinates;
            }
        }
    }

    return NULL;

}

coord* findCoordF(coord* coordinates, char** maze, point* rowcolInfo)
{
	//simply returns the matrix coordinates of F in the matrix array

    int i;
    int j;

    for(i=0; i<rowcolInfo->row; i++)
    {
        for(j=0; j<rowcolInfo->col; j++)
        {
            if(maze[i][j] == 'F')
            {
                coordinates->x = j;
                coordinates->y = i;
                return coordinates;
            }
        }
    }

    return NULL;

}



bool isReachableE(struct graph* grph, point* rowcolInfo, char** maze)
{
	//This function returns true of false if E is reachable after bfs is run 

    coord* E = malloc(sizeof(coord));

    E = findCoordE(E,maze,rowcolInfo);

    int point = E->x + (rowcolInfo->col * E->y);

    if(grph->vertices[point].color == black)
    {
        free(E);
        return true;
    }
    else
    {
        free(E);
        return false;
    }
    
}

bool isReachableL(struct graph* grph, point* rowcolInfo, char** maze)
{
	//This function returns true of false if L is reachable after bfs is run 

    coord* L = malloc(sizeof(coord));

    L = findCoordL(L,maze,rowcolInfo);

    int point = L->x + (rowcolInfo->col * L->y);

    if(grph->vertices[point].color == black)
    {
        free(L);
        return true;
    }
    else
    {
        free(L);
        return false;
    }
}

bool isAdj(char letter, char** maze, point* rowcolInfo, coord* coordinates)
{
    int x = coordinates->x;
    int y = coordinates->y;

    if(y+1 < rowcolInfo->row)
    {    
        if(maze[y+1][x]==letter)
        {
            return true;
        }

    }

    if(y-1 >=0)
    {
        if(maze[y-1][x]==letter)
        {
            return true;
        }
    }

    if(x+1 <rowcolInfo->col)
    {
        if(maze[y][x+1]==letter)
        {
            return true;
        }
    }

    if(x-1 >=0)
    {
        if(maze[y][x-1]==letter)
        {
            return true;
        }     

    }

    if(y+1 < rowcolInfo->row && x+1 <rowcolInfo->col)
    {
        if(maze[y+1][x+1]==letter)
        {
            return true;
        }    
    }

    if(y-1 >=0 && x-1 >=0)
    {
        if(maze[y-1][x-1]==letter)
        {
            return true;
        }
    }

    if(y+1<rowcolInfo->row && x-1 >=0)
    {
        if(maze[y+1][x-1]==letter)
        {
            return true;
        }        
    }

    if(y-1>=0 && x+1 <rowcolInfo->col)
    {

        if(maze[y-1][x+1]==letter)
        {
            return true;
        }         
    }

    return false;

}

int getDistances(point* rowcolInfo, char** maze)
{
	int counter = 0;
	int i,j;
	for(i=0; i<rowcolInfo->row;i++)
	{
		for(j=0; j<rowcolInfo->col; j++)
		{
			if(maze[i][j]=='.')
			{
				counter++;
			}
		}
	}
	return counter;
}

void freeArray(char** maze, point* rowcolInfo)
{
    int i;

    for(i=0; i<rowcolInfo->row; i++)
    {
        free(maze[i]);
    }

    free(maze);

}
void freeStack(node* stack)
{
    node* current = stack;
    node* temp;

    if(stack==NULL)
    {
        return;
    }

    while(current->next!=NULL)
    {
        temp=current->next;
        free(current);
        current=temp;
    }

    if(current->next==NULL)
    {
        free(current);
    }

    return;

}