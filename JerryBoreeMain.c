///
// Created by tamar on 21/12/2024.
//
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Defs.h"
#include "Jerry.h"
#include "HashTable.h"
#include "LinkedList.h"
#include "MultiValueHashTable.h"
#include <math.h>
#include "KeyValuePair.h"

/**
 * @struct PlanetList
 * Represents a list of planets, allowing dynamic storage and management of multiple planets.
 */

typedef struct {
    Planet **planets; ///< Array of pointers to Planet structs
    int size; ///< Number of planets in the list
} PlanetList;

// Create a list of planets
PlanetList *create_planet_list() {
    PlanetList *list = malloc(sizeof(PlanetList));
    if (!list) {
        list = NULL; // Explicitly set to NULL
        return list;
    }
    list->planets = NULL;
    list->size = 0;
    return list;
}

// Check if a planet already exists in the list
bool cheak_planet(PlanetList *list, Planet *planet) {
    if (!list || !planet) { // Check if inputs are NULL
        return false;
    }

    if (list->size == 0) {
        return false;
    } else {
        for (int i = 0; i < list->size; i++) {
            if (strcmp(list->planets[i]->name, planet->name) == 0) {
                return true; // Return true if the planet exists
            }
        }
    }
    return false;
}

// Add a planet to the planet list
status add_to_planet_list(PlanetList *pl, Planet *planet) {
    if (!pl || !planet) { // Check for NULL inputs
        return failure;
    }
    Planet **temp = realloc(pl->planets, (pl->size + 1) * sizeof(Planet *));
    if (!temp) {
        return failure; // Return failure if realloc fails
    }
    pl->planets = temp;
    pl->planets[pl->size] = planet;
    pl->size++;
    return success;
}

// Free memory allocated for the planet list
void free_planet_list(PlanetList *pl) {
    if (!pl) return; // If the PlanetList itself is NULL, nothing to free

    if (pl->planets) { // Check if the array of pointers is allocated
        for (int i = 0; i < pl->size; i++) {
            if (pl->planets[i]) { // Check if each planet is allocated
                free_planet(pl->planets[i]); // Free each planet using the provided function
                pl->planets[i] = NULL; // Nullify the pointer to avoid dangling pointers
            }
        }
        free(pl->planets); // Free the array of pointers
        pl->planets = NULL; // Nullify the pointer to the array
    }
    free(pl); // Free the structure itself
}

// Check if a number is prime
bool is_prime(int n) {
    if (n == 2) {return true;}
    if (n % 2 == 0) {return false;}
    if (n == 3){return true;}
    if (n % 3 == 0) {return false;}

    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return false;
        }
    }
    return true;
}

// Find the nearest prime number greater than or equal to n
int find_close_prime(int n) {
    if (n <= 2){return 2;}
    while (is_prime(n) == false) {
        n++;
    }
    return n;
}

// Check if an input string is a valid menu option (1-9)
static bool is_valid(char *input) {
    if (strlen(input) == 0) return false;
    char *valid[9] = {"1","2","3","4","5","6","7", "8", "9"};
    for (int i = 0; i < 9; i++) {
        if (strcmp(input, valid[i])==0) {
            return true;
        }
    }
    return false;
}

// Check if an input string is a valid secondary menu option (1-3)
static bool is_valid2(char *input) {
    if (strlen(input) == 0) return false;
    char *valid[3] = {"1","2","3"};
    for (int i = 0; i < 3; i++) {
        if (strcmp(input, valid[i])==0) {
            return true;
        }
    }
    return false;
}

// Process and add a planet from an input line
static status process_planet(PlanetList *planet_list, char *input_line) {
    if (!planet_list || !input_line){return failure;}
    char *name = strtok(input_line, ","); // Extract the name
    float x = atof(strtok(NULL, ",")); // Extract x-coordinate
    float y = atof(strtok(NULL, ",")); // Extract y-coordinate
    float z = atof(strtok(NULL, ",")); // Extract z-coordinate
    Planet *planet = create_planet(name, x, y, z); // Create the planet
    status func = add_to_planet_list(planet_list, planet); // Add the planet to the list
    if (func == success && planet) {
        return success;
    }
    return failure;
}

