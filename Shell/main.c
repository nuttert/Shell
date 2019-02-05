//
//
//  Created by vlad on 11.09.2018.
//  Copyright © 2018 vlad. All rights reserved.
//

#include<unistd.h>
#include <fcntl.h>
#include "initializerOfStructures.h"
#include "CommandHendler.h"
#include "StructuresForReading.h"
//READING
void readNewCommand(Text* text){
    char symbol = getchar();
    while (symbol != EOF){
        append(text, symbol);
        symbol = getchar();
    }
}

//MAIN
int main() {
    Text* text = getTextObject();
    startString();
    readNewCommand(text);
    return 0;
}







//Запись данных в файл
/*
 FILE* file = fopen("myfile.txt","w");
 fprintf (file,"cat 1.txt > file.txt\n");
 fclose(file);
 int file_desc = open("myfile.txt",0);
 
 
 dup2(file_desc,STDIN_FILENO);
 close(file_desc);
 */

