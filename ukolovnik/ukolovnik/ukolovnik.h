#pragma once
#include <iostream>
#ifndef UKOLOVNIK_H
#define UKOLOVNIK_H

#include <time.h>

typedef struct {
    char nazev[100];
    char datum[11];
    char cas[6];
    int priorita;
    int hotovo;
} Ukol;

void nacti_ukoly(const char* nazev_souboru, const char* dnesni_datum);
void oznacit_ukol(const char* nazev_souboru, const char* dnesni_datum);
void zmen_den(struct tm* tm, int smer);
void pridat_ukol(const char* nazev_souboru, const char* datum);
void zmenit_datum_ukolu(const char* nazev_souboru, const char* dnesni_datum);
void upravit_ukol(const char* nazev_souboru, const char* dnesni_datum);
void zobrazit_denni_rezim(const char* nazev_souboru, const char* datum);
void zobrazit_tydenni_rezim(const char* nazev_souboru, const char* datum);
void vymazat_ukol(const char* nazev_souboru, const char* dnesni_datum);

#endif