// Process and create a Jerry from an input line
Jerry *process_jerry(hashTable jerrytable, PlanetList *planet_list, char *input_line, linkedlist alljerries) {
    if (!planet_list || !input_line||!alljerries||!jerrytable) {return NULL;};
    char *id = strtok(input_line, ","); // Extract Jerry ID
    char *reality = strtok(NULL, ","); // Extract reality
    char *planet_name = strtok(NULL, ","); // Extract planet name
    int happiness = atoi(strtok(NULL, ",")); // Extract happiness level
    if (!id || !reality || !planet_name) {return NULL;}
    for (int i = 0; i < planet_list->size; i++) {
        if (strcmp(planet_list->planets[i]->name, planet_name) == 0) {
        Jerry *new_jerry = create_jerry(id, reality, planet_list->planets[i], happiness);
        if (!new_jerry) {
        return NULL;
        }
        status addjerry = addToHashTable(jerrytable , id , new_jerry);
        if (addjerry == failure) {
         free_jerry(new_jerry);
         return NULL;}

        addjerry = appendNode(alljerries, new_jerry);
            if (addjerry == failure) {
              removeFromHashTable(jerrytable,id);
              return NULL;
            }
         return new_jerry;}}
 return NULL;
}

// Process and add a physical characteristic to a Jerry
static status process_pc(multiValueHashTable multihashpc, Jerry *jerry, char *input_line) {
  if (!jerry || !input_line||!multihashpc) {
   return failure;
  }
  if (input_line[0] == '\t') { // Skip tab character if present
   input_line++;
  }

  char *pc_name = strtok(input_line, ":"); // Extract characteristic name
  char *pc_value_str = strtok(NULL, ":"); // Extract characteristic value as string
  float pc_val = atof(pc_value_str); // Convert value to float
  PhysicalCharacteristics *new_pc = create_physical_characteristics(pc_name, pc_val); // Create new characteristic
  if (!new_pc) {
   return failure;
  }
  status func = add_pc_to_jerry(jerry,new_pc); // Add the characteristic to Jerry
  if (func == failure) { // Free memory if addition fails
   free_physical_characteristics(new_pc);
   return failure;
  }
  status pc = addToMultiValueHashTable(multihashpc, pc_name, jerry);
  if (pc == failure) {
    free_physical_characteristics(new_pc);
    return failure;
 }
 return success;
}
// Copy a Jerry element without creating a deep copy
Element copyJerryVal(Element jerry) {
  if (!jerry) {
    return NULL;
  }
  Jerry *copyjerry = (Jerry *)(jerry);
  return (Element)copyjerry;
}

// Copy a string key
Element copyKey(Element str) {
  if (!str){
    return NULL;
  }
  char *original = (char *)str;
  char *copy = strdup(original);
  if (!copy) {
    return NULL;
  }
  return (Element)copy;
}

// Free a string key
status free_str_Key(Element str) {
  if (!str){
    return failure;
  }
  free((char *)str);
  return success;
}

// Print a string key (currently commented out)
status print_str_key(Element str) {
  if (!str){
    return failure;
  }
  printf("%s : \n", (char *)str);
  return success;
}

// Print a Jerry object
status print_jerry_val(Element jerry) {
  if (!jerry){
    return failure;
  }
  Jerry *printjerry = (Jerry *)(jerry);
  return print_jerry(printjerry);
}

// Free a Jerry object
status free_jerry_val(Element jerry) {
  if (!jerry){
    return failure;
  }
  Jerry *freejerry = (Jerry *)(jerry);
  return free_jerry(freejerry);
}

// Compare two string keys for equality
bool key_cmp(Element str1, Element str2) {
  if (!str1 || !str2){
    return false;
  }
  if (strcmp((char *)str1, (char *)str2) == 0) {
    return true;
  }
  return false;
}

