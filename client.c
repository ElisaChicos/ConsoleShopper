#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

/* codul de eroare returnat de anumite apeluri */
extern int errno;

/* portul de conectare la server*/
int port;
int CONECTAT = 0;
int gresit = 0;

int main(int argc, char *argv[])
{
    int sd;                    // descriptorul de socket
    struct sockaddr_in server; // structura folosita pentru conectare
    char msg[1000];            // mesajul trimis
    char msgfromserver[1000];

    /* exista toate argumentele in linia de comanda? */
    if (argc != 3)
    {
        printf("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
        return -1;
    }

    /* stabilim portul */
    port = atoi(argv[2]);

    /* cream socketul */
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Eroare la socket().\n");
        return errno;
    }

    /* umplem structura folosita pentru realizarea conexiunii cu serverul */
    /* familia socket-ului */
    server.sin_family = AF_INET;
    /* adresa IP a serverului */
    server.sin_addr.s_addr = inet_addr(argv[1]);
    /* portul de conectare */
    server.sin_port = htons(port);

    /* ne conectam la server */
    if (connect(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
    {
        perror("[client]Eroare la connect().\n");
        return errno;
    }

    while (1)
    {
        gresit = 0;
        if (read(sd, msgfromserver, sizeof(msg)) < 0)
        {
            perror("[client]Eroare la read() de la server.\n");
            return errno;
        }

        printf("%s\n", msgfromserver);
        bzero(msg, sizeof(msg));
        printf("Optiune: ");
        fflush(stdout);
        read(0, msg, sizeof(msg));

        if ((strstr(msg, "1") != 0 && strlen(msg) == 2) || strstr(msgfromserver, "Conectare nereusita! Incearca din nou!") != 0)
        {
            bzero(msg, sizeof(msg));
            printf("Introdu numele de utilizator: ");
            fflush(stdout);
            read(0, msg, sizeof(msg));
        }
        else
        {
            gresit++;
        }

        if (strstr(msg, "6") != 0 && strlen(msg) == 2)
        {
            bzero(msg, sizeof(msg));
            printf("Introdu id-ul produsului pe care il doresti: ");
            fflush(stdout);
            read(0, msg, sizeof(msg));

            if (strlen(msg) > 3)
            {
                while (strlen(msg) > 3)
                {
                    bzero(msg, sizeof(msg));
                    printf("Produsul nu exista! Introdu un al id: ");
                    fflush(stdout);
                    read(0, msg, sizeof(msg));
                }
            }
            else
            {
                while (msg[0] != 'c' && msg[0] != 'b' && msg[0] != 'e' && msg[0] != 'g' && strlen(msg) > 3)
                {
                    bzero(msg, sizeof(msg));
                    printf("Produsul nu exista! Introdu un al id: ");
                    fflush(stdout);
                    read(0, msg, sizeof(msg));
                }
            }
        }
        else
        {
            gresit++;
        }

        if (strstr(msgfromserver, "Conectare reusita!") != 0)
        {
            CONECTAT = 1;
        }

        if (strstr(msg, "0") != 0 && strlen(msg) == 2)
        {
            if (write(sd, msg, sizeof(msg)) <= 0)
            {
                perror("[client]Eroare la write() spre server.\n");
                return errno;
            }
            else
            {
                close(sd);
                return 0;
            }
        }
        else
        {
            gresit++;
        }

        if (strstr(msg, "2") != 0 && strlen(msg) == 2)
        {

            bzero(msg, sizeof(msg));
            printf("Alege o categorie din cele disponibile: \n Carti \n Bijuterii \n Electrocasnice \n Gaming \nIntrodu categoria dorita: ");
            fflush(stdout);
            read(0, msg, sizeof(msg));

            while (strstr(msg, "Carti") == 0 && strstr(msg, "Bijuterii") == 0 && strstr(msg, "Electrocasnice") == 0 &&
                   strstr(msg, "Gaming") == 0)
            {
                bzero(msg, sizeof(msg));
                printf("Categoria nu exista. \nIntrodu o alta categorie: ");
                fflush(stdout);
                read(0, msg, sizeof(msg));
            }
        }
        else
        {
            gresit++;
        }

        // if (strstr(msg, "0") == 0 && strstr(msg, "1") == 0 && strstr(msg, "2") == 0 && strstr(msg, "3") == 0 && strstr(msg, "4") == 0 && strstr(msg, "5") == 0 && strstr(msg, "6") == 0)
        // {
        //     bzero(msg, sizeof(msg));
        //     printf("Optiunea nu exista. \nIntrodu o alta optiune: ");
        //     fflush(stdout);
        //     read(0, msg, sizeof(msg));
        // }


        if (write(sd, msg, sizeof(msg)) <= 0)
        {
            perror("[client]Eroare la write() spre server.\n");
            return errno;
        }
    }
}
