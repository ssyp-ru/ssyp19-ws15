#define AnimalsNumber 7
#define CharNumber 7 
#define InheritProb ((rand()%500)/500.0-0.5)
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

typedef struct list {
	char name[64];
	struct list * next;
} list;

typedef struct property {
	char name[64];
	struct property * next;
	//struct property * head;
} property;

typedef struct animal {
	double charact[7];
	property * head;
} animal;

property * add_new(property * head, char* data)
{	
	property * mutagens = (property*) malloc(sizeof(property));
	int i = 0;
	do
	{
		mutagens->name[i] = data[i];
		i++;
	} while(data[i]);
	mutagens->name[i] = 0;
	mutagens->next = head;
	head = mutagens;
	return head;
}

void delete_list(property * head)
{
	if (!head)
		return;
	property * amegaLul;
	while(head)
	{
		if (head->next)
		{
			amegaLul = head->next;
			free(head);
			head = amegaLul;
		}
		else
		{
			free(head);
			head = NULL;	// HERE!!! How did you want to get !head without targeting pointer to NULL??
		}
	}
}

// DELETE SAME POINTER
property * delete_node(property * head, property * del)
{
	if (!head)
		return NULL;
	if(head == del)
	{
		head = head->next;
		free(del);
		return head;
	}
	property * prim = head;
	while(head->next != del)
	{
		head = head->next;
		if(!head->next)
			return prim;
	}
	head->next = del->next;
	free(del);
	return prim;
}

property * ListCpy(property * Source)
{
	if(!Source)
		return NULL;
	property * ret = (property *) malloc(sizeof(property));
	property * tmp = ret;
	strcpy(tmp->name, Source->name);
	Source = Source->next;
	while(Source)
	{
		tmp->next = (property *) malloc(sizeof(property));
		tmp = tmp->next;
		strcpy(tmp->name, Source->name);
		Source = Source->next;
	}
	tmp->next = NULL;
	return ret;
}

void AnimalKill(animal * MyAnimals, int kill, int Ans1, int Ans2)
{
	const int range = 2001;
	const double min = -1000.0;
	double rand_all;
	double rand_add;
	int fifty_fifty;
	

	for (int i = 0; i < CharNumber; i++){
		rand_all = (double) (rand() % range + min) / 20000.0;
		MyAnimals[kill].charact[i] = (MyAnimals[Ans1].charact[i] + MyAnimals[Ans2].charact[i]) / 2.0;
		MyAnimals[kill].charact[i] *= (1.0 + rand_all);
	}
	delete_list(MyAnimals[kill].head);
	MyAnimals[kill].head = NULL;

	property * Ans1prop = MyAnimals[Ans1].head;
	property * Ans2prop = MyAnimals[Ans2].head;
	property * TmpList1 = ListCpy(Ans1prop);
	property * TmpList2 = ListCpy(Ans2prop);
	property * runner1 = TmpList1;
	property * runner2 = TmpList2;
	while (runner1)
	{
		while(runner2)
		{
			if(strcmp(runner1->name, runner2->name) != 0)
				runner2 = runner2->next;
			else
				break;
		}
		if(runner2)
		{
			MyAnimals[kill].head = add_new(MyAnimals[kill].head, runner1->name);
			printf("Added 100%%: %s\n", MyAnimals[kill].head->name);
			// FIXME : absolutely not optimized
			TmpList1 = delete_node(TmpList1, runner1);
			runner1 = TmpList1;
			TmpList2 = delete_node(TmpList2, runner2);
			runner2 = TmpList2;
		}
		else
		{
			runner1 = runner1->next;
			runner2 = TmpList2;
		}
	}
	while(TmpList1)
	{
		runner1 = TmpList1;
		TmpList1 = TmpList1->next;
		if(InheritProb > 0)
		{
			runner1->next = MyAnimals[kill].head;
			MyAnimals[kill].head = runner1;
printf("I) Added 50%%: %s\n", MyAnimals[kill].head->name);
		}
		else
		{
			free(runner1);
		}
	}
	while(TmpList2)
	{
		runner2 = TmpList2;
		TmpList2 = TmpList2->next;
		if(InheritProb > 0)
		{
			runner2->next = MyAnimals[kill].head;
			MyAnimals[kill].head = runner2;
printf("II) Added 50%%: %s\n", MyAnimals[kill].head->name);
		}
		else
		{
			free(runner2);
		}
	}
}