// Convert a Jerry ID to a numeric value
int jerry2num(Element id) {
  if (!id){
    return 0;
  }
  int sum = 0;
  char *copy = (char *)id;
  for (int i = 0; i < strlen(copy); i++) {
    char ch = copy[i];
    if (isalpha(ch)) {
      sum += (int)ch;
    } else if (isdigit(ch)) {
      sum += ch - '0';
    }
  }
  return sum;
}

// Create a hash table for storing Jerries
hashTable createHashJerry(int size){
  hashTable jerrrytable = createHashTable(copyKey, free_str_Key, print_str_key, copyJerryVal, free_jerry_val, print_jerry_val, key_cmp, jerry2num, size);
  if (!jerrrytable) {
    return NULL;
  }
  return jerrrytable;
}

// Compare two Jerry objects for equality based on their IDs
bool equaljerrys(Element jerry1, Element jerry2) {
  if (!jerry1 || !jerry2) {
    return false;
  }
  Jerry *one = (Jerry *)jerry1;
  Jerry *two = (Jerry *)jerry2;
  if (strcmp(getjerryid(one), getjerryid(two)) == 0) {
    return true;
  }
  return false;
}

// A placeholder function that does not free Jerry objects
status NOTfreejerrys(Element jerry) {
  return success;
}

// Create a MultiValueHashTable for storing physical characteristics
multiValueHashTable createMultiValueHashTablePC(int size){
  multiValueHashTable hashPC = createMultiValueHashTable(copyKey, free_str_Key, print_str_key, copyJerryVal, NOTfreejerrys, print_jerry_val, key_cmp, jerry2num, size, equaljerrys);
  if (!hashPC) {
    return NULL;
  }
  return hashPC;
}

// Count the number of Jerries and physical characteristics in a file
status count_elements_infile(const char *filename, int *countjerrys, int *countpc) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    return failure;
  }

  char line[301];
  *countjerrys = 0;
  *countpc = 0;
  bool jerrys = false;

  while (fgets(line, sizeof(line), file)) {
    line[strcspn(line, "\n")] = '\0';
    if (strcmp(line, "Jerries") == 0) {
      jerrys = true;
      continue;
    }
    if (jerrys) {
      if (strchr(line, ':') == NULL) {
        (*countjerrys)++;
      } else {
        (*countpc)++;
      }
    }
  }

  fclose(file);
  return success;
}

// Load data from a file into the data structures
static status load_file(char *filename, PlanetList **planetList, hashTable JerrysHashTable, int num_of_planets, multiValueHashTable PC_MultiHashTable, linkedlist alljerries ) {
  if (!alljerries || !JerrysHashTable || !PC_MultiHashTable) {
    return failure;
  }
  FILE *file = fopen(filename, "r"); // Open file
  if (!file) {
    printf(" A memory problem has been detected in the program");
    return failure; // Return failure if file couldn't be opened
  }

  // Initialize the lists and counters
  *planetList = create_planet_list(); // Initialize planet list
  if (!(*planetList)) {
    return failure;
  }

  char line[301]; // Buffer for reading lines from the file
  status op_status = success; // Variable to track the status of operations

  // Read lines from the file while there are no errors and lines are available
  while (op_status == success && fgets(line, sizeof(line), file)) {
    line[strcspn(line, "\n")] = '\0'; // Remove trailing newline

    if (strcmp(line, "Planets") == 0) { // Process planets
      if (num_of_planets == 0) { // Check if there are any planets to process
        break; // Exit if no planets need to be processed
      }
      for (int i = 0; i < num_of_planets; i++) {
        if (!fgets(line, sizeof(line), file)) { // Read the next line for planet data
          op_status = failure;
          break;
        }
        line[strcspn(line, "\n")] = '\0'; // Remove the trailing newline
        op_status = process_planet(*planetList, line); // Process the planet and add it to the list
        if (op_status == failure) break;
      }
    }
    fgets(line, sizeof(line), file);
    line[strcspn(line, "\n")] = '\0';
    if (strcmp(line, "Jerries") == 0 && op_status == success) {
      Jerry *current_jerry = NULL;
      while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';
        if (strchr(line, ':') != NULL) {
          if (current_jerry) {
            op_status = process_pc(PC_MultiHashTable, current_jerry, line);
            if (op_status == failure) break;
          } else {
            printf(" A memory problem has been detected in the program \n");
            op_status = failure;
            break;
          }
        } else if (strchr(line, ',') != NULL) {
          current_jerry = process_jerry(JerrysHashTable, *planetList, line, alljerries);
          if (!current_jerry) {
            printf(" A memory problem has been detected in the program \n");
            op_status = failure;
            break;
          }
        }
      }
    }
    fclose(file);
    return op_status;
  }
  return op_status;
}

