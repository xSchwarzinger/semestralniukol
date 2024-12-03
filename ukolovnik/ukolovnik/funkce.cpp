#include "ukolovnik.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>

#define MAX_LINE_LENGTH 256

#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define RED "\033[0;31m"
#define STRIKETHROUGH "\033[9m"
#define RESET "\033[0m"

void nacti_ukoly(const char* nazev_souboru, const char* dnesni_datum) {
    FILE* soubor = fopen(nazev_souboru, "r");
    if (soubor == NULL) {
        printf("Chyba: Nelze otevrit soubor %s\n", nazev_souboru);
        return;
    }

    char radek[MAX_LINE_LENGTH];
    Ukol ukol;
    int index = 1;

    printf("\nUkoly na den (%s):\n", dnesni_datum);
    while (fgets(radek, sizeof(radek), soubor)) {
        radek[strcspn(radek, "\n")] = 0;

        char* token = strtok(radek, ";");
        if (token != NULL) {
            strcpy(ukol.nazev, token);
        }

        token = strtok(NULL, ";");
        if (token != NULL) {
            strcpy(ukol.datum, token);
        }

        token = strtok(NULL, ";");
        if (token != NULL) {
            strcpy(ukol.cas, token);
        }

        token = strtok(NULL, ";");
        if (token != NULL) {
            ukol.priorita = atoi(token);
        }

        token = strtok(NULL, ";");
        if (token != NULL) {
            ukol.hotovo = atoi(token);
        }

        if (strcmp(ukol.datum, dnesni_datum) == 0) {
            const char* barva;
            switch (ukol.priorita) {
            case 1:
                barva = GREEN;
                break;
            case 2:
                barva = YELLOW;
                break;
            case 3:
                barva = RED;
                break;
            default:
                barva = RESET;
                break;
            }

            const char* format = ukol.hotovo ? STRIKETHROUGH : "";
            printf("%d. %s%s%s%s%s\n", index, barva, format, ukol.nazev, RESET, RESET);
            printf("   Cas: %s\n", ukol.cas);
            index++;
        }
    }

    fclose(soubor);
}
void oznacit_ukol(const char* nazev_souboru, const char* dnesni_datum) {
    FILE* soubor = fopen(nazev_souboru, "r");
    if (soubor == NULL) {
        printf("Chyba: Nelze otevrit soubor %s\n", nazev_souboru);
        return;
    }

    char radek[MAX_LINE_LENGTH];
    Ukol ukol;
    int index = 1;
    int vybrany_index = -1;

    printf("\nVyberte ulohu k oznaceni na den (%s):\n", dnesni_datum);
    while (fgets(radek, sizeof(radek), soubor)) {
        radek[strcspn(radek, "\n")] = 0;

        char* token = strtok(radek, ";");
        if (token != NULL) {
            strcpy(ukol.nazev, token);
        }

        token = strtok(NULL, ";");
        if (token != NULL) {
            strcpy(ukol.datum, token);
        }

        token = strtok(NULL, ";");
        if (token != NULL) {
            strcpy(ukol.cas, token);
        }

        token = strtok(NULL, ";");
        if (token != NULL) {
            ukol.priorita = atoi(token);
        }

        token = strtok(NULL, ";");
        if (token != NULL) {
            ukol.hotovo = atoi(token);
        }

        if (strcmp(ukol.datum, dnesni_datum) == 0) {
            printf("%d. %s (Hotovo: %s)\n", index, ukol.nazev, ukol.hotovo ? "Ano" : "Ne");
            index++;
        }
    }

    printf("Zadejte cislo ulohy k oznaceni: ");
    scanf(" %d", &vybrany_index);

    rewind(soubor);
    index = 1;
    FILE* temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        printf("Chyba: Nelze otevrit docasny soubor\n");
        fclose(soubor);
        return;
    }

    while (fgets(radek, sizeof(radek), soubor)) {
        radek[strcspn(radek, "\n")] = 0;

        char* token = strtok(radek, ";");
        if (token != NULL) {
            strcpy(ukol.nazev, token);
        }

        token = strtok(NULL, ";");
        if (token != NULL) {
            strcpy(ukol.datum, token);
        }

        token = strtok(NULL, ";");
        if (token != NULL) {
            strcpy(ukol.cas, token);
        }

        token = strtok(NULL, ";");
        if (token != NULL) {
            ukol.priorita = atoi(token);
        }

        token = strtok(NULL, ";");
        if (token != NULL) {
            ukol.hotovo = atoi(token);
        }

        if (strcmp(ukol.datum, dnesni_datum) == 0 && index == vybrany_index) {
            ukol.hotovo = !ukol.hotovo; // Pøepnutí stavu hotovo
            printf("Uloha '%s' oznacena jako %s.\n", ukol.nazev, ukol.hotovo ? "hotova" : "nehotova");
        }

        fprintf(temp, "%s;%s;%s;%d;%d\n", ukol.nazev, ukol.datum, ukol.cas, ukol.priorita, ukol.hotovo);
        if (strcmp(ukol.datum, dnesni_datum) == 0) {
            index++;
        }
    }

    fclose(soubor);
    fclose(temp);
    remove(nazev_souboru);
    rename("temp.txt", nazev_souboru);
}
void zmen_den(struct tm* tm, int smer) {
    tm->tm_mday += smer;
    mktime(tm);
}

