#include <sys/types.h> // tipuri de date utilizate pentru interactiunea cu sistemul de fisiere si alte subsisteme Unix de ex ino pt inode ul fisierelor 
#include <dirent.h>// pt functii de manipulare de directoare (opendir, readdir, closedir)
#include <sys/stat.h> //informatii despre fisiere  (permisiuni, tipuri, dimensiuni).
#include <pwd.h> // pt a obtine info despre utilizatorii sistemului
#include <grp.h> // pt a obtine info despre group
#include <time.h> // pt datele si orele fisierelor

//functie care listeaza fisierele dintr-un folder in forma ls
void list_files(const char *path, int show_hidden, int detailed, int show_inode) {
    // path: calea directorului pentru care se afiseaza continutul 
    // show_hidden: (1/0) pentru afisarea fisierelor ascunse - pt ls -a
    //detailed: (1/0) pentru afisarea detaliilor fiecarui fisier - pt ls -l
    //show_inode: (1/0) pentru afiSarea numărului inode al fiecărui fisier - pt ls -i 


    struct dirent *entry; //structura ce reprezinta info despre un fisier sau director dintr un director specificat d_name, d_ino 
    struct stat file_stat; // structura pt a stoca info detaliate ale unui fisier util la ls -l
    DIR *dir = opendir(path); // se deschide directorul
    if(strlen(path) == 0) return; // daca s-a dat un path vid se da return
    if (!dir) { // eroare in caz ca dir nu exista
        printf("Error: Can't open directory %s.\n", path);
        return;
    }
    printf("%s:\n", path); // se afiseaza numele dir inainte de a afisa continutul 
    while ((entry = readdir(dir)) != NULL) { // citire fisier cu fisisier din dir ( se opreste cand nu mai sunt fisiere in dir )
        if (!show_hidden && entry->d_name[0] == '.') // daca nu e optiunea -a se ignora fisierele ascunse si nu se afiseaza 
            continue;

        if (show_inode) // verificam / printam inode daca are optiunea -i
            printf("%lu ", entry->d_ino);

        if (detailed) { // daca e optiunea cu -l (cu detalii)
            char full_path[1024]; // initializare sir de caractere pt path
            snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name); //se construieste calea completa si se stocheaza in full_path

            if (stat(full_path, &file_stat) == -1) { // se cauta info cu stat , daca apelul stat esueaza se trece la urm fisier
                printf("Error retrieving file information");
                continue;
            }

            printf((S_ISDIR(file_stat.st_mode)) ? "d" : "-"); //daca e dir se printeaza d daca nu se printeaza -
            printf((file_stat.st_mode & S_IRUSR) ? "r" : "-");//verificare/printare permisier read used
            printf((file_stat.st_mode & S_IWUSR) ? "w" : "-");//write user
            printf((file_stat.st_mode & S_IXUSR) ? "x" : "-");//executie user
            printf((file_stat.st_mode & S_IRGRP) ? "r" : "-");//read group 
            printf((file_stat.st_mode & S_IWGRP) ? "w" : "-");//write group
            printf((file_stat.st_mode & S_IXGRP) ? "x" : "-");//executie group
            printf((file_stat.st_mode & S_IROTH) ? "r" : "-");//read other
            printf((file_stat.st_mode & S_IWOTH) ? "w" : "-");//write other
            printf((file_stat.st_mode & S_IXOTH) ? "x" : "-");//executie other
            printf(" ");

            printf("%ld ", file_stat.st_nlink); // printam numarul de link-uri

            struct passwd *pw = getpwuid(file_stat.st_uid); // se obtine numele user ului
            struct group *gr = getgrgid(file_stat.st_gid); // se obtine numele group ului
            printf("%s %s ", pw ? pw->pw_name : "unknown", gr ? gr->gr_name : "unknown"); //printam user ul si group ul
            printf("%5ld ", file_stat.st_size); //printam marimea

            char time[20]; // vector pt stocarea timpului 
            strftime(time, sizeof(time), "%b %d %H:%M", localtime(&file_stat.st_mtime)); //format data
            printf("%s ", time); // printam data
        }

        printf("%s\n", entry->d_name); //printam numele fisierului
    }

    closedir(dir);//inchidem directorul
}