// Find a Jerry by ID in a hash table
Jerry *jerrybyid(hashTable hashjerry, char *key) {
  if (!hashjerry || !key) {
    return NULL;
  }
  return (Jerry *)(lookupInHashTable(hashjerry, key));
}

// Add a physical characteristic to a Jerry and update the MultiValueHashTable
status addpctojerryhash(multiValueHashTable multihashpc, Jerry *jerry, char *key, float pcval) {
    if (!jerry || !key || !multihashpc) {
        return failure;
    }
    PhysicalCharacteristics *pc = create_physical_characteristics(key, pcval);
    if (!pc) {
        return failure;
    }
    status s = addToMultiValueHashTable(multihashpc, key, jerry);
    if (s == failure) {
        free_pc(pc);
        return failure;
    }

    s = add_pc_to_jerry(jerry, pc);
    if (s == failure) {
        free_physical_characteristics(pc);
        removeFromMultiValueHashTable(multihashpc, key, jerry);
        return failure;
    }
    return success;
}

// Remove a physical characteristic from a Jerry and update the MultiValueHashTable
status removepcfromjerry(multiValueHashTable multihashpc, Jerry *jerry, char *key) {
    if (!jerry || !key || !multihashpc) {
        return failure;
    }
    status s = removeFromMultiValueHashTable(multihashpc, key, jerry);
    if (s == failure) {
        return failure;
    }
    s = delete_pc_to_jerry(jerry, key);
    if (s == failure) {
        return failure;
    }
    return success;
}

// Remove a Jerry from all data structures
status removejerry(multiValueHashTable multihashpc, hashTable hashjerry, Jerry *jerry, linkedlist alljerries) {
    if (!jerry || !hashjerry || !multihashpc) {
        return failure;
    }
    for (int i = 0; i < jerry->pc_num; i++) {
        if (jerry->PhysicalCharacteristics[i]) {
            removeFromMultiValueHashTable(multihashpc, jerry->PhysicalCharacteristics[i]->name, jerry);
        }
    }
    deleteNode(alljerries, jerry);
    removeFromHashTable(hashjerry, getjerryid(jerry));
    return success;
}

// Add a new Jerry to the hash table and linked list
Jerry *addjerrytotabele(hashTable jerryhash, char *id , char *reality , int happiness, Planet *planet, linkedlist alljerries) {
    if (!jerryhash || !id || !reality) {
        return NULL;
    }
    Jerry *newjerry = create_jerry(id, reality, planet, happiness);
    print_jerry(newjerry);
    if (!newjerry) {
        return NULL;
    }

    status s = addToHashTable(jerryhash, id, newjerry);
    if (s == failure) {
        free_jerry(newjerry);
        return NULL;
    }
    s = appendNode(alljerries, newjerry);
    if (s == failure) {
        removeFromHashTable(jerryhash, id);
        return NULL;
    }
    return newjerry;
}