void AnimalsPrint(animal * MyAnimals, int * random)
{
	int rand = 0;
	char character[7][32] = {"Speed km/h", "IQ", "Length of the hat cm", "Mass kg", "Quality of fat kg", "Tail cm", "Foot size cm"};
	for(int animal = 0; animal < 3; animal++)
	{
		printf("%d) Animal %d: ", animal + 1, animal + 1);
		for(int i = 0; i < CharNumber; i++)
		{
			printf("%s = %10.1lf\t", character[i], MyAnimals[random[animal]].charact[i]);
		}
		property * tmp = MyAnimals[random[animal]].head;
		printf("\nProperties: ");
		while(tmp != NULL)
		{
			printf(" %s", tmp->name);
			tmp=tmp->next;
		}
		printf("\n\n");
	}
}

void ListOfAllAnimals(animal * MyAnimals)
{
	char character[7][32] = {"Speed km/h", "IQ", "Length of the hat cm", "Mass kg", "Quality of fat kg", "Tail cm", "Foot size cm"};
	for(int animal = 0; animal < AnimalsNumber; animal++)
	{
		printf("\nProperties: ");
		printf("%d) Animal %d: ", animal + 1, animal + 1);
		for(int i = 0; i < CharNumber; i++)
			printf("%s = %10.1lf\n", character[i], MyAnimals[animal].charact[i]);
		property * tmp = MyAnimals[animal].head;
		while(tmp != NULL)
		{
			printf(" %s", tmp->name);
			tmp=tmp->next;
		}
		printf("\n");
	}
	return;
}

int main()
{
	srand(time(NULL));
	animal MyAnimals[AnimalsNumber];
	for (int k = 0; k < 7/*AnimalsNumber*/; k++)
		MyAnimals[k].head = NULL;
	FILE * input;
	char readNull;
	// TODO FILE * characters;
	// TODO FILE * defaultIput;
	if(!(input = fopen("input.txt", "r")))
	{
		printf("Invalid input file!\n");
		return 1;
	}
	char * duc;
	duc = (char *) malloc(sizeof(char) * 64);
	for(int p = 0; p < AnimalsNumber; p++)
	{
		for(int t = 0; t < CharNumber; t++) 
		{
			fscanf(input, "%lf", &MyAnimals[p].charact[t]);
		}
//printf("Reading properties...\n");
		fscanf(input, "%c", &readNull);
		while(1)
		{
			fscanf(input, "%[^\n]s", duc);
			fscanf(input, "%c", &readNull);
			if(duc[0] == 0 || duc[0] == '\n')
			{
				break;
			}
//printf("Read value: %s\n", duc);
			MyAnimals[p].head = add_new(MyAnimals[p].head, duc);
			duc[0] = 0;
		}
	}
	for(;;)
	{	
		int random[3];
		random[0] = rand() % AnimalsNumber;
		do
		{
			random[1] = rand() % AnimalsNumber;
		} while(random[1] == random[0]);
		do
		{
			random[2] = rand() % AnimalsNumber;
		} while(random[2] == random[1] || random[2] == random[0]);
		//random 3 animals
		//1
//random[0] = 1;
//random[1] = 5;
//random[2] = 6;
		AnimalsPrint(MyAnimals, random);

		//2

		printf ("Which animal do you want to kill?\n(1-3 -- animal; 9 -- The whole population; 7 -- Default population; 0 -- exit)\n");
		//char read[32];
		int choise;
		scanf("%d", &choise);
		if (choise == 0)
			break;
		else if (choise == 9)
			ListOfAllAnimals(MyAnimals);	// TODO
		else if(choise == 1)
			AnimalKill(MyAnimals, random[0], random[1], random[2]);
		else if(choise == 2)
			AnimalKill(MyAnimals, random[1], random[0], random[2]);
		else if(choise == 3)
			AnimalKill(MyAnimals, random[2], random[1], random[0]);
		else if (choise == 7)
		{
			// TODO: Read animals from defaultInput
		}
		else
		{
			printf("Undefined command!\n");
			fclose(input);
			return -1;
		}
		// TODO : call for function
	}
	fclose(input);
	// TODO: SaveAnimals();
	return 0;
}

