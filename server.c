#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define PORT 2024
extern int errno;
int CONECTAT = 0;
int nr_articole = 0;
int main()
{

    struct sockaddr_in server; // structura folosita de server
    struct sockaddr_in from;
    char msg[1000];          //mesajul primit de la client
    char msgrasp[100] = " "; //mesaj de raspuns pentru client
    char cos_de_cumparaturi[1000] = " ";
    int sd; //descriptorul de socket

    /* crearea unui socket */
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("[server]Eroare la socket().\n");
        return errno;
    }

    /* pregatirea structurilor de date */
    bzero(&server, sizeof(server));
    bzero(&from, sizeof(from));

    /* umplem structura folosita de server */
    /* stabilirea familiei de socket-uri */
    server.sin_family = AF_INET;
    /* acceptam orice adresa */
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    //  printf("ip adress is : %d\n",inet_ntoa(server.sin_addr));
    /* utilizam un port utilizator */
    server.sin_port = htons(PORT);

    /* atasam socketul */
    if (bind(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
    {
        perror("[server]Eroare la bind().\n");
        return errno;
    }

    /* punem serverul sa asculte daca vin clienti sa se conecteze */
    if (listen(sd, 5) == -1)
    {
        perror("[server]Eroare la listen().\n");
        return errno;
    }

    while (1)
    {
        int client;
        int length = sizeof(from);

        printf("[server]Asteptam la portul %d...\n", PORT);
        fflush(stdout);

        /* acceptam un client (stare blocanta pana la realizarea conexiunii) */

        client = accept(sd, (struct sockaddr *)&from, &length);

        /* eroare la acceptarea conexiunii de la un client */
        if (client < 0)
        {
            perror("[server]Eroare la accept().\n");
            continue;
        }

        int forkpid = fork();
        if (forkpid == 0)
        {
            bzero(msg, sizeof(msg));
            fflush(stdout);
            strcat(msg, "Bine ai venit! \n Optiuni dispoibile: \n [1]Conectare \n [0]Exit");

            while (1)
            {

                if (write(client, msg, sizeof(msg)) <= 0)
                {
                    perror("[server]Eroare la write() catre client.\n");
                    continue; /* continuam sa ascultam */
                }
                else
                    printf("[server]Mesajul a fost trasmis cu succes.\n");

                if (read(client, msg, sizeof(msg)) <= 0)
                {
                    perror("[server]Eroare la read() de la client.\n");
                    close(client);
                    continue;
                }

                printf("[server]Mesajul a fost receptionat...%s\n", msg);

                if (strstr(msg, "0") != 0 && strlen(msg) == 2)
                {
                    close(client);
                    return 0;
                }
                else
                {
                    if (CONECTAT == 0)
                    {
                        FILE *fp;
                        char *line = NULL;
                        size_t len = 0;
                        ssize_t r;
                        int corect = 0;
                        fp = fopen("users.txt", "r");

                        while ((r = getline(&line, &len, fp)) != -1)
                        {
                            printf("%s", line);

                            if (strstr(msg, line) != 0 && strlen(msg) == strlen(line) + 1)
                            {

                                corect = 1;
                                break;
                            }
                        }
                        fclose(fp);
                        printf("%d", corect);
                        if (corect == 1)
                        {
                            bzero(msg, sizeof(msg));
                            strcat(msg, "Conectare reusita! \nOptiuni disponibile: \n [2]Afisarea produselor \n [3]Plasare comanda \n [4]Vizualizare cos de cumparaturi \n [5]Salvare cos de cumparaturi \n [0]Exit");
                            CONECTAT = 1;
                        }
                        else
                        {
                            bzero(msg, sizeof(msg));
                            strcat(msg, "Conectare nereusita! Incearca din nou!\n Optiuni dispoibile: \n [1]Conectare");
                        }
                    }
                }

                if (strstr(msg, "2") != 0 && strlen(msg) == 2 && CONECTAT == 1)
                {
                    bzero(msg, sizeof(msg));
                    strcat(msg, "Alege o categorie din cele disponibile: \n Carti \n Bijuterii \n Electrocasnice \n Gaming");
                }

                if (strstr(msg, "Carti") != 0 && strlen(msg) == 6 && CONECTAT == 1)
                {
                    FILE *fp;
                    char *line = NULL;
                    size_t len = 0;
                    ssize_t r;
                    bzero(msg, sizeof(msg));

                    fp = fopen("carti.txt", "r");

                    while ((r = getline(&line, &len, fp)) != -1)
                    {

                        strcat(msg, line);
                    }
                    fclose(fp);
                    strcat(msg, "\n Optiuni disponibile: \n [2]Vizualizare alta categorie \n [3]Plasare comanda \n [4]Vizualizare cos de cumparaturi \n [5]Salvare cos de cumparaturi \n [6]Adaugare element din aceasta categorie in cos \n [0]Exit");
                }

                if (strstr(msg, "Bijuterii") != 0 && strlen(msg) == 10 && CONECTAT == 1)
                {
                    FILE *fp;
                    char *line = NULL;
                    size_t len = 0;
                    ssize_t r;
                    bzero(msg, sizeof(msg));

                    fp = fopen("bijuterii.txt", "r");

                    while ((r = getline(&line, &len, fp)) != -1)
                    {

                        strcat(msg, line);
                    }
                    fclose(fp);
                    strcat(msg, "\n Optiuni disponibile: \n [2]Vizualizare alta categorie \n [3]Plasare comanda \n [4]Vizualizare cos de cumparaturi \n [5]Salvare cos de cumparaturi \n [6]Adaugare element din aceasta categorie in cos \n [0]Exit");
                }

                if (strstr(msg, "Gaming") != 0 && strlen(msg) == 7 && CONECTAT == 1)
                {
                    FILE *fp;
                    char *line = NULL;
                    size_t len = 0;
                    ssize_t r;
                    bzero(msg, sizeof(msg));

                    fp = fopen("gaming.txt", "r");

                    while ((r = getline(&line, &len, fp)) != -1)
                    {
                        strcat(msg, line);
                    }
                    fclose(fp);
                    strcat(msg, "\n Optiuni disponibile: \n [2]Vizualizare alta categorie \n [3]Plasare comanda \n [4]Vizualizare cos de cumparaturi \n [5]Salvare cos de cumparaturi \n [6]Adaugare element din aceasta categorie in cos \n [0]Exit");
                }

                if (strstr(msg, "Electrocasnice") != 0 && strlen(msg) == 15 && CONECTAT == 1)
                {
                    FILE *fp;
                    char *line = NULL;
                    size_t len = 0;
                    ssize_t r;
                    bzero(msg, sizeof(msg));

                    fp = fopen("electocasnice.txt", "r");

                    while ((r = getline(&line, &len, fp)) != -1)
                    {

                        strcat(msg, line);
                    }
                    fclose(fp);
                    strcat(msg, "\n Optiuni disponibile: \n [2]Vizualizare alta categorie \n [3]Plasare comanda \n [4]Vizualizare cos de cumparaturi \n [5]Salvare cos de cumparaturi \n [6]Adaugare element din aceasta categorie in cos \n [0]Exit");
                }

                if (msg[0] == 'b' && strstr(msg, "b") != 0 && strlen(msg) == 3 && CONECTAT == 1)
                {
                    FILE *fp;
                    char *line = NULL;
                    size_t len = 0;
                    ssize_t r;
                    int corect = 0;
                    fp = fopen("bijuterii.txt", "r");

                    while ((r = getline(&line, &len, fp)) != -1)
                    {
                        corect = 0;

                        for (int i = 0; i < strlen(msg); i++)
                        {
                            if (msg[i] == line[i])
                            {
                                corect++;
                            }
                            else
                            {
                                break;
                            }
                        }

                        if (strlen(msg) - 1 == corect)
                        {
                            strcat(cos_de_cumparaturi, line);
                            strcat(cos_de_cumparaturi, "\n");
                            nr_articole++;
                        }
                    }
                    fclose(fp);

                    if (corect > 0)
                    {
                        bzero(msg, sizeof(msg));
                        strcat(msg, "Articolul a fost adaugat in cos! \n Optiuni disponibile: \n [2]Vizualizare alta categorie \n [3]Plasare comanda \n [4]Vizualizare cos de cumparaturi \n [5]Salvare cos de cumparaturi \n [6]Adaugare element din aceasta categorie in cos \n [0]Exit");
                    }
                    else
                    {
                        bzero(msg, sizeof(msg));
                        strcat(msg, "Articolul nu exista! \n Optiuni disponibile: \n [2]Vizualizare alta categorie \n [3]Plasare comanda \n [4]Vizualizare cos de cumparaturi \n [5]Salvare cos de cumparaturi \n [6]Adaugare element din aceasta categorie in cos \n [0]Exit");
                    }
                }

                if (msg[0] == 'c' && strstr(msg, "c") != 0 && strlen(msg) == 3 && CONECTAT == 1)
                {
                    FILE *fp;
                    char *line = NULL;
                    size_t len = 0;
                    ssize_t r;
                    int corect = 0;
                    fp = fopen("carti.txt", "r");

                    while ((r = getline(&line, &len, fp)) != -1)
                    {
                        corect = 0;

                        for (int i = 0; i < strlen(msg); i++)
                        {
                            if (msg[i] == line[i])
                            {
                                corect++;
                            }
                            else
                            {

                                break;
                            }
                        }

                        if (strlen(msg) - 1 == corect)
                        {
                            strcat(cos_de_cumparaturi, line);
                            nr_articole++;
                        }
                    }
                    fclose(fp);

                    if (corect > 0)
                    {
                        bzero(msg, sizeof(msg));
                        strcat(msg, "Articolul a fost adaugat in cos! \n Optiuni disponibile: \n [2]Vizualizare alta categorie \n [3]Plasare comanda \n [4]Vizualizare cos de cumparaturi \n [5]Salvare cos de cumparaturi \n [6]Adaugare element din aceasta categorie in cos \n [0]Exit");
                        printf("%s", cos_de_cumparaturi);
                    }
                    else if (corect == 0)
                    {
                        bzero(msg, sizeof(msg));
                        strcat(msg, "Articolul nu exista! \n Optiuni disponibile: \n [2]Vizualizare alta categorie \n [3]Plasare comanda \n [4]Vizualizare cos de cumparaturi \n [5]Salvare cos de cumparaturi \n [6]Adaugare element din aceasta categorie in cos \n [0]Exit");
                    }
                }
                if (msg[0] == 'e' && strstr(msg, "e") != 0 && strlen(msg) == 3 && CONECTAT == 1)
                {
                    FILE *fp;
                    char *line = NULL;
                    size_t len = 0;
                    ssize_t r;
                    int corect = 0;
                    fp = fopen("electocasnice.txt", "r");

                    while ((r = getline(&line, &len, fp)) != -1)
                    {
                        corect = 0;

                        for (int i = 0; i < strlen(msg); i++)
                        {
                            if (msg[i] == line[i])
                            {
                                corect++;
                            }
                            else
                            {
                                //corect = 0;
                                break;
                            }
                        }

                        if (strlen(msg) - 1 == corect)
                        {
                            strcat(cos_de_cumparaturi, line);
                            nr_articole++;
                        }
                    }
                    fclose(fp);

                    if (corect > 0)
                    {
                        bzero(msg, sizeof(msg));
                        strcat(msg, "Articolul a fost adaugat in cos! \n Optiuni disponibile: \n [2]Vizualizare alta categorie \n [3]Plasare comanda \n [4]Vizualizare cos de cumparaturi \n [5]Salvare cos de cumparaturi \n [6]Adaugare element din aceasta categorie in cos \n [0]Exit");
                        printf("%s", cos_de_cumparaturi);
                    }
                    else if (corect == 0)
                    {
                        bzero(msg, sizeof(msg));
                        strcat(msg, "Articolul nu exista! \n Optiuni disponibile: \n [2]Vizualizare alta categorie \n [3]Plasare comanda \n [4]Vizualizare cos de cumparaturi \n [5]Salvare cos de cumparaturi \n [6]Adaugare element din aceasta categorie in cos \n [0]Exit");
                    }
                }

                if (msg[0] == 'g' && strstr(msg, "g") != 0 && strlen(msg) == 3 && CONECTAT == 1)
                {
                    FILE *fp;
                    char *line = NULL;
                    size_t len = 0;
                    ssize_t r;
                    int corect = 0;
                    fp = fopen("gaming.txt", "r");

                    while ((r = getline(&line, &len, fp)) != -1)
                    {
                        corect = 0;

                        for (int i = 0; i < strlen(msg); i++)
                        {
                            if (msg[i] == line[i])
                            {
                                corect++;
                            }
                            else
                            {

                                break;
                            }
                        }

                        if (strlen(msg) - 1 == corect)
                        {
                            strcat(cos_de_cumparaturi, line);
                            nr_articole++;
                        }
                    }
                    fclose(fp);

                    if (corect > 0)
                    {
                        bzero(msg, sizeof(msg));
                        strcat(msg, "Articolul a fost adaugat in cos! \n Optiuni disponibile: \n [2]Vizualizare alta categorie \n [3]Plasare comanda \n [4]Vizualizare cos de cumparaturi \n [5]Salvare cos de cumparaturi \n [6]Adaugare element din aceasta categorie in cos \n [0]Exit");
                        printf("%s", cos_de_cumparaturi);
                    }
                    else if (corect == 0)
                    {
                        bzero(msg, sizeof(msg));
                        strcat(msg, "Articolul nu exista! \n Optiuni disponibile: \n [2]Vizualizare alta categorie \n [3]Plasare comanda \n [4]Vizualizare cos de cumparaturi \n [5]Salvare cos de cumparaturi \n [6]Adaugare element din aceasta categorie in cos \n [0]Exit");
                    }
                }

                if (strstr(msg, "4") != 0 && strlen(msg) == 2 && CONECTAT == 1)
                {
                    if (nr_articole > 0)
                    {
                        bzero(msg, sizeof(msg));
                        strcat(msg, "Cosul de cumparturi contine: \n");
                        strcat(msg, cos_de_cumparaturi);
                        strcat(msg, "\n Optiuni disponibile: \n [2]Vizualizare alta categorie \n [3]Plasare comanda \n [4]Vizualizare cos de cumparaturi \n [5]Salvare cos de cumparaturi \n [6]Adaugare element din aceasta categorie in cos \n [0]Exit");
                    }
                    else
                    {
                        bzero(msg, sizeof(msg));
                        strcat(msg, "Cosul de cumparturi este gol! \n Optiuni disponibile: \n [2]Afisarea produselor \n [3]Plasare comanda \n [4]Vizualizare cos de cumparaturi \n [5]Salvare cos de cumparaturi \n [0]Exit");
                    }
                }
                if (strstr(msg, "5") != 0 && strlen(msg) == 2 && CONECTAT == 1)
                {
                    if (nr_articole > 0)
                    {
                        // char nume[100]='';
                        // strcat(nume,"salvare_");
                        // strcat(nume,)
                        FILE *cos = fopen("salvarecos.txt", "w");
                        fputs(cos_de_cumparaturi, cos);
                        fclose(cos);
                        bzero(msg, sizeof(msg));
                        strcat(msg, "Cosul de cumparaturi a fost salvat cu succes!");
                        strcat(msg, "\n Optiuni disponibile: \n [2]Vizualizare alta categorie \n [3]Plasare comanda \n [4]Vizualizare cos de cumparaturi \n [5]Salvare cos de cumparaturi \n [6]Adaugare element din aceasta categorie in cos \n [0]Exit");
                    }
                    else
                    {
                        bzero(msg, sizeof(msg));
                        strcat(msg, "Cosul de cumparturi este gol! \n Optiuni disponibile: \n [2]Afisarea produselor \n [3]Plasare comanda \n [4]Vizualizare cos de cumparaturi \n [5]Salvare cos de cumparaturi \n [0]Exit");
                    }
                }
                if (strstr(msg, "3") != 0 && strlen(msg) == 2 && CONECTAT == 1)
                {
                    if (nr_articole > 0)
                    {
                        bzero(msg, sizeof(msg));
                        strcat(msg, "Comanda a fost plasata!");
                        strcat(msg, "\n Optiuni disponibile: \n [2]Vizualizare alta categorie \n [3]Plasare comanda \n [4]Vizualizare cos de cumparaturi \n [5]Salvare cos de cumparaturi \n [6]Adaugare element din aceasta categorie in cos \n [0]Exit");
                    }
                    else
                    {
                        bzero(msg, sizeof(msg));
                        strcat(msg, "Cosul de cumparturi este gol si comanda nu poate fi plasata! \n Optiuni disponibile: \n [2]Afisarea produselor \n [3]Plasare comanda \n [4]Vizualizare cos de cumparaturi \n [5]Salvare cos de cumparaturi \n [0]Exit");
                    }
                }
                if (strstr(msg, "0") == 0 && strstr(msg, "1") == 0 && strstr(msg, "2") == 0 && strstr(msg, "3") == 0 && strstr(msg, "4") == 0 && strstr(msg, "5") == 0 && strstr(msg, "6") == 0 && strlen(msg)==2)
                {
                    bzero(msg, sizeof(msg));
                    strcat(msg, "Optiunea nu exista. \nIntrodu o alta optiune ");
                }

                // close(client);
                // return 0;
            }
        }
        else
        {
            close(client);
        }
    }
}