// Find the most similar Jerry based on a physical characteristic and its value
Jerry *similarjerry(hashTable jerryhash, multiValueHashTable multihashpc, char *pc, float val) {
    if (!jerryhash || !multihashpc || !pc) {
        return NULL;
    }

    linkedlist all = lookupInMultiValueHashTable(multihashpc, pc);
    if (!all || getLengthList(all) == 0) {
        return NULL;
    }

    Jerry *most_similar = NULL;
    float closest_diff = 999;

    for (int i = 0; i < getLengthList(all); i++) {
        Jerry *current = (Jerry *)getDataByIndex(all, i);
        if (!current) {
            continue;
        }
        for (int j = 0; j < current->pc_num; j++) {
            if (strcmp(current->PhysicalCharacteristics[j]->name, pc) == 0) {
                float diff = fabs(current->PhysicalCharacteristics[j]->val - val);
                if (diff < closest_diff) {
                    closest_diff = diff;
                    most_similar = current;
                }
                break;
            }
        }
    }

    return most_similar;
}

// Find the saddest Jerry in the linked list
Jerry *saddestjerry(linkedlist alljerries) {
    if (!alljerries) {
        return NULL;
    }
    int saddest = 999;
    Jerry *newjerry = NULL;
    for (int i = 0; i < getLengthList(alljerries); i++) {
        Jerry *current = getDataByIndex(alljerries, i);
        int temp = current->happiness;
        if (temp < saddest) {
            saddest = temp;
            newjerry = current;
        }
    }
    return newjerry;
}

// Find a planet by name in the planet list
Planet *checkplanetname(PlanetList *planetlist, char *name) {
    if (!planetlist || !name) {
        return NULL;
    }
    for (int i = 0; i < planetlist->size; i++) {
        if (strcmp(planetlist->planets[i]->name, name) == 0) {
            return planetlist->planets[i];
        }
    }
    return NULL;
}

// Clean up all data structures
status cleanall(linkedlist alljerries, multiValueHashTable multihashpc, hashTable hashjerry, PlanetList *planetList) {
    destroyHashTable(hashjerry);
    destroyMultiValueHashTable(multihashpc);
    free_planet_list(planetList);
    destroyList(alljerries);
    return success;
}

// Ensure the happiness of a Jerry is within valid bounds (0-100)
void valid_happiness(Jerry *jerry) {
    if (!jerry) {
        return;
    }
    if (jerry->happiness < 0) {
        jerry->happiness = 0;
        return;
    }
    if (jerry->happiness > 100) {
        jerry->happiness = 100;
        return;
    }
}

// Update the happiness of all Jerries in the list based on conditions
status update_happiness(linkedlist alljerries, int above, int addabove, int decbelow) {
    if (!alljerries) {
        return failure;
    }
    for (int i = 0; i < getLengthList(alljerries); i++) {
        Jerry *currentJ = (Jerry *)(getDataByIndex(alljerries, i));
        if (currentJ->happiness >= above) {
            currentJ->happiness += addabove;
        } else {
            currentJ->happiness -= decbelow;
        }
        valid_happiness(currentJ);
    }
    return success;
}

void print_menu(){
    printf("Welcome Rick, what are your Jerry's needs today ? \n"
       "1 : Take this Jerry away from me \n"
       "2 : I think I remember something about my Jerry \n"
       "3 : Oh wait. That can't be right \n"
       "4 : I guess I will take back my Jerry now \n"
       "5 : I can't find my Jerry. Just give me a similar one \n"
       "6 : I lost a bet. Give me your saddest Jerry \n"
       "7 : Show me what you got \n"
       "8 : Let the Jerries play \n"
       "9 : I had enough. Close this place \n");
}
// Handle the addition of a new Jerry to the daycare
//PLEASE NOTE - in all cases no need to chaka nulls pointer- it already not null if the menu works.
status case1(hashTable hashjerry, PlanetList *planetList, linkedlist alljerries ) {
    char planet_ID[301] = {0};
    char jerry_dimension[301] = {0};
    char jerry_ID[301] ={0};
    int happiness = 0;

    printf("What is your Jerry's ID ? \n");
    scanf("%s", jerry_ID);
    if (lookupInHashTable(hashjerry, jerry_ID) != NULL) {
        printf("Rick did you forgot ? you already left him here ! \n");
        return success;
    }
    printf("What planet is your Jerry from ? \n");
    scanf("%s", planet_ID);
    Planet *planet = checkplanetname(planetList, planet_ID);
    if (planet == NULL) {
        printf("%s is not a known planet ! \n", planet_ID);
        return success;
    }
    printf("What is your Jerry's dimension ? \n");
    scanf("%s", jerry_dimension);
    printf("How happy is your Jerry now ? \n");
    scanf("%d", &happiness);
    Jerry *new = addjerrytotabele(hashjerry, jerry_ID, jerry_dimension, happiness,planet, alljerries );
    if (new == NULL) {
        return  failure;
    }
    return success;
}

