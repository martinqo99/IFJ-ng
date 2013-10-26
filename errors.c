/**
 * Predmet:  IFJ / IAL
 * Projekt:  Implementace interpretu jazyka PHP13
 * Varianta: a/1/I
 * Soubor:   errors.c
 *
 * Popis:
 *
 *
 * Datum:    12.9.2013
 *
 * Autori:   Frantisek Kolacek   <xkolac12@stud.fit.vutbr.cz>
 *           Stodulka Daniel
 *           Hermann Lukas
 *           Tran Manh Hoang
 */

#include "errors.h"

E_CODE gErrorCode;

char* E_MESSAGES[] = {
    [ERROR_OK] = "Preklad probehl uspesne"    
};

void programAbort(E_CODE errorCode){
    gErrorCode = errorCode;
    
    exit(errorCode);    
}