
int started = 0;
//functie pentru logare in cont
int login(char *filename, char * user, char * pass) {
    int fd = open(filename, O_RDONLY); // deschidem fisierul
    int login_status = 0; //se initializeaza statusul pe 0 initial
    char accdata[256]; //initializare sir de caractere 
    accdata[0] = '\0'; //se adauga caracterul nul
    strcat(accdata, user); //se adauga userul la accdata
    strcat(accdata, " "); //se adauga un spatiu
    strcat(accdata, pass); // se adauga parola
    // la final accdata va fi de forma "user parola"
    if (fd == -1) { // verificam daca fisierul a fost deschis
        printf("Eroare: File can't be opened.");
        return 0;
    }

    char buffer[BUFFER_SIZE]; //buffer pt citirea litera cu litera a liniilor
    char line[BUFFER_SIZE];// buffer pt stocarea liniilor citite din fisier
    int bytesRead; // nr de octeti cititi intr-o operatiune de citire
    int lineIndex = 0; //indicele curent al sirului line  

    while ((bytesRead = read(fd, buffer, BUFFER_SIZE)) > 0) { //se citeste din fisier cat timp mai sunt octeti de citit adica >0
                                                                 //la fiecare iteratie se citeste un bloc de date de dimensiune BUFFER_SIZE si se stocheaza in buffer
        for (int i = 0; i < bytesRead; i++) {  //se parcurge fiecare caracter din buffer ul citit
            if (buffer[i] == '\n' || buffer[i] == '\0') { //daca se intalneste \n sau \0 se presupune o linie citita
                line[lineIndex] = '\0'; // adauga terminatorul de sir
                

                if(strcmp(accdata, line) == 0) { // verificam daca linia line area acesi valoare cu accdata , daca da e login status 1 
                    login_status = 1;
                }
                lineIndex = 0; // resetare index pe 0
            } else { // daca nu s-a ajuns la finalul liniei 
                line[lineIndex++] = buffer[i]; // se adauga in line caracterul urmator 
                if (lineIndex >= BUFFER_SIZE - 1) { // in caz de se atinge bufferul resetam indexul
                    lineIndex = 0;
                }
            }
        }
    }
    close(fd); // inchidem fisierul
    if (bytesRead == -1) { // fail safe in caz ca a esuat citirea fisieruluiS
        return 0;
    }

    return login_status; // returnam status-ul 1 daca a fost gasit contul, 0 altfel
}