// Handle the addition of a physical characteristic to an existing Jerry
status case2(hashTable JerrysHashTable, multiValueHashTable PC_MultiHashTable) {
    char jerry_pc[301] = {0};
    char jerry_ID[301] ={0};
    float val= 0;
    printf("What is your Jerry's ID ? \n");
    scanf("%s", jerry_ID);
    Jerry *jerry = lookupInHashTable(JerrysHashTable, jerry_ID);
    if (jerry == NULL) {
        printf("Rick this Jerry is not in the daycare ! \n");
        return success;
    }
    printf("What physical characteristic can you add to Jerry - %s ? \n", jerry_ID);
    scanf("%s", jerry_pc );
    if (cheak_if_pc(jerry, jerry_pc)){
        printf("The information about his %s already available to the daycare ! \n", jerry_pc);
        return success;
    }
    printf("What is the value of his %s ? \n", jerry_pc);
    scanf("%f", &val);
    status continue_plan;
    continue_plan = addpctojerryhash(PC_MultiHashTable, jerry, jerry_pc, val);
    if (continue_plan == failure) {
        return failure;
    }
    continue_plan = displayMultiValueHashElementsByKey(PC_MultiHashTable, jerry_pc);
    if (continue_plan == failure) {
        return failure;
    }
    return success;
}

// Handle the removal of a physical characteristic from an existing Jerry
status case3(hashTable hashjerry, multiValueHashTable multihashpc ) {
    char jerry_pc[301] = {0};
    char jerry_ID[301] ={0};
    printf("What is your Jerry's ID ? \n");
    scanf("%s", jerry_ID);
    Jerry *jerry1 = lookupInHashTable(hashjerry, jerry_ID);
    if (jerry1 == NULL) {
        printf("Rick this Jerry is not in the daycare ! \n");
        return success;
    }
    printf("What physical characteristic do you want to remove from Jerry - %s ? \n", jerry_ID);
    scanf("%s", jerry_pc );
    if (cheak_if_pc(jerry1, jerry_pc)==false){
        printf("The information about his %s not available to the daycare ! \n", jerry_pc);
        return success;
    }
    status continue_plan = removepcfromjerry(multihashpc, jerry1, jerry_pc);
    if (continue_plan == failure) {
        return failure;
    }
    print_jerry(jerry1);
    return success;
}

// Handle the removal of a Jerry from the daycare
status case4(hashTable hashjerry, multiValueHashTable multihashpc , linkedlist alljerries ) {
    char jerry_ID[301] ={0};
    printf("What is your Jerry's ID ? \n");
    scanf("%s", jerry_ID);
    Jerry *jerry2 = lookupInHashTable(hashjerry, jerry_ID);
    if (jerry2 == NULL) {
        printf("Rick this Jerry is not in the daycare ! \n");
        return success;
    }
    status s = removejerry(multihashpc, hashjerry, jerry2, alljerries);
    if (s == failure) {
        return failure;
    }
    printf("Rick thank you for using our daycare service ! Your Jerry awaits ! \n");
    return success;
}

