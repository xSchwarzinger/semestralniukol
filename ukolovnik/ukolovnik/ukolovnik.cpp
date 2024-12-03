#include "ukolovnik.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

int main() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char dnesni_datum[11];
    char aktualni_datum[11];
    int manualni_datum = 0;
    int aktualni_rezim = 1;
    const char* nazev_souboru = "ukoly.txt";

    snprintf(aktualni_datum, sizeof(aktualni_datum), "%02d-%02d-%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    strcpy(dnesni_datum, aktualni_datum);

    while (1) {
        system("cls");

        if (aktualni_rezim == 1) {
            printf("Zobrazuji v dennim rezimu:\n");
            nacti_ukoly(nazev_souboru, dnesni_datum);
        }
        else if (aktualni_rezim == 2) {
            printf("Zobrazuji v tydennim rezimu:\n");
            zobrazit_tydenni_rezim(nazev_souboru, dnesni_datum);
        }

        printf("\nPouzijte sipky vlevo/vpravo pro zmenu dne, ESC pro dalsi moznosti.\n");

        int ch = _getch();
        if (ch == 0 || ch == 224) {
            ch = _getch();
            if (ch == 75) {
                zmen_den(&tm, -1);
                if (!manualni_datum) {
                    snprintf(dnesni_datum, sizeof(dnesni_datum), "%02d-%02d-%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
                }
            }
            else if (ch == 77) {
                zmen_den(&tm, 1);
                if (!manualni_datum) {
                    snprintf(dnesni_datum, sizeof(dnesni_datum), "%02d-%02d-%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
                }
            }
        }
        else if (ch == 27) {
            printf("\n1 - Pridat ulohu\n2 - Upravit ulohu\n3 - Oznacit ulohu hotovou/nehotovou\n4 - Zmenit datum ulohy\n5 - Prepnout na zadane datum\n6 - Navrat k aktualnimu datu\n7 - Zobrazit v dennim rezimu\n8 - Zobrazit v tydennim rezimu\n9 - Vymazat ulohu\nVyberte moznost: ");
            int moznost;
            scanf(" %d", &moznost);
            if (moznost == 1) {
                pridat_ukol(nazev_souboru, dnesni_datum);
            }
            else if (moznost == 2) {
                upravit_ukol(nazev_souboru, dnesni_datum);
            }
            else if (moznost == 3) {
                oznacit_ukol(nazev_souboru, dnesni_datum);
            }
            else if (moznost == 4) {
                zmenit_datum_ukolu(nazev_souboru, dnesni_datum);
            }
            else if (moznost == 5) {
                printf("Zadejte datum (dd-mm-yyyy): ");
                char nove_datum[11];
                scanf(" %10s", nove_datum);
                strcpy(dnesni_datum, nove_datum);
                manualni_datum = 1;
            }
            else if (moznost == 6) {
                strcpy(dnesni_datum, aktualni_datum);
                manualni_datum = 0;
            }
            else if (moznost == 7) {
                aktualni_rezim = 1;
            }
            else if (moznost == 8) {
                aktualni_rezim = 2;
            }
            else if (moznost == 9) {
                vymazat_ukol(nazev_souboru, dnesni_datum);
            }
        }
    }

    return 0;
}
