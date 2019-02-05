//
//  initializerOfStructures.h
//  Мракобес
//
//  Created by vlad on 13.10.2018.
//  Copyright © 2018 vlad. All rights reserved.
//

#ifndef initializerOfStructures_h
#define initializerOfStructures_h

#include "StructuresForReading.h"


Text* getTextObject(void);
void propertyDeinitializer(Text* text);
void attributeDeinitializer(ListOfAttributesWithArguments** listOfAttribute);
#endif /* initializerOfStructures_h */
