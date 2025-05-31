
int totalLines = 0; //variabila folosita pentru incrementare(opt -b/-c)
//functie read pentru cat -b/-c
int read_cat_data_bc(char * filename, int option, int chars) {
    int fd = open(filename, O_RDONLY); // se deschide fisierul
    
    if (fd == -1) {
        printf("Error: %s doesn't exist.\n", filename);  //eroare in caz ca nu se reuseste deschiderea
        return 1;
    }
    char buffer[BUFFER_SIZE]; //buffer pt citirea caracter cu caracter a liniilor
    char line[BUFFER_SIZE]; // buffer pt stocarea liniilor citite din fisier
    int bytesRead;// nr de octeti cititi intr-o operatiune de citire
    int lineIndex = 0;//indicele curent al sirului line  

    while ((bytesRead = read(fd, buffer, BUFFER_SIZE)) > 0) {//se citeste din fisier cat timp mai sunt octeti de citit adica >0
                                                                 //la fiecare iteratie se citeste un bloc de date de dimensiune BUFFER_SIZE si se stocheaza in buffer
        for (int i = 0; i < bytesRead; i++) {//se parcurge fiecare caracter din buffer ul citit
            if(option == 1) {//optiunea -c
                printf("%c", buffer[i]); // se printeaza fiecare caracter
                chars--; // se scade din numarul de caractere
                if(chars <= 0) { //se verifica daca mai are caractere de citit
                    printf("\n");
                    break; // se opreste daca a terminat de printat caracterele date
                }
            }
            else {// optiunea -b
                if (buffer[i] == '\n' || buffer[i] == '\0') {// daca e final de linie sau fisier
                    line[lineIndex] = '\0';// se pune terminatorul de sir 
                    if(strlen(line)) { //verificam daca linia nu e goala liniile non-empty numerotate
                        totalLines++; //se incrementeaza indexul liniei
                        printf("%d\t%s\n", totalLines, line); // se afiseaza indexul si linia (neaparat non-empty)
                    }
                    lineIndex = 0; //se pune indicele pe 0 pt a se trece la urm linie
                } else {// daca nu e final de linie sau fisier 
                    line[lineIndex++] = buffer[i]; // se trece la caracterul urmator
                    if (lineIndex >= BUFFER_SIZE - 1) { // in caz de se atinge bufferul resetam indexul
                        lineIndex = 0;
                    }
                }
            }
        }
    }
    close(fd); // inchidere fisier
    if (bytesRead == -1) {
        printf("Error: %s doesn't exist.\n", filename); // fail safe in caz ca a esuat citirea fisierului
        return 1;
    }
    return 0;
} 

int cat(const char argsv[]) {
    char args[256]; //in args se va stoca toata comanda
    strcpy(args, argsv); // copierea 
    char *token = strtok(args, " ");//token = cat
    char token_local[24]; // se initializeaza token_local
    token = strtok(NULL, " ");//token = optiune / fisier
    if(token == NULL) return printf("cat [-b|-c] filename1 filename2 etc\nPrint the content of the files\n-b - all non-empty lines\n-c - all lines counted\n"); // daca nu exista optiune sau fisier se afiseaza cum se foloseste comanda
    strcpy(token_local, token); // se copiaza in token_local optiunea sau fisierul
    if(token_local[strlen(token_local)-1] == '\n')token_local[strlen(token_local)-1] = '\0'; //se verifica daca ultimul caracter al sirului este newline si daca da il inlocuieste cu \0 terminatorul de sir

    if(strcmp(token_local, "-b") == 0){ //verificare optiune -b
        totalLines = 0; 
        token = strtok(NULL, " ");//token = fisier
        if(token == NULL) return printf("cat [-b|-c] filename1 filename2 etc\nPrint the content of the files\n-b - all non-empty lines\n-c - all lines counted\n"); //daca nu exista fisierul se afiseaza cum se foloseste comanda
        while(token != NULL) { // cat timp exista fisier/ fisiere (e while pt ca pot fi mai multe fisiere)
            strcpy(token_local, token); // token_local = fisier actual
            if(token_local[strlen(token_local)-1] == '\n') token_local[strlen(token_local)-1] = '\0'; //se verifica daca ultimul caracter al sirului este newline si daca da il inlocuieste cu \0 terminatorul de sir
            read_cat_data_bc(token_local, 0, 0);//apelam functia de citire pt optiunea -b(0)
            token = strtok(NULL, " "); // se trece la urm fisier in caz ca exista 
        }
    }
    else if(strcmp(token_local, "-c") == 0){//verificare optiune -c
        totalLines = 0;
        token = strtok(NULL, " "); // token = numar
        if(token == NULL) return printf("cat [-b|-c nr] filename1 filename2 etc\nPrint the content of the files\n-b - all non-empty lines\n-c - first n charachers\n");//daca nu exista fisierul se afiseaza cum se foloseste comanda
        strcpy(token_local, token);
        if(token_local[strlen(token_local)-1] == '\n') token_local[strlen(token_local)-1] = '\0'; //se verifica daca ultimul caracter al sirului este newline si daca da il inlocuieste cu \0 terminatorul de sir
        for(int i = 0; i<strlen(token_local); i++) { // se parcurge token_local
            if(isdgr(token_local[i]) == 0) { //verificam daca token-ul este format foar din cifre
                printf("Error: Invalid number.");// daca nu eroare
                token = NULL;
                break;
            }   
        }
        int chars = atoi(token_local);
        if(chars <= 0)return printf("cat [-b|-c nr] filename1 filename2 etc\nPrint the content of the files\n-b - all non-empty lines\n-c - first n charachers\n");//daca nu este un numar pozitiv
        token = strtok(NULL, " "); //token=fisier
        if(token == NULL) return printf("cat [-b|-c nr] filename1 filename2 etc\nPrint the content of the files\n-b - all non-empty lines\n-c - first n charachers\n");//daca nu este niciun fisier
        while(token != NULL) {// cat timp exista fisier/ fisiere (e while pt ca pot fi mai multe fisiere)
            strcpy(token_local, token);// token_local = fisier actual
            if(token_local[strlen(token_local)-1] == '\n') token_local[strlen(token_local)-1] = '\0'; //se verifica daca ultimul caracter al sirului este newline si daca da il inlocuieste cu \0 terminatorul de sir
            read_cat_data_bc(token_local, 1, chars); // apelam functia pt optiunea -c(1)
            token = strtok(NULL, " "); // se trece la urm fisier in caz ca exista 
        }
    }
    else { //caz in care e cat fara optiuni
        read_all_lines(token_local); // se afiseaza toate liniile de din fisier cu functia implementata in system.c

        token = strtok(NULL, " "); //se trece la urmatorul posibil fisier
        while(token != NULL) { // cat timp exista fisier/ fisiere (e while pt ca pot fi mai multe fisiere)
            strcpy(token_local, token); //se copiatza fisierul actual in token_local 
            if(token_local[strlen(token_local)-1] == '\n') token_local[strlen(token_local)-1] = '\0'; //se verifica daca ultimul caracter al sirului este newline si daca da il inlocuieste cu \0 terminatorul de sir
            read_all_lines(token_local); // se afiseaza toate liniile cu functia implementata in system.c
            token = strtok(NULL, " "); // se trece la urm posibil fisier
        }
    }

    return 0;
}