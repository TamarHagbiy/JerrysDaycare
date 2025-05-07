#include "Jerry.h"

// Create a coordinate structure from given x, y, z values
coord create_coord(float x, float y, float z) {
    coord new_coord = {x, y, z};
    return new_coord; // Return a new coordinate structure
}

// Create a physical characteristic
PhysicalCharacteristics * create_physical_characteristics(char *pc_name, float val) {
    if (!pc_name) { // Check for NULL inputs
        return NULL;
    }

    // Allocate memory for a new PhysicalCharacteristic
    PhysicalCharacteristics * new_pc = malloc(sizeof(PhysicalCharacteristics));
    if (!new_pc) {
        new_pc = NULL; // Explicitly set to NULL for clarity
        return NULL; // Return failure if malloc fails
    }

    // Allocate memory for the name field
    new_pc->name = malloc(strlen(pc_name) + 1);
    if (!new_pc->name) {
        free(new_pc); // Free the PhysicalCharacteristic if name allocation fails
        new_pc = NULL;
        return NULL;
    }

    // Copy the name and set the value
    strcpy(new_pc->name, pc_name);
    new_pc->val = val;
    return new_pc;
}

// Add a PhysicalCharacteristic to Jerry's list
status add_pc_to_jerry(Jerry *jerry, PhysicalCharacteristics *new_pc) {
    if (!new_pc) {
        return failure;
    }
    // Reallocate memory for the array of PhysicalCharacteristics
    PhysicalCharacteristics **new_pc_ptr = realloc(jerry->PhysicalCharacteristics,
                                                    (jerry->pc_num + 1) * sizeof(PhysicalCharacteristics *));
    if (!new_pc_ptr) {
        free_physical_characteristics(new_pc); // Free the PhysicalCharacteristic
        return failure; // Return failure if realloc fails
    }

    // Update Jerry's list of PhysicalCharacteristics
    jerry->PhysicalCharacteristics = new_pc_ptr;
    jerry->PhysicalCharacteristics[jerry->pc_num] = new_pc;
    jerry->pc_num++;
    return success;
}

// Free memory allocated for a PhysicalCharacteristic
void free_physical_characteristics(PhysicalCharacteristics *pc) {
    if (!pc) return;
    if (pc->name) {
        free(pc->name); // Free the name field
        pc->name = NULL;
    }
    free(pc); // Free the PhysicalCharacteristic structure
}

// Create a new Planet structure
Planet *create_planet(char *pc_name , float x, float y, float z) {
    if (!pc_name) { // Check if the name is NULL
        return NULL;
    }
    Planet *new_planet = malloc(sizeof(Planet));
    if (!new_planet) {
        return NULL; // Return NULL if memory allocation fails
    }
    new_planet->name = malloc(strlen(pc_name) + 1);
    if (!new_planet->name) {
        free(new_planet); // Free Planet structure if name allocation fails
        return NULL;
    }
    strcpy(new_planet->name, pc_name);
    new_planet->coord = create_coord(x, y, z); // Set the coordinates
    return new_planet;
}

// Free memory allocated for a Planet structure
void free_planet(Planet *planet) {
    if (!planet) {
        return;
    }
    if (planet->name) {
        free(planet->name); // Free the name field
        planet->name = NULL;
    }
    free(planet); // Free the Planet structure
}

// Create a new Origin structure
Origin *create_origin(Planet *planet, char *reality) {
    if (!planet || !reality) { // Check for NULL inputs
        return NULL;
    }
    Origin *new_origin = malloc(sizeof(Origin));
    if (!new_origin) {
        new_origin = NULL; // Explicitly set to NULL
        return new_origin;
    }
    new_origin->planet = planet;
    new_origin->reality = malloc(strlen(reality) + 1);
    if (!new_origin->reality) {
        free(new_origin); // Free Origin structure if reality allocation fails
        return NULL;
    }
    strcpy(new_origin->reality, reality); // Set the reality field
    return new_origin;
}

// Free memory allocated for an Origin structure
void free_origin(Origin *origin) {
    if (!origin) return;
    if (origin->reality) {
        free(origin->reality); // Free the reality field
        origin->reality = NULL;
    }
    free(origin); // Free the Origin structure
}

// Create a Jerry structure
Jerry *create_jerry(char *Id, char * reality, Planet *planet, int happiness) {
    if (!Id || !planet) { // Check for NULL inputs
        return NULL;
    }
    Origin *origin = create_origin(planet, reality);
    if (!origin) {
      return NULL; // Return NULL if Origin creation fails
    }
    Jerry *new_jerry = malloc(sizeof(Jerry));
    if (!new_jerry) {
        new_jerry = NULL; // Explicitly set to NULL
        return new_jerry;
    }
    new_jerry->Id = malloc(strlen(Id) + 1);
    if (!new_jerry->Id) {
        free(new_jerry); // Free Jerry structure if Id allocation fails
        return NULL;
    }
    strcpy(new_jerry->Id, Id);
    new_jerry->origin = origin;
    new_jerry->happiness = happiness;
    new_jerry->pc_num = 0;
    new_jerry->PhysicalCharacteristics = NULL;
    return new_jerry;
}