// Handle finding a similar Jerry based on physical characteristics
status case5(hashTable hashjerry, multiValueHashTable multihashpc , linkedlist alljerries ) {
    char jerry_pc[301] = {0};
    float val= 0;
    printf("What do you remember about your Jerry ? \n");
    scanf("%s", jerry_pc);
    if( lookupInMultiValueHashTable(multihashpc, jerry_pc) == NULL) {
        printf("Rick we can not help you - we do not know any Jerry's %s ! \n", jerry_pc);
        return success;
    }
    printf("What do you remember about the value of his %s ? \n", jerry_pc);
    scanf("%f", &val);
    Jerry *to_remove = similarjerry(hashjerry, multihashpc, jerry_pc, val);
    printf("Rick this is the most suitable Jerry we found : \n");
    print_jerry(to_remove);
    status s = removejerry(multihashpc, hashjerry, to_remove,alljerries);
    if (s == failure) {
        return failure;
    }
    printf("Rick thank you for using our daycare service ! Your Jerry awaits ! \n");
    return success;
}

// Handle finding and removing the saddest Jerry
status case6(hashTable hashjerry, multiValueHashTable multihashpc, linkedlist alljerries ) {
    if (getLengthList(alljerries) > 0) {
        printf("Rick this is the most suitable Jerry we found : \n");
        print_jerry(saddestjerry(alljerries));
        removejerry(multihashpc, hashjerry,saddestjerry(alljerries),alljerries );
        printf("Rick thank you for using our daycare service ! Your Jerry awaits ! \n");
    } else {
        printf("Rick we can not help you - we currently have no Jerries in the daycare ! \n");
    }
    return success;
}

// Handle displaying information about Jerries or planets
status case7(hashTable hashjerry, multiValueHashTable multihashpc, linkedlist alljerries, PlanetList *planetList) {
    printf("What information do you want to know ? \n"
           "1 : All Jerries \n"
           "2 : All Jerries by physical characteristics \n"
           "3 : All known planets \n");
    char jerry_pc[301] = {0};
    char choice7[301] = {0};
    int choice = 0;
    scanf("%s",choice7);
    if (is_valid2(choice7)) {
        choice = atoi(choice7);
    }
    else {
        choice = 4;
    }
        switch (choice) {
            case 1: // All Jerries
                if (getLengthList(alljerries) > 0) {
                    printList(alljerries);
                } else {
                    printf("Rick we can not help you - we currently have no Jerries in the daycare ! \n");
                }
            break;

            case 2:

                    // Jerries by physical characteristics
                    printf("What physical characteristics ? \n");
                    if (scanf("%300s", jerry_pc) != 1) { // Limit input length to avoid buffer overflow
                        printf("Rick invalid input for physical characteristics ! \n");
                        return success;
                    }
                    if (lookupInMultiValueHashTable(multihashpc, jerry_pc) != NULL) {
                        displayMultiValueHashElementsByKey(multihashpc, jerry_pc);
                    } else {
                        printf("Rick we can not help you - we do not know any Jerry's %s ! \n", jerry_pc);
                    }

            break;

            case 3: // All known planets
                if (planetList == NULL || planetList->size == 0) {
                    printf("Rick we can not help you - no planets are available ! \n");
                    break;
                    }
                    for (int i = 0; i < planetList->size; i++) {
                        print_planet(planetList->planets[i]);
                    }
                    break;

            default:
                printf("Rick this option is not known to the daycare ! \n");
            break;
        }

    return success;
}

// Handle initiating activities for Jerries
status case8(linkedlist alljerries) {
    if (getLengthList(alljerries) > 0) {
        printf("What activity do you want the Jerries to partake in ? \n"
            "1 : Interact with fake Beth \n"
            "2 : Play golf \n"
            "3 : Adjust the picture settings on the TV \n");
        char choice8[301] = {0};
        int choice3 = 0;
        scanf("%s",choice8);
        if (is_valid2(choice8)) {
            choice3 = atoi(choice8);
        }
        else {
            choice3 = 4;
        }
            switch (choice3) {
                case 1:
                    update_happiness(alljerries,20,15,5);
                    printf("The activity is now over ! \n");
                    printList(alljerries);
                break;
                case 2:
                    update_happiness(alljerries,50,10,10);
                    printf("The activity is now over ! \n");
                    printList(alljerries);
                break;
                case 3:
                    update_happiness(alljerries,0,20,0);
                    printf("The activity is now over ! \n");
                    printList(alljerries);
                break;
                case 4:
                    printf("Rick this option is not known to the daycare ! \n");
                    break;
            }
        return success;
        }
    printf("Rick we can not help you - we currently have no Jerries in the daycare ! \n");
    return success;
}

