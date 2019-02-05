//
//  SymbolHendler.h
//  Мракобес
//
//  Created by vlad on 13.10.2018.
//  Copyright © 2018 vlad. All rights reserved.
//

#ifndef SymbolHendler_h
#define SymbolHendler_h

#include <stdio.h>
#include "StructuresForReading.h"
//SYMBOL HANDALING
void deleteSpaces(void);
void deleteSpacesInFileName(char* symbol);

ClassifiacationOfSymbols space(currentConfigurationsForText* configurations,Text* text,char* symbol);
ClassifiacationOfSymbols quote(currentConfigurationsForText* configurations);
ClassifiacationOfSymbols endOfCommand(currentConfigurationsForText* configurations,Text* text,char* symbol);
ClassifiacationOfSymbols slesh(currentConfigurationsForText* configurations,Text* text,char* symbol);
ClassifiacationOfSymbols newAttribute(currentConfigurationsForText* configurations);

ClassifiacationOfSymbols backGroundMode(currentConfigurationsForText* configurations,Text* text,char* symbol);

ClassifiacationOfSymbols treatmentOfOuttoConveyerRedirectional(currentConfigurationsForText* configurations,
                                                               Text* text,
                                                               char* symbol,
                                                               bool* redirectionModeWasClose,
                                                               bool* redirectionModeWasOpen);

ClassifiacationOfSymbols treatmentOfIntoRedirectional(currentConfigurationsForText* configurations,
                                                      Text* text,
                                                      char* symbol,
                                                      bool* conveyorModeWasClose,
                                                      bool* conveyorModeWasOpen);

ClassifiacationOfSymbols redirectionMode(currentConfigurationsForText* configurations,
                                      Text* text,
                                      DirectionalOfConveyorMode directional,
                                      char* symbol);
ClassifiacationOfSymbols conveyorMode(Text* text,currentConfigurationsForText* configurations);

#endif /* SymbolHendler_h */
