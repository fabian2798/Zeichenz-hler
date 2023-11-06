#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <ctype.h>
void usage()
{
    printf("Usage : ./myprog [-c|-w|-l] [-i] inputfile\n");
    exit(EXIT_FAILURE);
}
int main(int argc, char *argv[])
{
    int opt, fd, stat;
    int imwort = 0;
    int cflag, wflag, lflag = 0;
    int zeichen = 0;
    int word = 0;
    int line = 0;
    FILE *fp = stdin; //falls kein filename übergeben wird aus consolen eingabe lesen
    char z;
    char lastz;
    char filename[255] = ""; //feld als leer definiert
    struct stat buf;         //vorgefertigte struct in <sys/types.h>

    while ((opt = getopt(argc, argv, "cwli:")) != -1)
    { //getopt bietet optionen als bedingung, -1 stoppt getopt

        switch (opt)
        {
        case 'c':      // c option in getopt
            cflag = 1; //gibt signal aus wenn c option gewählt wird
            break;
        case 'w':      // w option in getopt
            wflag = 1; //gibt signal aus wenn w option gewählt wird
            break;
        case 'l':      // l option in getopt
            lflag = 1; //gibt signal aus wenn l option gewählt wird
            break;
        case 'i': // i option in getopt
            strncpy(filename, optarg, 255);
            break;
        case '?': //unbekannte optionen
            usage();
            break;
        default:
            usage();
            break;
        }
    }

    if (optind < argc)
    {                                         // wenn mehr argumente angegeben sind (argc) als argumente benötigt werden(optind)
        strncpy(filename, argv[optind], 255); //strncpy kopiert argc[optind] in filename
    }

    if (strlen(filename) != 0)
    {                                  //strlen=länge des strings(int wert), führt aus wenn string nicht leer ist
        fd = open(filename, O_RDONLY); //Filedscriptor auf FILE fp, O_RDONLY lesen only,O_WRONLY schreiben only, O_RDWR lesen und schreiben
        stat = fstat(fd, &buf);        //überprüfung FILE übergibt 0 bei erfolg, 1 bei fail
        if (stat != 0)
        { // file descriptor failt wenn das file nicht gefunden wird
            printf("Error Filedesciptor\n");
            exit(EXIT_FAILURE);
        }
        if (stat == 0)
        { // file descriptor erfolgreich wenn das file gefunden wird
            fp = fopen(filename, "r");
        }
    }

    if (cflag == 1 || wflag == 1 || lflag == 1)
    {
        while ((z = fgetc(fp)) && !feof(fp))
        { //zählt solange bis das eonf of file nicht erreicht ist
            zeichen++;
            if (z == '\n')
            { //wenn zeilenumbruch zähle zeile
                line++;
            }
            if (isspace(z))
            {                    //überprüft ob z == whitespace
                if (imwort == 1) //wir befinden uns im wort
                {
                    word++;
                    imwort = 0;
                }
            }
            else //imwort == 0 nicht im wort somit ein whitespace
            {
                imwort = 1; //nach whitespace kommt ein wort
            }
            lastz = z; //letztes zeichen wird zwischnegespeichert(zeichen vor EOF)
        }
        if (isalpha(lastz)) //lastz == buchstabe (imwort == 1)
        {
            word++;
            line++; //erkennt eine zeile ohne \n mit wörtern somit line+1
        }
        close(fd);  //schließt file descriptor
        fclose(fp); //schließt file
    }

    if (cflag == 1)
    {
        printf("Anzahl der Zeichen: %d\n", zeichen);
    }
    if (wflag == 1)
    {
        printf("Anzahl der Woerter: %d\n", word);
    }
    if (lflag == 1)
    {
        printf("Anzahl der Zeilen: %d\n", line);
    }
    return 0;
}