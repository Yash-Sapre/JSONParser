#include<stdio.h>
#include <stdlib.h>
#include<string.h>


// Simple JSON Parser in C
// Given below is the grammer I used for JSON Parsing
// E -> {R}  
// R -> "t" : Q| "t" : Q ,R
// Q -> "t" | E

char json[100] ;
int top = 0;

void error(){
    printf("Invalid JSON");
}

void skipSpacesAndNewLines(){
    while((json[top] == '\n') || (json[top] == ' '))
        top++;
}

void skipToTheQuotes(){
    while((json[top] != '"') )
        top++;
}
void R();
void Q();


void E(){
    skipSpacesAndNewLines();
    if(json[top] == '{'){
        top++;
        skipSpacesAndNewLines();
        R();
        // printf("%c",json[top]);
        if(json[top] == '}'){
            top++;
            skipSpacesAndNewLines();
            if(json[top] == '\0')
                printf("Valid JSON\n");
        }
    }


}

void R(){

    if(json[top] == '"'){
        top++;
        skipToTheQuotes();
        if(json[top] == '"'){
            top++;
            skipSpacesAndNewLines();
            if(json[top] == ':'){
                top++;
                skipSpacesAndNewLines();
                Q();
                skipSpacesAndNewLines();

                if(json[top] == ','){
                    top++;
                    skipSpacesAndNewLines();
                    R();
                }
            }
        }
        else
            error();
    }
}



void Q(){
    skipSpacesAndNewLines();

    if(json[top] == '{'){

        E();
    }
    
    if(json[top] == '"'){
        top++;
        skipToTheQuotes();

        if(json[top] == '"'){
            top++;
            skipSpacesAndNewLines();
        }
        else
            error();

    }
}

void main(){
    FILE *fp;
    char filename[] = "testing.json"; // Replace with your file name

    fp = fopen(filename, "r");
    char line[50];
    while(fgets(line, 10, fp))
        strcat(json,line);
    printf("The JSON is : \n%s\n",json);

    fclose(fp); // Close the file when done
    E();
    printf("%c",json[top]);

}