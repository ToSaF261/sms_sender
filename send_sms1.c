#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

int main(void) {
    
    printf("Content-type: text/html\n\n"); 

    char *longueur_str = getenv("CONTENT_LENGTH"); 
    if (longueur_str == NULL) {
        printf("<html><body><p>Erreur: Aucune donnée reçue.</p></body></html>");
        return 1;
    }

    int longueur = atoi(longueur_str); 
    if (longueur <= 0) {
        printf("<html><body><p>Erreur: Longueur de données incorrecte.</p></body></html>");
        return 1;
    }

    char donnees_post[1024]; 
    if (fread(donnees_post, 1, longueur, stdin) != longueur) {
        printf("<html><body><p>Erreur lors de la lecture des données.</p></body></html>");
        return 1;
    }
    donnees_post[longueur] = '\0'; 

    char *numero_utilisateur = strstr(donnees_post, "numero=");
    if (numero_utilisateur == NULL) {
        printf("<html><body><p>Erreur: Paramètre 'numero' manquant.</p></body></html>");
        return 1;
    }
    numero_utilisateur += 7; 

    char *fin = strchr(numero_utilisateur, '&'); 
    if (fin != NULL) {
        *fin = '\0'; 
    }

    CURL *curl; 
    CURLcode resultat; 
    char donnees[512]; 

    curl = curl_easy_init(); 
    if (curl) {
        struct curl_slist *en_tetes = NULL; 

        curl_easy_setopt(curl, CURLOPT_URL, "https://yp3zld.api.infobip.com/sms/2/text/advanced"); 

        en_tetes = curl_slist_append(en_tetes, "Authorization: App 7d8d45366c40bbcb49f54352267a1ff0-1c2a59ad-bebf-4e91-9843-94314533006d"); 
        en_tetes = curl_slist_append(en_tetes, "Content-Type: application/json"); 
        en_tetes = curl_slist_append(en_tetes, "Accept: application/json"); 

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, en_tetes);

        snprintf(donnees, sizeof(donnees), 
            "{\"messages\":[{\"destinations\":[{\"to\":\"261345402966\"}],\"from\":\"%s\",\"text\":\"Ity laharana ity dia tafita soamatsara amin'ny dingana manaraka. Ito ny teny miafina hamahany ny niveau2  8426 .\"}]}",
            numero_utilisateur);

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, donnees); 

        resultat = curl_easy_perform(curl);

        if (resultat != CURLE_OK) {
            printf("<html><body><p>Erreur lors de l'envoi du SMS: %s</p></body></html>", curl_easy_strerror(resultat)); 
        } else {
            printf("<html><body><p>SMS envoyé avec succès à %s</p></body></html>", numero_utilisateur); 
        }

        curl_slist_free_all(en_tetes); 
        curl_easy_cleanup(curl); 
    } else {
        printf("<html><body><p>Erreur lors de l'initialisation de cURL.</p></body></html>");
    }

    return 0; 
}