void ls(const char argsv[]) {
    char token_local[24];// initializare token_local
    char args[256];//vector in care vom salva comanda+optiuni+dir (efectiv toata comanda)
    strcpy(args, argsv); //facem o copie
    char * token = strtok(args, " "); // token = ls

    token = strtok(NULL, " "); // token = optiune
    if(token == NULL) { // daca nu se ofera nicio optiune/nume de dir
        list_files(".", 0,0,0); // face ls pe dir curent cu optiunile 0 pt how_hidden 0 pt detailed 0 pt show_inode adica ls simplu fara -a sau -l sau -i adica doar se afiseaza fisierele din dir curent
        return ;
    }
    strcpy(token_local, token); //facem o copie a token-ului (optiunea)
    if(token_local[strlen(token_local)-1] == '\n' ) token_local[strlen(token_local)-1] = '\0'; // se verifica daca ultimul caracter al sirului este newline si daca da il inlocuieste cu \0 terminatorul de sir


    int show_hidden = 0, detailed = 0, show_inode = 0;
    //initializare optiuni pt -a sau -l sau -i pe 0

    
    if (strcmp(token_local, "-a") == 0) { //verificam daca argumentul este -a
        show_hidden = 1; //activam show_hidden
        token = strtok(NULL, " "); // trecem la urm token =dir
        if(token == NULL) list_files(".", show_hidden, detailed, show_inode); //daca urm token (dir) nu exista afisam ls -a pentru dir curent
        while(token != NULL) {// in cazul in care exista unul sau mai multe directoare 
            char file_name[32]; //pt salvarea dir
            strcpy(file_name, token); // se salveaza dir in file_name
            if(file_name[strlen(file_name)-1] == '\n') file_name[strlen(file_name)-1] = '\0';// se verifica daca ultimul caracter al sirului este newline si daca da il inlocuieste cu \0 terminatorul de sir
            list_files(file_name, show_hidden, detailed, show_inode); // se apeleaza functia list_files (ls -a file_name)
            token = strtok(NULL, " "); // se trece la urm dir
        }
    } else if (strcmp(token_local, "-l") == 0) { //verificam daca argumentul este -l
        detailed = 1;//activam detailed
        token = strtok(NULL, " ");// trecem la urm token =dir
        if(token == NULL) list_files(".", show_hidden, detailed, show_inode);//daca urm token (dir) nu exista afisam ls pentru dir curent
        while(token != NULL) {// in cazul in care exista unul sau mai multe directoare 
            char file_name[32];//pt salvarea dir
            strcpy(file_name, token); // se salveaza dir in file_name
            if(file_name[strlen(file_name)-1] == '\n') file_name[strlen(file_name)-1] = '\0';// se verifica daca ultimul caracter al sirului este newline si daca da il inlocuieste cu \0 terminatorul de sir
            list_files(file_name, show_hidden, detailed, show_inode); // se apeleaza functia list_files (ls -l file_name)
            token = strtok(NULL, " ");// se trece la urm dir
        }
    } else if (strcmp(token_local, "-i") == 0) { //verificam daca argumentul este -i
        show_inode = 1; //activam show_inode
        token = strtok(NULL, " "); //trecem la urm token=dir
        if(token == NULL) list_files(".", show_hidden, detailed, show_inode); //daca urm token (dir) nu exista afisam ls pentru dir curent
        while(token != NULL) {// in cazul in care exista unul sau mai multe directoare 
            char file_name[32]; //pt salvarea dir
            strcpy(file_name, token);  // se salveaza dir in file_name 
            if(file_name[strlen(file_name)-1] == '\n') file_name[strlen(file_name)-1] = '\0';// se verifica daca ultimul caracter al sirului este newline si daca da il inlocuieste cu \0 terminatorul de sir
            list_files(file_name, show_hidden, detailed, show_inode);// se apeleaza functia list_files (ls -i file_name)
            token = strtok(NULL, " ");// se trece la urm dir
        }
    } else if (strcmp(token_local, "-al") == 0 || strcmp(token_local, "-la") == 0) {//verificam daca argumentul este -al sau -la
        show_hidden = 1;//activam show_hidden
        detailed = 1;//activam detailed
        token = strtok(NULL, " ");//trecem la urm token=dir
        if(token == NULL) list_files(".", show_hidden, detailed, show_inode);//daca urm token (dir) nu exista afisam ls pentru dir curent
        while(token != NULL) {// in cazul in care exista unul sau mai multe directoare 
            char file_name[32];//pt salvarea dir
            strcpy(file_name, token); // se salveaza dir in file_name 
            if(file_name[strlen(file_name)-1] == '\n') file_name[strlen(file_name)-1] = '\0';// se verifica daca ultimul caracter al sirului este newline si daca da il inlocuieste cu \0 terminatorul de sir
            list_files(file_name, show_hidden, detailed, show_inode);// se apeleaza functia list_files (ls -al/ ls -la file_name)
            token = strtok(NULL, " ");// se trece la urm dir
        }
    } else if (strcmp(token_local, "-ai") == 0 || strcmp(token_local, "-ia") == 0) {//verificam daca argumentul este -ai sau -ia
        show_hidden = 1;//activam show_hidden
        show_inode = 1;// activam show_inode
        token = strtok(NULL, " ");//trecem la urm token=dir
        if(token == NULL) list_files(".", show_hidden, detailed, show_inode);//daca urm token (dir) nu exista afisam ls pentru dir curent
        while(token != NULL) {// in cazul in care exista unul sau mai multe directoare 
            char file_name[32];//pt salvarea dir
            strcpy(file_name, token);// se salveaza dir in file_name 
            if(file_name[strlen(file_name)-1] == '\n') file_name[strlen(file_name)-1] = '\0';// se verifica daca ultimul caracter al sirului este newline si daca da il inlocuieste cu \0 terminatorul de sir
            list_files(file_name, show_hidden, detailed, show_inode);//se apeleaza functia list_files (ls -ai/ ls -ia file_name)
            token = strtok(NULL, " ");// se trece la urm dir
        }
    } else if (strcmp(token_local, "-li") == 0 || strcmp(token_local, "-il") == 0) {//verificam daca argumentul este -li sau -il
        detailed = 1; //activam detailed
        show_inode = 1; //activam show_inode
        token = strtok(NULL, " ");//trecem la urm token=dir
        if(token == NULL) list_files(".", show_hidden, detailed, show_inode);//daca urm token (dir) nu exista afisam ls pentru dir curent
        while(token != NULL) {// in cazul in care exista unul sau mai multe directoare 
            char file_name[32];//pt salvarea dir
            strcpy(file_name, token);// se salveaza dir in file_name 
            if(file_name[strlen(file_name)-1] == '\n') file_name[strlen(file_name)-1] = '\0';// se verifica daca ultimul caracter al sirului este newline si daca da il inlocuieste cu \0 terminatorul de sir
            list_files(file_name, show_hidden, detailed, show_inode); //se apeleaza functia list_files (ls -li/ ls -il file_name)
            token = strtok(NULL, " ");// se trece la urm dir
        }
    } else if (strcmp(token_local, "-ali") == 0 || strcmp(token_local, "-ail") == 0 || strcmp(token_local, "-lai") == 0 || strcmp(token_local, "-lia") == 0 || strcmp(token_local, "-ial") == 0 || strcmp(token_local, "-ila") == 0) {
        //se verifica daca argumentul este o combinatie de -ali
        show_hidden = 1; //se activeaza show_hidden
        detailed = 1; //se activeaza detailed
        show_inode = 1; // se activeaza show_inode
        token = strtok(NULL, " ");//trecem la urm token=dir
        if(token == NULL) list_files(".", show_hidden, detailed, show_inode);//daca urm token (dir) nu exista afisam ls pentru dir curent
        while(token != NULL) {// in cazul in care exista unul sau mai multe directoare 
            char file_name[32];//pt salvarea dir
            strcpy(file_name, token);// se salveaza dir in file_name 
            if(file_name[strlen(file_name)-1] == '\n') file_name[strlen(file_name)-1] = '\0';// se verifica daca ultimul caracter al sirului este newline si daca da il inlocuieste cu \0 terminatorul de sir
            list_files(file_name, show_hidden, detailed, show_inode);//se apeleaza functia list_files (ls -ali/ sau orice combinatie de a cu l cu i)
            token = strtok(NULL, " ");// se trece la urm dir
        }
    } 
    else { // caz in care nu exista optiuni
        if(token == NULL) list_files(".", show_hidden, detailed, show_inode); // nu e specificat nici dir nici optiune se apeleaza pe dir curent
        else list_files(token_local, show_hidden, detailed, show_inode); // e specificat dir dar nu si optiunea 
        token = strtok(NULL, " "); // se trece la urm director daca exista
        while(token != NULL) { // daca exista urm dir / mai multe directoare
            strcpy(token_local, token); //se salveaza dir in token_local
            if(token_local[strlen(token_local)-1] == '\n') token_local[strlen(token_local)-1] = '\0';// se verifica daca ultimul caracter al sirului este newline si daca da il inlocuieste cu \0 terminatorul de sir
            list_files(token_local, show_hidden, detailed, show_inode); // se apeleaza functia pt ls pe dir 
            token = strtok(NULL, " "); // se trece la urm dir 
        }
        return;
    }

}