// Handle closing the daycare and cleaning up all data
status case9(linkedlist alljerries, multiValueHashTable multihashpc, hashTable hashjerry, PlanetList *planetList) {
    cleanall(alljerries, multihashpc, hashjerry, planetList);
    printf("The daycare is now clean and close ! \n");
    return success;
}

int main(int argc, char *argv[]) {
    // Parse number of planets and data file name from command line arguments
    int num_of_planets = atoi(argv[1]); // Get number of planets
    char *datafile = argv[2]; // Get data file name

    // Initialize counters for Jerries and planetary characteristics
    int  numofjerrys = 0;
    int  numofpc = 0;
    count_elements_infile(datafile, &numofjerrys, &numofpc); // Count elements in the data file

    // Find the nearest prime numbers for hash table sizes
    numofjerrys  = find_close_prime(numofjerrys);
    numofpc = find_close_prime(numofpc);

    // Initialize data structures
    PlanetList *planetList = NULL;

    // Create a linked list for Jerries
    linkedlist alljerries = createLinkedList(copyJerryVal, NOTfreejerrys, equaljerrys, print_jerry_val);
    if (!alljerries) {
        printf(" A memory problem has been detected in the program");
        exit(1);
    }

    // Create a hash table for Jerries
    hashTable hashjerry = createHashJerry(numofjerrys);
    if (!hashjerry) {
        destroyList(alljerries);
        printf(" A memory problem has been detected in the program");
        exit(1);
    }

    // Create a multi-value hash table for planetary characteristics
    multiValueHashTable multihashpc = createMultiValueHashTablePC(numofpc);
    if (!multihashpc) {
        destroyList(alljerries);
        destroyHashTable(hashjerry);
        exit(1);
    }

    // Load data from the file into the data structures
    status op_status = load_file(datafile, &planetList, hashjerry, num_of_planets, multihashpc, alljerries);
    if (op_status == failure) {
        // Cleanup on failure
        cleanall(alljerries, multihashpc, hashjerry, planetList);
        printf(" A memory problem has been detected in the program");
        exit(1);
    }

    // Main program loop
    status continue_plan = success;
    while (continue_plan == success) {
        print_menu(); // Display the menu
        int choice = 0;
        char input_choice[301] = {0};
        scanf("%s", input_choice); // Get user input
        if (is_valid(input_choice)) { // Validate input
            choice = atoi(input_choice); // Convert input to integer
        }

        // Handle menu options
        switch (choice) {
            case 1:
                continue_plan = case1(hashjerry, planetList, alljerries);
                break;
            case 2:
                continue_plan = case2(hashjerry, multihashpc);
                break;
            case 3:
                continue_plan = case3(hashjerry, multihashpc);
                break;
            case 4:
                continue_plan = case4(hashjerry, multihashpc, alljerries);
                break;
            case 5:
                continue_plan = case5(hashjerry, multihashpc, alljerries);
                break;
            case 6:
                continue_plan = case6(hashjerry, multihashpc, alljerries);
                break;
            case 7:
                continue_plan = case7(hashjerry, multihashpc, alljerries, planetList);
                break;
            case 8:
                continue_plan = case8(alljerries);
                break;
            case 9:
                case9(alljerries, multihashpc, hashjerry, planetList);
                exit(0);
            default:
                printf("Rick this option is not known to the daycare ! \n");
                break;
        }
    }

    // Cleanup and exit on error
    cleanall(alljerries, multihashpc, hashjerry, planetList);
    printf(" A memory problem has been detected in the program \n");
    exit(1);
}






