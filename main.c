#include <stdio.h>//librarie standard pt operatiuni de input/output (ex printf, scanf)
#include <unistd.h> // librarie linux pt manipulare procese fisiere etc
#include <stdlib.h>// librarie pt alocare/dezalocare memorie 
#include <fcntl.h>// librarie pt controlul fisierelor
#include <string.h>// librarie pt manipularea sirurilorde caractere
#pragma GCC diagnostic ignored "-Wunused-result"  // dezactivare warning unused result

#define BUFFER_SIZE 4092
//se defineste o variabila BUFFER_SIZE cu valoarea 4092 folosita pt dimensiunea buffer-ului

#include "login.c"
#include "system.c"
//include fisierele utilizate



int main(int argc, char * argv[])
//argc = nr argumente din linia de comanda
// argv lista argumentelor trimise din linia de comanda 
{
    char username[24];
    char pwd[144];
    printf("Username: ");
    scanf("%s", username); // citire user
    printf("Password: ");
    scanf("%s", pwd); // citire parola
    
    if(login("passwords.txt", username, pwd) == 0){ 
        printf("Incorrect login details.\n");
        return 1;
    //daca functia login returneaza 0 se primeste mesajul ca e incoret si se iese cu return 1
    }
    else {
        char command[256];
        //initializare variabila in care se stocheaza comenzile
        while(1) { // asteptare comanda noua
            printf("%s@bash > ", username); 
            //afisare personalizata sub forma [nume_utilizator]@bash > , unde [nume_utilizator] este valoarea stocata in username
            fgets(command, 256, stdin); // citire linia cu comanda (maxim 256 caractere incluseiv \n) si se stocheaza in variabila command
            mySystem(command);// apelare functia system pt comanda citita 
        }
    }
    return 0;
}