void pridat_ukol(const char* nazev_souboru, const char* datum) {
    FILE* soubor = fopen(nazev_souboru, "a");
    if (soubor == NULL) {
        printf("Chyba: Nelze otevrit soubor %s\n", nazev_souboru);
        return;
    }

    Ukol ukol;
    strcpy(ukol.datum, datum);

    printf("Zadejte nazev ulohy: ");
    getchar();
    fgets(ukol.nazev, sizeof(ukol.nazev), stdin);
    ukol.nazev[strcspn(ukol.nazev, "\n")] = 0;

    printf("Zadejte cas (hh:mm): ");
    scanf(" %5s", ukol.cas);
    printf("Zadejte prioritu (1-3): ");
    scanf(" %d", &ukol.priorita);
    ukol.hotovo = 0;

    fprintf(soubor, "%s;%s;%s;%d;%d\n", ukol.nazev, ukol.datum, ukol.cas, ukol.priorita, ukol.hotovo);
    fclose(soubor);
}
void zmenit_datum_ukolu(const char* nazev_souboru, const char* dnesni_datum) {
    FILE* soubor = fopen(nazev_souboru, "r");
    if (soubor == NULL) {
        printf("Chyba: Nelze otevrit soubor %s\n", nazev_souboru);
        return;
    }

    char radek[MAX_LINE_LENGTH];
    Ukol ukol;
    int index = 1;
    int vybrany_index = -1;

    printf("\nVyberte ulohu ke zmene data na den (%s):\n", dnesni_datum);
    while (fgets(radek, sizeof(radek), soubor)) {
        radek[strcspn(radek, "\n")] = 0;

        char* token = strtok(radek, ";");
        if (token != NULL) {
            strcpy(ukol.nazev, token);
        }

        token = strtok(NULL, ";");
        if (token != NULL) {
            strcpy(ukol.datum, token);
        }

        token = strtok(NULL, ";");
        if (token != NULL) {
            strcpy(ukol.cas, token);
        }

        token = strtok(NULL, ";");
        if (token != NULL) {
            ukol.priorita = atoi(token);
        }

        token = strtok(NULL, ";");
        if (token != NULL) {
            ukol.hotovo = atoi(token);
        }

        if (strcmp(ukol.datum, dnesni_datum) == 0) {
            printf("%d. %s\n", index, ukol.nazev);
            index++;
        }
    }

    printf("Zadejte cislo ulohy ke zmene data: ");
    scanf(" %d", &vybrany_index);

    rewind(soubor);
    index = 1;
    FILE* temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        printf("Chyba: Nelze otevrit docasny soubor\n");
        fclose(soubor);
        return;
    }

    while (fgets(radek, sizeof(radek), soubor)) {
        radek[strcspn(radek, "\n")] = 0;

        char* token = strtok(radek, ";");
        if (token != NULL) {
            strcpy(ukol.nazev, token);
        }

        token = strtok(NULL, ";");
        if (token != NULL) {
            strcpy(ukol.datum, token);
        }

        token = strtok(NULL, ";");
        if (token != NULL) {
            strcpy(ukol.cas, token);
        }

        token = strtok(NULL, ";");
        if (token != NULL) {
            ukol.priorita = atoi(token);
        }

        token = strtok(NULL, ";");
        if (token != NULL) {
            ukol.hotovo = atoi(token);
        }

        if (strcmp(ukol.datum, dnesni_datum) == 0 && index == vybrany_index) {
            printf("Upravujete datum pro ulohu: %s\n", ukol.nazev);
            printf("Zadejte nove datum (dd-mm-yyyy): ");
            scanf(" %10s", ukol.datum);
        }

        fprintf(temp, "%s;%s;%s;%d;%d\n", ukol.nazev, ukol.datum, ukol.cas, ukol.priorita, ukol.hotovo);
        if (strcmp(ukol.datum, dnesni_datum) == 0) {
            index++;
        }
    }

    fclose(soubor);
    fclose(temp);
    remove(nazev_souboru);
    rename("temp.txt", nazev_souboru);
}
void zobrazit_denni_rezim(const char* nazev_souboru, const char* datum) {
    printf("\nDenní režim (%s):\n", datum);
    nacti_ukoly(nazev_souboru, datum);
}
void zobrazit_tydenni_rezim(const char* nazev_souboru, const char* datum) {
    int den, mesic, rok;
    if (sscanf(datum, "%02d-%02d-%04d", &den, &mesic, &rok) != 3) {
        printf("Chyba: Neplatny format datumu. Ocekava se dd-mm-yyyy.\n");
        return;
    }
    struct tm tm = { 0 };
    tm.tm_mday = den;
    tm.tm_mon = mesic - 1;
    tm.tm_year = rok - 1900;
    mktime(&tm);

    int day_of_week = (tm.tm_wday == 0) ? 6 : tm.tm_wday - 1;
    tm.tm_mday -= day_of_week;
    mktime(&tm);

    printf("\nTydenni rezim (od %02d-%02d-%04d):\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);

    for (int i = 0; i < 7; i++) {
        char denni_datum[11];
        snprintf(denni_datum, sizeof(denni_datum), "%02d-%02d-%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
        nacti_ukoly(nazev_souboru, denni_datum);

        tm.tm_mday++;
        mktime(&tm);
    }
}





void upravit_ukol(const char* nazev_souboru, const char* dnesni_datum) {
    FILE* soubor = fopen(nazev_souboru, "r");
    if (soubor == NULL) {
        printf("Chyba: Nelze otevrit soubor %s\n", nazev_souboru);
        return;
    }

    char radek[MAX_LINE_LENGTH];
    Ukol ukol;
    int index = 1;
    int vybrany_index = -1;

    printf("\nVyberte ulohu k uprave na den (%s):\n", dnesni_datum);
    while (fgets(radek, sizeof(radek), soubor)) {
        radek[strcspn(radek, "\n")] = 0;

        char* token = strtok(radek, ";");
        if (token != NULL) {
            strcpy(ukol.nazev, token);
        }

        token = strtok(NULL, ";");
        if (token != NULL) {
            strcpy(ukol.datum, token);
        }

        token = strtok(NULL, ";");
        if (token != NULL) {
            strcpy(ukol.cas, token);
        }

        token = strtok(NULL, ";");
        if (token != NULL) {
            ukol.priorita = atoi(token);
        }

        token = strtok(NULL, ";");
        if (token != NULL) {
            ukol.hotovo = atoi(token);
        }

        if (strcmp(ukol.datum, dnesni_datum) == 0) {
            printf("%d. %s\n", index, ukol.nazev);
            index++;
        }
    }

    printf("Zadejte cislo ulohy k uprave: ");
    scanf(" %d", &vybrany_index);

    rewind(soubor);
    index = 1;
    FILE* temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        printf("Chyba: Nelze otevrit docasny soubor\n");
        fclose(soubor);
        return;
    }

    while (fgets(radek, sizeof(radek), soubor)) {
        radek[strcspn(radek, "\n")] = 0;

        char* token = strtok(radek, ";");
        if (token != NULL) {
            strcpy(ukol.nazev, token);
        }

        token = strtok(NULL, ";");
        if (token != NULL) {
            strcpy(ukol.datum, token);
        }

        token = strtok(NULL, ";");
        if (token != NULL) {
            strcpy(ukol.cas, token);
        }

        token = strtok(NULL, ";");
        if (token != NULL) {
            ukol.priorita = atoi(token);
        }

        token = strtok(NULL, ";");
        if (token != NULL) {
            ukol.hotovo = atoi(token);
        }

        if (strcmp(ukol.datum, dnesni_datum) == 0 && index == vybrany_index) {
            printf("\nUpravujete ulohu: %s\n", ukol.nazev);
            printf("Vyberte, co chcete upravit:\n");
            printf("1 - Nazev\n2 - Cas\n3 - Prioritu\n4 - Vsechny atributy\nVyberte moznost: ");
            int volba;
            scanf(" %d", &volba);

            if (volba == 1 || volba == 4) {
                printf("Zadejte novy nazev ulohy: ");
                getchar();
                fgets(ukol.nazev, sizeof(ukol.nazev), stdin);
                ukol.nazev[strcspn(ukol.nazev, "\n")] = 0;
            }
            if (volba == 2 || volba == 4) {
                printf("Zadejte novy cas (hh:mm): ");
                scanf(" %5s", ukol.cas);
            }
            if (volba == 3 || volba == 4) {
                printf("Zadejte novou prioritu (1-3): ");
                scanf(" %d", &ukol.priorita);
            }
        }

        fprintf(temp, "%s;%s;%s;%d;%d\n", ukol.nazev, ukol.datum, ukol.cas, ukol.priorita, ukol.hotovo);
        if (strcmp(ukol.datum, dnesni_datum) == 0) {
            index++;
        }
    }

    fclose(soubor);
    fclose(temp);
    remove(nazev_souboru);
    rename("temp.txt", nazev_souboru);
}
void vymazat_ukol(const char* nazev_souboru, const char* dnesni_datum) {
    FILE* soubor = fopen(nazev_souboru, "r");
    if (soubor == NULL) {
        printf("Chyba: Nelze otevrit soubor %s\n", nazev_souboru);
        return;
    }

    char radek[MAX_LINE_LENGTH];
    Ukol ukol;
    int index = 1;
    int vybrany_index = -1;

    printf("\nVyberte ulohu k vymazani na den (%s):\n", dnesni_datum);
    while (fgets(radek, sizeof(radek), soubor)) {
        radek[strcspn(radek, "\n")] = 0;

        char* token = strtok(radek, ";");
        if (token != NULL) {
            strcpy(ukol.nazev, token);
        }

        token = strtok(NULL, ";");
        if (token != NULL) {
            strcpy(ukol.datum, token);
        }

        token = strtok(NULL, ";");
        if (token != NULL) {
            strcpy(ukol.cas, token);
        }

        token = strtok(NULL, ";");
        if (token != NULL) {
            ukol.priorita = atoi(token);
        }

        token = strtok(NULL, ";");
        if (token != NULL) {
            ukol.hotovo = atoi(token);
        }

        if (strcmp(ukol.datum, dnesni_datum) == 0) {
            printf("%d. %s\n", index, ukol.nazev);
            index++;
        }
    }

    printf("Zadejte cislo ulohy k vymazani: ");
    scanf(" %d", &vybrany_index);

    rewind(soubor);
    index = 1;
    FILE* temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        printf("Chyba: Nelze otevrit docasny soubor\n");
        fclose(soubor);
        return;
    }

    while (fgets(radek, sizeof(radek), soubor)) {
        radek[strcspn(radek, "\n")] = 0;

        char* token = strtok(radek, ";");
        if (token != NULL) {
            strcpy(ukol.nazev, token);
        }

        token = strtok(NULL, ";");
        if (token != NULL) {
            strcpy(ukol.datum, token);
        }

        token = strtok(NULL, ";");
        if (token != NULL) {
            strcpy(ukol.cas, token);
        }

        token = strtok(NULL, ";");
        if (token != NULL) {
            ukol.priorita = atoi(token);
        }

        token = strtok(NULL, ";");
        if (token != NULL) {
            ukol.hotovo = atoi(token);
        }

        if (strcmp(ukol.datum, dnesni_datum) == 0 && index == vybrany_index) {
            printf("Uloha '%s' byla vymazana.\n", ukol.nazev);
        }
        else {
            fprintf(temp, "%s;%s;%s;%d;%d\n", ukol.nazev, ukol.datum, ukol.cas, ukol.priorita, ukol.hotovo);
        }

        if (strcmp(ukol.datum, dnesni_datum) == 0) {
            index++;
        }
    }

    fclose(soubor);
    fclose(temp);
    remove(nazev_souboru);
    rename("temp.txt", nazev_souboru);
}