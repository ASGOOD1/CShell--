//functie cu care citim si afisam toate liniile dintr-un fisier
int read_all_lines(char * filename) {
    int fd = open(filename, O_RDONLY); // deschidem fisierul
    
    if (fd == -1) { // verificam daca exista si am putut sa-l deschidem
        printf("Error: %s doesn't exist.\n", filename);
        return 1;
    }
    char buffer[BUFFER_SIZE]; //initializare buffer pt citirea liniilor caracter cu caracter 
    char line[BUFFER_SIZE]; // initializare buffer pt stocarea liniilor 
    int bytesRead;// nr de octeti cititi intr-o operatiune de citire
    int lineIndex = 0; //indicele curent al sirului line

    while ((bytesRead = read(fd, buffer, BUFFER_SIZE)) > 0) { //se citeste din fisier cat timp mai sunt octeti de citit adica >0
                                                                //la fiecare iteratie se citeste un bloc de date de dimensiune BUFFER_SIZE si se stocheaza in buffer
        for (int i = 0; i < bytesRead; i++) { // se parcurge fiecare caracter din buffer ul citit 
            if (buffer[i] == '\n' || buffer[i] == '\0') { //daca e final de linie sau fisier
                line[lineIndex] = '\0'; //se pune terminatorul de sir
                printf("%s\n", line); // printam linia
                lineIndex = 0; //se pune indexul pe 0
            } else { // altfel daca nu e final de linie sau fisier
                line[lineIndex++] = buffer[i]; // se adauga urmatorul caracter in line 
                if (lineIndex >= BUFFER_SIZE - 1) { //// in caz de se atinge bufferul resetam indexul
                    lineIndex = 0;
                }
            }
        }
    }
    close(fd);
  
  
    if (bytesRead == -1) { // failsafe - nu a putut sa citeasca fisierul
        printf("Error: %s doesn't exist.\n", filename);
        return 1;
    }

    return 0;
} 
//comenzile incluse
#include "first.c" 
#include "last.c"
#include "cat.c"
#include "ls.c"

int mySystem(const char * cmd) {
    if(started != 0) { // fail-safe bad start
        if(strstr(cmd, "cls") != NULL)  {// daca comanda e cls se curata ecranul
            printf("\033[H\033[J"); // printam codul ANSI al caracterului ESC (clear screen)
        }
        else if(strstr(cmd, "first") != NULL){ // daca comanda e first se apeleaza functia first 
            if(strlen(cmd) <= 6) return printf("first [-l(ines) lines] filename1 filename2 etc\nPrints first l lines\n"); // verificare numar de caractere in caz ca se scrie doar "first" se afiseaza cum se utilizeaza comanda
            first(cmd);// apelam comanda
        }
        else if(strstr(cmd, "last") != NULL) {// daca comanda e last se apeleaza functia last
            if(strlen(cmd) <= 5) return printf("last [-l(ines) lines] filename1 filename2 etc\nPrints last l lines\n");// verificare numar de caractere in caz ca se scrie doar "last" se afiseaza cum se utilizeaza comanda
            first(cmd);// apelam comanda
            last(cmd);
        }
        else if(strstr(cmd, "cat") != NULL) {// daca comanda e cat se apeleaza functia cat
            if(strlen(cmd) <= 4) return printf("cat [-b|-c] filename1 filename2 etc\nPrint the content of the files\n-b - all non-empty lines\n-c - all lines counted\n");// verificare numar de caractere in caz ca se scrie doar "cat" se afiseaza cum se utilizeaza comanda
            cat(cmd);
        }
        else if(strstr(cmd, "ls") != NULL) {// daca comanda e cat se apeleaza functia ls
            ls(cmd);
        }
        else { //daca nu gaseste niciuna din comenzi returneaza unknown command
            system(cmd);
        }
    }
    if(started == 0) {// la prima executie started = 0 se pune started pe 1 si se curata ecranul de partea cand ne logam dupa system ramane pe 1 si se pot executa orice comenzi
        started = 1;
        mySystem("cls");
    }
    return 0;
}
