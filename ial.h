/**
 * Predmet:  IFJ / IAL
 * Projekt:  Implementace interpretu imperativniho jazyka
 * Varianta: a/1/I
 * Soubor:   ial.h
 *
 * Popis:
 *
 *
 * Datum:    21.11.2013
 *
 * Autori:   Frantisek Kolacek   <xkolac12@stud.fit.vutbr.cz>
 *           Stodulka Daniel
 *           Hermann Lukas
 *           Tran Manh Hoang
 */

#ifndef IAL_H_INCLUDED
#define IAL_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "strings.h"

/*
 Metoda vyhledávání, kterou bude využívat vestavˇená funkce find_string:
	Pro vyhledávání použijte Knuth-Moris-Pratt ̊uv algoritmus.
	
Vyhledá první výskyt zadaného podˇretˇezce v řeťezci a vrátí jeho pozici 
(poˇcítáno od nuly). První parametr je ˇretˇezec, ve kterém
bude daný podˇretˇezec vyhledáván. Druhým parametrem je vyhledávaný podˇretˇezec.
Prázdný ˇretˇezec se vyskytuje v libovolném ˇretˇezci na pozici 0 .
Pokud podˇretˇezec není nalezen, je vrácena hodnota -1 . 
*/

int find_string(STRING, STRING);

/*
Metoda řazení, kterou bude využívat vestavěná funkce sort_string:
	Pro řazení použijte algoritmus Quick sort.
Seˇradí znaky v daném ˇretˇezci tak, aby znak s nižší ordi-
nální hodnotou vždy pˇredcházel znaku s vyšší ordinální hodnotou. Vrácen je ˇretˇezec
obsahující seˇrazené znaky.
*/

void sort_string(STRING);

/*
Tabulka symbol ̊u bude implementována pomocí abstraktní datové struktury:
	Tabulku symbolů implementujte pomocí binárního vyhledávacího stromu.
*/







#endif