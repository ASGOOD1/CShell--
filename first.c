
int read_first(char * filename, int lines) { //functie citire primele n linii
    if(filename[strlen(filename)-1] == '\n') filename[strlen(filename)-1] = '\0';
    int fd = open(filename, O_RDONLY); // se deschide fisierul
    
    if (fd == -1) {
        printf("Error: %s doesn't exist.\n", filename); // eroare in caz ca a esuat deschiderea 
        return 1;
    }
    int readlines = 0; // contor pt linii
    char buffer[BUFFER_SIZE]; //buffer pt citirea caracter cu caracter a liniilor
    char line[BUFFER_SIZE]; // buffer pt stocarea liniilor citite din fisier
    int bytesRead;// nr de octeti cititi intr-o operatiune de citire
    int lineIndex = 0;//indicele curent al sirului line  


    while ((bytesRead = read(fd, buffer, BUFFER_SIZE)) > 0) {//se citeste din fisier cat timp mai sunt octeti de citit adica >0
                                                                 //la fiecare iteratie se citeste un bloc de date de dimensiune BUFFER_SIZE si se stocheaza in buffer
        for (int i = 0; i < bytesRead; i++) {//se parcurge fiecare caracter din buffer ul citit
            if (buffer[i] == '\n' || buffer[i] == '\0') {// daca e final de linie sau fisier
                line[lineIndex] = '\0';// se pune terminatorul de sir 
                printf("%s\n", line); // se printeaza linia
                readlines++;//incrementam numarul de linii parcurse si afisate 
                if(readlines == lines) break; // daca am ajuns la numarul cautat ne oprim
                lineIndex = 0;// punem din nou indicele pe 0 pt ca ne pregatim de urmatoarea linie
            } else { // daca nu e final de linie sau fisier
                line[lineIndex++] = buffer[i]; //se trece la caracterul urmator 
                if (lineIndex >= BUFFER_SIZE - 1) { // in caz de se atinge bufferul resetam indexul
                    lineIndex = 0;
                }
            }
        }
    }
    close(fd); // inchidem fisierul 
    if (bytesRead == -1) { // fail safe in caz ca a esuat citirea fisierului
        printf("Error: %s doesn't exist.\n", filename);
        return 1;
    }

    return 0;
} 

int isdgr(char c) { //functie de verificare daca un caracter este cifra
    if(c >= '0' && c <= '9') return 1;
    return 0;
}
int first(const char argsv[]) {
    char token_local[24]; //iitializam un token_local
    char args[256]; //initializam args sa copiem tot din argsv
    strcpy(args, argsv);// copierea
    char * token = strtok(args, " "); // token = first

    token = strtok(NULL, " "); // token = optiune
    strcpy(token_local, token); // se copiaza optiunea in token_local
    if(token_local[strlen(token_local)-1] == '\n' ) token_local[strlen(token_local)-1] = '\0';// se verifica daca ultimul caracter al sirului este newline si daca da il inlocuieste cu \0 terminatorul de sir

    if(strcmp(token_local, "-l") == 0) { // verificam daca token-ul este optiune
        token = strtok(NULL, " ");//token = numar
        if(token == NULL) return printf("first [-l(ines) lines] filename1 filename2 etc\nPrints first l lines\n");// daca dupa -l nu se specifica un nr se afiseaza modul de utilizare a lui first
        strcpy(token_local, token);// se salveaza nr de linii de afisat in token_local
        if(token_local[strlen(token_local)-1] == '\n' ) token_local[strlen(token_local)-1] = '\0'; // se verifica daca ultimul caracter al sirului este newline si daca da il inlocuieste cu \0 terminatorul de sir
        for(int i = 0; i<strlen(token_local); i++) { // se parcurge token_local
            if(isdgr(token_local[i]) == 0) { //verificam daca token-ul este format foar din cifre
                printf("Error: Invalid number.");// daca nu eroare
                token = NULL;
                break;
            }   
        }
        int lines = atoi(token_local); // transformam token_local in integer
        token = strtok(NULL, " "); // token = primul fisier
        while(token != NULL) { //while in caz ca sunt mai multe fisiere
            strcpy(token_local, token); //se copiaza fisierul actual in token_local
            if(token_local[strlen(token_local)-1] == '\n' ) token_local[strlen(token_local)-1] = '\0';// se verifica daca ultimul caracter al sirului este newline si daca da il inlocuieste cu \0 terminatorul de sir
            read_first(token_local, lines); //afisam primele lines linii cu functia read_first implementata mai sus
            token = strtok(NULL, " "); // token ia valoarea la urm fisier
        }

        return 1;
    }
    else { //caz in care nu se da optiunea -l si avem doar first fis1 fis2 etc
        
        while (token != NULL)//while in caz ca sunt mai multe fisiere
        {
            strcpy(token_local, token);//se copiaza fisierul actual in token_local
            if(token_local[strlen(token_local)-1] == '\n') token_local[strlen(token_local)-1] = '\0';//se verifica daca ultimul caracter al sirului este newline si daca da il inlocuieste cu \0 terminatorul de sir
            read_all_lines(token_local); // afisam toate liniile din fisiere cu functia read_all_lines implementata in system.c
            token = strtok(NULL, " "); // se trece la urmatorul fisier
        }
        
    }
    return 0;
}