// Free memory allocated for a PhysicalCharacteristic
void free_pc(PhysicalCharacteristics *pc) {
    if (!pc) return;
    if (pc->name) {
        free(pc->name); // Free the name field
        pc->name = NULL;
    }
    free(pc); // Free the PhysicalCharacteristic structure
}

// Free memory allocated for a Jerry structure
status free_jerry(Jerry *jerry) {
    if (!jerry) return failure;

    if (jerry->Id) {
        free(jerry->Id); // Free the ID
        jerry->Id = NULL;
    }
    if (jerry->origin) {
       free_origin(jerry->origin); // Free the Origin structure
    }

    if (jerry->PhysicalCharacteristics) {
        for (int i = 0; i < jerry->pc_num; i++) {
            if (jerry->PhysicalCharacteristics[i]) {
                free_pc(jerry->PhysicalCharacteristics[i]); // Free each PhysicalCharacteristic
                jerry->PhysicalCharacteristics[i] = NULL;
            }
        }
        free(jerry->PhysicalCharacteristics); // Free the array of PhysicalCharacteristics
        jerry->PhysicalCharacteristics = NULL;
    }
    free(jerry); // Free the Jerry structure
    return success;
}

// Delete a PhysicalCharacteristic from Jerry
status delete_pc_to_jerry(Jerry *jerry, char *pc_name) {
    if (!jerry || !pc_name || !jerry->PhysicalCharacteristics) { // Check for NULL inputs
        return failure;
    }
    for (int i = 0; i < jerry->pc_num; i++) {
        if (strcmp(pc_name, jerry->PhysicalCharacteristics[i]->name) == 0) {
            free_pc(jerry->PhysicalCharacteristics[i]); // Free the matching PhysicalCharacteristic
            jerry->PhysicalCharacteristics[i] = NULL;
            for (int j = i; j < jerry->pc_num - 1; j++) {
                jerry->PhysicalCharacteristics[j] = jerry->PhysicalCharacteristics[j + 1]; // Shift elements
            }
            jerry->PhysicalCharacteristics[jerry->pc_num - 1] = NULL;
            jerry->pc_num--;
            if (jerry->pc_num == 0){
                free(jerry->PhysicalCharacteristics); // Free the array if empty
                jerry->PhysicalCharacteristics = NULL;
                return success;
            }
            PhysicalCharacteristics **temp = realloc(jerry->PhysicalCharacteristics, jerry->pc_num * sizeof(PhysicalCharacteristics *));
            if (!temp) { // Handle realloc failure
                return failure;
            }
            jerry->PhysicalCharacteristics = temp; // Assign the new memory block
            return success;
        }
    }
    return success;
}

// Check if a PhysicalCharacteristic exists in Jerry's list
bool cheak_if_pc(Jerry *jerry, char *pc_name) {
    if (!jerry || !pc_name || !jerry->PhysicalCharacteristics) { // Check for NULL inputs
        return false;
    }
    for (int i = 0; i < jerry->pc_num; i++) {
        if (strcmp(pc_name, jerry->PhysicalCharacteristics[i]->name) == 0) {
            return true; // Return true if the characteristic exists
        }
    }
    return false;
}

// Print information about a Planet
void print_planet(Planet *planet) {
    if (!planet) return;
    printf("Planet : %s (%.2f,%.2f,%.2f) \n", planet->name, planet->coord.x, planet->coord.y, planet->coord.z);
}

// Print information about a Jerry
status print_jerry(Jerry *jerry) {
    if (!jerry) return failure;
    printf("Jerry , ID - %s : \n", jerry->Id);
    printf("Happiness level : %d \n", jerry->happiness);
    printf("Origin : %s \n", jerry->origin->reality);
    print_planet(jerry->origin->planet);
    if (jerry->pc_num > 0) {
        printf("Jerry's physical Characteristics available : \n\t");
        for (int i = 0; i < jerry->pc_num; i++) {
            printf("%s : %.2f ", jerry->PhysicalCharacteristics[i]->name,
                   (float)jerry->PhysicalCharacteristics[i]->val);
            if (i < jerry->pc_num - 1) {
                printf(", ");
            }
        }
        printf("\n");
    }
    return success;
}

// Get the ID of a Jerry
char * getjerryid(Jerry *jerry){
    if (!jerry) return NULL;
    return jerry->Id; // Return the ID of the Jerry
}

// Get the happiness level of a Jerry
int getjerryhappiness(Jerry *jerry){
    if (!jerry) return 0;
    return jerry->happiness; // Return the happiness level of the Jerry
}

// Get the number of PhysicalCharacteristics of a Jerry
int getjerrynumpc(Jerry *jerry){
    if (!jerry) return 0;
    return jerry->pc_num; // Return the number of PhysicalCharacteristics
}
