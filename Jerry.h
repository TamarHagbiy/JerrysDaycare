//
// Created by tamar on 30/11/2024.
//

#ifndef JERRY_H
#define JERRY_H
#include "Defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struct Definitions

/**
 * @struct coord
 * Represents a 3D coordinate in space.
 * Contains x, y, and z values as floating-point numbers.
 */
typedef struct {
    float x; ///< X-coordinate
    float y; ///< Y-coordinate
    float z; ///< Z-coordinate
} coord;

/**
 * Creates a new 3D coordinate.
 * @param x X-coordinate value.
 * @param y Y-coordinate value.
 * @param z Z-coordinate value.
 * @return A new coord object.
 */
coord create_coord(float x, float y, float z);

/**
 * @struct Planet
 * Represents a planet with a name and its spatial coordinates.
 */
typedef struct {
    char *name; ///< The name of the planet
    coord coord; ///< The planet's coordinates in 3D space
} Planet;

/**
 * @struct Origin
 * Represents the origin of a Jerry, including its planet and reality.
 */
typedef struct {
    Planet *planet; ///< Pointer to the planet of origin
    char *reality; ///< The reality of the Jerry (e.g., C-137)
} Origin;

/**
 * @struct PhysicalCharacteristics
 * Represents a physical characteristic of a Jerry (e.g., height, weight).
 */
typedef struct {
    char *name; ///< The name of the characteristic (e.g., "Height")
    float val; ///< The value of the characteristic (e.g., 170.5 for height)
} PhysicalCharacteristics;

/**
 * @struct Jerry
 * Represents a Jerry character with an ID, origin, physical characteristics, happiness level, and more.
 */
typedef struct {
    char *Id; ///< Unique identifier for the Jerry
    Origin *origin; ///< Origin of the Jerry, including its planet and reality
    PhysicalCharacteristics **PhysicalCharacteristics; ///< Array of physical characteristics
    int pc_num; ///< Number of physical characteristics
    int happiness; ///< Happiness level of the Jerry
} Jerry;

// Function Declarations

/**
 * Frees a physical characteristic object.
 * @param pc Pointer to the PhysicalCharacteristics object to free.
 */
void free_physical_characteristics(PhysicalCharacteristics *pc);

/**
 * Creates a new Planet with the given name and coordinates.
 * @param pc_name Name of the planet.
 * @param x X-coordinate of the planet.
 * @param y Y-coordinate of the planet.
 * @param z Z-coordinate of the planet.
 * @return Pointer to the newly created Planet, or NULL if memory allocation fails.
 */
Planet *create_planet(char *pc_name, float x, float y, float z);

/**
 * Frees the memory allocated for a Planet object.
 * @param planet Pointer to the Planet object to free.
 */
void free_planet(Planet *planet);

/**
 * Prints the details of a Planet to the console.
 * @param planet Pointer to the Planet to print.
 */
void print_planet(Planet *planet);

/**
 * Frees the memory allocated for an Origin object.
 * @param origin Pointer to the Origin object to free.
 */
void free_origin(Origin *origin);

/**
 * Creates a new Jerry object with the given details.
 * @param Id Unique ID for the Jerry.
 * @param reality Reality string (e.g., "C-137").
 * @param planet Pointer to the planet of origin.
 * @param happiness Initial happiness level.
 * @return Pointer to the newly created Jerry, or NULL if memory allocation fails.
 */
Jerry *create_jerry(char *Id, char *reality, Planet *planet, int happiness);

/**
 * Frees the memory allocated for a Jerry object.
 * @param jerry Pointer to the Jerry object to free.
 * @return `success` if the Jerry was successfully freed, otherwise `failure`.
 */
status free_jerry(Jerry *jerry);

/**
 * Frees a physical characteristic object.
 * @param pc Pointer to the PhysicalCharacteristics object to free.
 */
void free_pc(PhysicalCharacteristics *pc);

/**
 * Creates a new physical characteristic with the given name and value.
 * @param pc_name The name of the physical characteristic (e.g., "Height"). Must not be NULL.
 * @param val The value of the physical characteristic (e.g., 180.5).
 * @return Pointer to the newly created PhysicalCharacteristics structure, or NULL if memory allocation fails or input is invalid.
 */
PhysicalCharacteristics *create_physical_characteristics(char *pc_name, float val);

/**
 * Adds a physical characteristic to a Jerry.
 * @param jerry Pointer to the Jerry to modify.
 * @param physical_characteristics Pointer to the PhysicalCharacteristics to add.
 * @return `success` if the characteristic was added, otherwise `failure`.
 */
status add_pc_to_jerry(Jerry *jerry, PhysicalCharacteristics *physical_characteristics);

/**
 * Removes a physical characteristic from a Jerry.
 * @param jerry Pointer to the Jerry to modify.
 * @param pc_name Name of the characteristic to remove.
 * @return `success` if the characteristic was removed, otherwise `failure`.
 */
status delete_pc_to_jerry(Jerry *jerry, char *pc_name);

/**
 * Checks if a Jerry has a specific physical characteristic.
 * @param jerry Pointer to the Jerry to check.
 * @param pc_name Name of the characteristic to check for.
 * @return `true` if the characteristic exists, otherwise `false`.
 */
bool cheak_if_pc(Jerry *jerry, char *pc_name);

/**
 * Prints the details of a Jerry to the console.
 * @param jerry Pointer to the Jerry to print.
 * @return `success` if the Jerry was printed successfully, otherwise `failure`.
 */
status print_jerry(Jerry *jerry);

/**
 * Gets the ID of a Jerry.
 * @param jerry Pointer to the Jerry.
 * @return The ID string of the Jerry.
 */
char *getjerryid(Jerry *jerry);

/**
 * Gets the happiness level of a Jerry.
 * @param jerry Pointer to the Jerry.
 * @return The happiness level of the Jerry.
 */
int getjerryhappiness(Jerry *jerry);

/**
 * Gets the number of physical characteristics of a Jerry.
 * @param jerry Pointer to the Jerry.
 * @return The number of physical characteristics.
 */
int getjerrynumpc(Jerry *jerry);

#endif //JERRY_H
