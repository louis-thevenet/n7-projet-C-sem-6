#define _GNU_SOURCE
#include "liste_noeud.h"
#include <math.h>
#include <stdlib.h>

struct liste_noeud_t {
  noeud_id_t noeud;
  float distance;
  noeud_id_t precedent;
  liste_noeud_t *suivant;
};

liste_noeud_t *creer_liste() {
  liste_noeud_t *liste = malloc(sizeof(liste_noeud_t));

  if (liste == NULL) {
    exit(EXIT_FAILURE);
  }

  liste->noeud = NO_ID;
  liste->distance = INFINITY;
  liste->precedent = NO_ID;
  liste->suivant = NULL;

  return liste;
}
void detruire_liste(liste_noeud_t **liste_ptr) {
  if (liste_ptr == NULL || *liste_ptr == NULL) {
    return;
  }

  detruire_liste(&(*liste_ptr)->suivant);
  free(*liste_ptr);
  *liste_ptr = NULL;
}
bool est_vide_liste(const liste_noeud_t *liste) {
  return NO_ID == liste->noeud;
}

bool contient_noeud_liste(const liste_noeud_t *liste, noeud_id_t noeud) {
  if (est_vide_liste(liste)) {
    return false;
  }
  return liste->noeud == noeud || contient_noeud_liste(liste->suivant, noeud);
}

bool contient_arrete_liste(const liste_noeud_t *liste, noeud_id_t source,
                           noeud_id_t destination) {

  if (est_vide_liste(liste))
    return false;

  if (liste->noeud == destination && liste->precedent == source)
    return true;

  return contient_arrete_liste(liste->suivant, source, destination);
}

float distance_noeud_liste(const liste_noeud_t *liste, noeud_id_t noeud) {
  if (est_vide_liste(liste))
    return INFINITY;

  if (liste->noeud == noeud)
    return liste->distance;

  return distance_noeud_liste(liste->suivant, noeud);
}

noeud_id_t precedent_noeud_liste(const liste_noeud_t *liste, noeud_id_t noeud) {
  if (est_vide_liste(liste))
    return NO_ID;

  if (liste->noeud == noeud)
    return liste->precedent;

  return precedent_noeud_liste(liste->suivant, noeud);
}

noeud_id_t min_noeud_liste(const liste_noeud_t *liste) {
  liste_noeud_t *tmp = malloc(sizeof(liste_noeud_t));
  liste_noeud_t *head = tmp;
  *head = *liste;

  noeud_id_t min_noeud = NO_ID;
  float min = INFINITY;

  while (!est_vide_liste(head)) {
    if (head->distance < min) {
      min = head->distance;
      min_noeud = head->noeud;
    }
    head = head->suivant;
  }
  free(tmp);
  return min_noeud;
}

void inserer_noeud_liste(liste_noeud_t *liste, noeud_id_t noeud,
                         noeud_id_t precedent, float distance) {

  liste_noeud_t *tmp = malloc(sizeof(liste_noeud_t));
  *tmp = *liste;
  liste->noeud = noeud;
  liste->precedent = precedent;
  liste->distance = distance;
  liste->suivant = tmp;
}

void changer_noeud_liste(liste_noeud_t *liste, noeud_id_t noeud,
                         noeud_id_t precedent, float distance) {

  if (!contient_noeud_liste(liste, noeud)) {
    inserer_noeud_liste(liste, noeud, precedent, distance);
  } else {
    if (liste->noeud == noeud) {

      liste->precedent = precedent;
      liste->distance = distance;

    } else {
      changer_noeud_liste(liste->suivant, noeud, precedent, distance);
    }
  }
}

void supprimer_noeud_liste(liste_noeud_t *liste, noeud_id_t noeud) {
  if (est_vide_liste(liste))
    return;

  if (liste->noeud == noeud) {
    liste_noeud_t *tmp = liste->suivant;
    *liste = *(liste->suivant);
    free(tmp);
  } else {
    supprimer_noeud_liste(liste->suivant, noeud);
  }
}
