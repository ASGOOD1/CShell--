
int read_last(char * filename, int from_line) { //functie citire / afisare ultimele n linii
    int fd = open(filename, O_RDONLY); //se deschide fisierul 
    
    if (fd == -1) {
        printf("Error: %s doesn't exist.\n", filename);
        return 1; //eroare in caz ca fisierul nu a putut fi deschis
    }

    int readlines = 0; //contor pt nr linii citite 
    char buffer[BUFFER_SIZE]; //buffer pt citirea caracter cu caracter a liniilor
    char line[BUFFER_SIZE]; // buffer pt stocarea liniilor citite din fisier
    int bytesRead;// nr de octeti cititi intr-o operatiune de citire
    int lineIndex = 0;//indicele curent al sirului line  

    while ((bytesRead = read(fd, buffer, BUFFER_SIZE)) > 0) {//se citeste din fisier cat timp mai sunt octeti de citit adica >0
                                                                 //la fiecare iteratie se citeste un bloc de date de dimensiune BUFFER_SIZE si se stocheaza in buffer
          for (int i = 0; i < bytesRead; i++) {  //se parcurge fiecare caracter din buffer ul citit
            if (buffer[i] == '\n' || buffer[i] == '\0') { // daca e final de linie sau fisier
                line[lineIndex] = '\0';// se pune terminatorul de sir 
                readlines++; // incrementem numarul de linii cititite
                if(readlines >= from_line) printf("%s\n", line); // printam daca numarul de linii este mai mare decat linia de la care cautam
                lineIndex = 0; // se trece indexul pe 0 
            } else { // daca nu e final de linie sau fiser 
                line[lineIndex++] = buffer[i]; // se salveaza urm caracter in line 
                if (lineIndex >= BUFFER_SIZE - 1) {  // in caz de se atinge bufferul resetam indexul
                    lineIndex = 0;
                }
            }
        }
    }
    close(fd); // se inchide fisierul 
     
    if (bytesRead == -1) {  // fail safe in caz ca a esuat citirea fisierului
        printf("Error: %s doesn't exist.\n", filename);
        return 1;
    }
    return 0;
}


//functie care numara cate linii are un fisier
int linecount(char * filename) {
    int fd = open(filename, O_RDONLY); // se deschide fisierul
    if (fd == -1) {
        printf("Error: %s doesn't exist.\n", filename);
        return -1; // eroare la deschiderea fisierului 
    }
    int totallines = 0; // contor pt linii 
    char buffer[BUFFER_SIZE];  //buffer pt citirea caracter cu caracter a liniilor
    int bytesRead; // nr de octeti cititi intr-o operatiune de citire

    while ((bytesRead = read(fd, buffer, BUFFER_SIZE)) > 0) {//se citeste din fisier cat timp mai sunt octeti de citit adica >0
                                                                 //la fiecare iteratie se citeste un bloc de date de dimensiune BUFFER_SIZE si se stocheaza in buffer
        for (int i = 0; i < bytesRead; i++) { // se parcurge fiecare caracter din buffer ul citit 
            if (buffer[i] == '\n' || buffer[i] == '\0') { // daca e final de linie sau fisier se incrementeaza contorul pt linii 
                totallines++;
            } 
        }
    }
    close(fd); //se inchide fisierul
    if (bytesRead == -1) { // eroare in caz ca a esuat citirea fisierului 
        printf("Error: %s doesn't exist.\n", filename);
        return -1;
    }
    return totallines; // se returneaza contorul
}

int last(const char argsv[]) { // argsv sir de caractere ce contine intreaga comanda cu optiuni cu tot 
    char token_local[24];
    char args[256]; // initializare copie pt argsv
    strcpy(args, argsv); // copiere continut 
    char * token = strtok(args, " "); // token = last

    token = strtok(NULL, " "); // token = optiune / fisier
    strcpy(token_local, token); // se copiaza token in token_local
    if(token_local[strlen(token_local)-1] == '\n' ) token_local[strlen(token_local)-1] = '\0'; // se verifica daca ultimul caracter al sirului este newline si daca da il inlocuieste cu \0 terminatorul de sir

    if(strcmp(token_local, "-l") == 0) {  // daca optiunea este -l 
        token = strtok(NULL, " ");//token = numar
        if(token == NULL) return printf("last [-l(ines) lines] filename1 filename2 etc\nPrints last l lines\n"); // daca dupa -l nu se specifica un nr se afiseaza modul de utilizare a lui last
        strcpy(token_local, token); // se salveaza nr de linii de afisat in token_local
        if(token_local[strlen(token_local)-1] == '\n' ) token_local[strlen(token_local)-1] = '\0'; //se verifica daca ultimul caracter al sirului este newline si daca da il inlocuieste cu \0 terminatorul de sir
        for(int i = 0; i<strlen(token_local); i++) { // se parcurge toke_local si se asigura ca e format doar din numere
            if(isdgr(token_local[i]) == 0) {  // functia isdgr() e implementata in fisierul first.c
                printf("Error: Invalid number.");
                token = NULL;
                break;
            }   
        }
        int lines = atoi(token_local); // se transforma token_local din sir de caractere in nr
        token = strtok(NULL, " "); // token = primul fisier 
        while(token != NULL) { // while in caz ca sunt mai multe fisiere
            strcpy(token_local, token); // se copiaza numele fisierului in token_local
            if(token_local[strlen(token_local)-1] == '\n' ) token_local[strlen(token_local)-1] = '\0'; // se verifica daca ultimul caracter al sirului este newline si daca da il inlocuieste cu \0 terminatorul de sir
            int linecounter = linecount(token_local); // numaram cate linii are fisierul
            int starting_line = 0;
            if(linecounter - lines < 0) starting_line = 0; // daca are mai putine linii decat am dat noi, o sa afiseze tot fisierul
            else starting_line = linecounter - lines+1; // altfel incepe sa afiseze de la linia linecount-n+1 (Fisier de 8 linii, vrem sa afisam ultimele 2: 8-2+1 = 7 (afiseaza linia 7 si 8))
            read_last(token_local, starting_line); // se apeleaza functia read_last facuta mai sus
            token = strtok(NULL, " "); // se trece la urm fisier
        }

        return 1;
    }
    else {// caz in care nu avem optiuni si avem doat "last fisier1 fisier2...."
        
        while (token != NULL)
        {
            strcpy(token_local, token); // se copiaza numele fisierului in token_local 
            if(token_local[strlen(token_local)-1] == '\n') token_local[strlen(token_local)-1] = '\0'; // se verifica daca ultimul caracter al sirului este newline si daca da il inlocuieste cu \0 terminatorul de sir
            read_all_lines(token_local); // se afiseaza toate liniile din fisier cu functia facuta in fisierul system.c
            token = strtok(NULL, " "); // se trece la urm token adica urmatorul fisier daca exista
        }
        
    }
    return 0;
}