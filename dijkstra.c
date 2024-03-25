#include "dijkstra.h"
#include <stdlib.h>

/**
 * construire_chemin_vers - Construit le chemin depuis le noeud de départ donné
 * vers le noeud donné. On passe un chemin en entrée-sortie de la fonction, qui
 * est mis à jour par celle-ci.
 *
 * Le noeud de départ est caractérisé par un prédécesseur qui vaut `NO_ID`.
 *
 * Ce sous-programme fonctionne récursivement :
 *  1. Si le noeud a pour précédent `NO_ID`, on a fini (c'est le noeud de
 * départ, le chemin de départ à départ se compose du simple noeud départ)
 *  2. Sinon, on construit le chemin du départ au noeud précédent (appel
 * récursif)
 *  3. Dans tous les cas, on ajoute le noeud au chemin, avec les
 * caractéristiques associées dans visites
 *
 * @param chemin [in/out] chemin dans lequel enregistrer les étapes depuis le
 * départ vers noeud
 * @param visites [in] liste des noeuds visités créée par l'algorithme de
 * Dijkstra
 * @param noeud noeud vers lequel on veut construire le chemin depuis le départ
 */
void construire_chemin_vers(liste_noeud_t *chemin, liste_noeud_t *visites,
                            noeud_id_t noeud) {
  if (noeud == NO_ID)
    return;
  construire_chemin_vers(chemin, visites,
                         precedent_noeud_liste(visites, noeud));
  inserer_noeud_liste(chemin, noeud, precedent_noeud_liste(visites, noeud),
                      distance_noeud_liste(visites, noeud));
}

float dijkstra(const struct graphe_t *graphe, noeud_id_t source,
               noeud_id_t destination, liste_noeud_t **chemin) {

  liste_noeud_t *a_visites = creer_liste();
  inserer_noeud_liste(a_visites, source, NO_ID, 0.0);

  liste_noeud_t *visites = creer_liste();

  noeud_id_t *voisins = malloc(sizeof(noeud_id_t) * nombre_noeuds(graphe));

  while (!est_vide_liste(a_visites)) {
    noeud_id_t noeud_courant = min_noeud_liste(a_visites);
    inserer_noeud_liste(visites, noeud_courant,
                        precedent_noeud_liste(a_visites, noeud_courant),
                        distance_noeud_liste(a_visites, noeud_courant));
    supprimer_noeud_liste(a_visites, noeud_courant);

    int n = nombre_voisins(graphe, noeud_courant);
    noeuds_voisins(graphe, noeud_courant, voisins);

    for (int i = 0; i < n; i++) {
      float distance_par_nc =
          noeud_distance(graphe, noeud_courant, voisins[i]) +
          distance_noeud_liste(visites, noeud_courant);
      float distance_voisin = distance_noeud_liste(a_visites, voisins[i]);

      if (distance_par_nc < distance_voisin) {
        changer_noeud_liste(a_visites, voisins[i], noeud_courant,
                            distance_par_nc);
      }
    }
  }

  if (chemin != NULL) {
    *chemin = creer_liste();
    construire_chemin_vers(*chemin, visites, destination);
  }
  float res =  distance_noeud_liste(visites, destination);
detruire_liste(&a_visites);
detruire_liste(&visites);
free(voisins);
return res;
}
