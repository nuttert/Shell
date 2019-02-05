//
//  CommandHendler.h
//  Мракобес
//
//  Created by vlad on 13.10.2018.
//  Copyright © 2018 vlad. All rights reserved.
//

#ifndef CommandHendler_h
#define CommandHendler_h
#include "StructuresForReading.h"
typedef enum numbersOfPosition{
    positionByFirstCommand = 1
}numbersOfPosition;
int classicCommandHandler(Text* text,currentConfigurationsForText* configurations);
void backgroundModeHandler(Text* text,currentConfigurationsForText* configurations);
void startString(void);
void choiceCommandStatement(Text* text,currentConfigurationsForText* configurations);
bool backgroundCommandHandler(Text* text,currentConfigurationsForText* configurations);
ClassifiacationOfSymbols setDescriptorsForRediractions(Text* text,
                                                       char* fileName,
                                                       bool clearFile,
                                                       currentConfigurationsForText* configurations,
                                                       DirectionalOfConveyorMode directional);
#endif /* CommandHendler_h */
