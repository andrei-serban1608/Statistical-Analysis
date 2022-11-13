#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
// se defineste o structura pentru perechea folosita 
// in datele de intrare
typedef struct data{
    int timestamp;
    double value;
}data;
// se defineste o structura pentru nodul unei
// liste dublu inlantuite, in care tipul de date
// este perechea (timestamp, value)
typedef struct node{
    data elem;
    struct node *prev;
    struct node *next;
}node;
// functia pentru adaugarea unui element nou pe ultima
// pozitie a listei; in cazul in care lista este vida
// aceasta este creata cu un singur element, altfel
// se adauga cu un element auxiliar la sfarsitul listei
void addToEnd(node **head, node **tail, data x){
    node *newcell = (node *)calloc(1, sizeof(node));
    newcell->elem = x;
    newcell->next = NULL;
    if(*head == NULL){
        *head = newcell;
        *tail = newcell;
    }
    else{
        newcell->prev = *tail;
        (*tail)->next = newcell;
        (*tail) = newcell;
    }
}
// functia pentru citirea de la tastatura a elementelor
// ce vor fi introduse in lista dublu inlantuita
void readList(node **head, node **tail, int n){
    int i;
    data d;
    for(i = 1; i <= n; i++){
        scanf("%d%lf", &(d.timestamp), &(d.value));
        addToEnd(head, tail, d);
    }
}
// functia pentru eliberarea fiecarei celule din lista
void freeList(node *head, node *tail)
{
    node *i;
    while (head != NULL){
        i = head;
        head = head->next;
        free(i);
    }
}
// functia pentru afisarea campurilor de date
// retinute in lista
void printList(node *head){
    node *i;
    for(i = head; i != NULL; i = i->next)
        printf("%d %.2lf\n", i->elem.timestamp, i->elem.value);
}
// functia pentru eliminarea unui element cu adresa x
// din lista
void removeElement(node **head, node **tail, node *x){
    node *i, *next_i;
    for(i = *head; i != NULL; i = next_i){
        // fac acest artificiu pentru a evita accesarea unei celule dintr-o celula eliberata din memorie
        next_i = i->next;
        if(i == x){
            // daca elementul eliminat e capul listei, capul listei se modifica
            if(i == *head){
                i->next->prev = NULL;
                *head = i->next;
            }
            // analog, daca elementul eliminat e coada listei
            else if(i == *tail){
                i->prev->next = NULL;
                *tail = i->prev;
            }
            // in cazul general de eliminare a unui element de la mijlocul listei,
            // se schimba legaturile astfel incat elementul de dupa cel eliminat
            // sa pointeze la cel dinaintea elementului eliminat si cel de dinaintea
            // elementului eliminat sa pointeze la cel de dupa elementul eliminat
            else{
                i->prev->next = i->next;
                i->next->prev = i->prev;
            }
            free(i);
        }
    }
}
// functia pentru adaugarea unui element cu un timestamp "TStamp" si o valoare
// "value", dupa elementul cu adresa *x
void addAfterAddress(node **head, node **tail, node **x, int TStamp, double value){
    node *i = *head, *newcell;
    while(i != NULL){
        if(i == *x){
            newcell = (node *)malloc(sizeof(node));
            newcell->elem.timestamp = TStamp;
            newcell->elem.value = value;
            newcell->next = i->next;
            newcell->prev = i;
            i->next = newcell;
            if(newcell->next != NULL)
                newcell->next->prev = newcell;
            break;
        }
        else
            i = i->next;
    }
}
// functia pentru rezolvarea punctului 2.1 din tema:
// "Eliminare de exceptii folosind metode statistice"
void e1(node **head, node **tail, int *n){
    double mean, stdDev, sum;
    node *i, *j, *tmpHead, *tmpTail, *next_i;
    // initializez o lista temporara pentru a retine valorile ce trebuie eliminate
    tmpHead = tmpTail = NULL;
    int k;
    // parcurg lista de la cel de-al 3-lea element, pana la antepenultimul, deoarece marimea ferestrei
    // este de k = 5; astfel, se parcurge fiecare mijloc de fereastra
    for(i = (*head)->next->next; i->next->next != NULL; i = i->next){
        // pentru parcurgerea fiecarei ferestre in parte, pointer-ul de parcurgere va incepe mereu
        // cu doua pozitii inaintea mijlocului ferestrei si va merge in ->next de 5 ori
        j = i->prev->prev;
        // se calculeaza suma elementelor din fereastra
        sum = 0;
        for(k = 1; k <= 5; k++){
            sum += j->elem.value;
            j = j->next;
        }
        // formula mediei
        mean = sum / 5;
        // este iarasi initializat pointer-ul de parcurgere a ferestrei pentru calcului deviatiei standard
        j = i->prev->prev;
        // se calculeaza suma patratelor diferentelor dintre element si medie
        sum = 0;
        for(k = 1; k <= 5; k++){
            sum += (j->elem.value - mean) * (j->elem.value - mean);
            j = j->next;
        }
        // formula deviatiei standard
        stdDev = sqrt(sum / 5);
        // se verifica daca mijlocul ferestrei apartine intervalului [mean - stdDev, mean + stdDev]
        if(i->elem.value < (mean - stdDev) || i->elem.value > (mean + stdDev))
            // daca nu apartine, este adaugat la lista temporara pentru eliminarea ulterioara
            addToEnd(&tmpHead, &tmpTail, i->elem);
    }
    // parcurg elementele relevante ale listei (de la al 3lea la antepenultimul) si 
    // le compar pe rand cu toate elementele din lista temporara; in cazul egalitatii
    // elementul este eliminat din lista si numarul total de elemente scade
    for(j = tmpHead; j != NULL; j = j->next)
        for(i = (*head)->next->next; i->next->next != NULL; i = next_i)
        {
            // din nou, fac acest artificiu deoarece celula de la i este eventual eliminata,
            // caz in care s-ar trece mai departe printr-o referinta la o celula deja eliberata
            // din memorie
            next_i = i->next;
                // se verifica ambele campuri de date ale continutului celulelor, deoarece
                // egalitatea == este invalida la structuri de date complexe
                if(i->elem.timestamp == j->elem.timestamp && i->elem.value == j->elem.value){
                    removeElement(head, tail, i);
                    (*n)--;
                }
        }
    // eliberarea memoriei pentru lista temporara
    freeList(tmpHead, tmpTail);
}
// functia pentru rezolvarea punctului 2.2.1 din tema:
// "Eliminare de zgomot folosind filtre: Filtrarea mediana"
void e2(node **head, node **tail, int *n){
    node *i, *j, *tmpHead, *tmpTail, *aux;
    int k;
    double max1, max2, max3;
    // initializez o lista temporara pentru a retine toate valorile mediane,
    // pe care apoi le copiez in lista initiala
    tmpHead = tmpTail = NULL;
    // se parcurg doar nodurile relevante pentru mijloacele ferestrelor
    for(i = (*head)->next->next; i->next->next != NULL; i = i->next){
        // pentru parcurgerea ferestrei de k=5 pointerul e initializat mereu cu doua pozitii inaintea mijlocului
        j = i->prev->prev;
        // la fiecare parcurgere este introdus in lista temporara mijlocul ferestrei, pentru pastrarea timestamp-ului
        addToEnd(&tmpHead, &tmpTail, i->elem);
        // in loc sa sortez fiecare fereastra si sa aflu elementul de la mijloc, lucrez mereu cu 5 elemente, deci
        // daca calculez cele mai mari 3 valori, cea de-a 3a va fi intotdeauna mediana ferestrei; initializez
        // valorile maxime cu cel mai mic numar ce poate fi retinut pe un double
        max1 = max2 = max3 = -1 * __DBL_MAX__;
        for(k = 1; k <= 5; k++){
            // daca valoarea gasita e mai mare decat max1, toate cele 3 valori se modifica
            if(j->elem.value >= max1){
                max3 = max2;
                max2 = max1;
                max1 = j->elem.value;
            }
            // daca valoarea gasita e mai mica decat max1, dar mai mare decat max2, doar max2 si max3 se modifica
            else if(j->elem.value >= max2){
                max3 = max2;
                max2 = j->elem.value;
            }
            // daca valoarea gasita e mai mica decat max1 si max2, dar mai mare decat max3, doar aceasta se modifica
            else if(j->elem.value >= max3)
                max3 = j->elem.value;
            j = j->next;
        }
        // coada listei temporare isi modifica campul pentru valoare cu mediana ferestrei, dar timestamp-ul ramane acelasi
        tmpTail->elem.value = max3;
    }
    // se parcurg ambele liste pana la sfarsitul listei mai mici (cea temporara) pentru a se inlocui in lista
    // initiala campurile de date pentru filtrarea cu mediana
    j = tmpHead;
    for(i = *head; j != NULL; i = i->next){
        i->elem = j->elem;
        j = j->next;
    }
    // se elibereaza memoria pentru celulele de la finalul listei temporare pana la finalul listei principale
    freeList(i->next, *tail);
    // se modifica referinta la elementul de pe pozitia ultimului element din lista temporara, pentru a
    // afisa doar pana la acea valoare
    aux = i->prev->next;
    i->prev->next = NULL;
    *tail = i->prev;
    // se elibereaza memoria pentru celula respectiva
    free(aux);
    // n-ul se micsoreaza cu 4, deoarece primele 2 si ultimele 2 elemente sunt eliminate
    *n -= 4;
    // se elibereaza memoria pentru lista temporara
    freeList(tmpHead, tmpTail);
}
// functia pentru rezolvarea punctului 2.2.2 din tema:
// "Eliminarea de zgomot folosind filtre: filtrare folosind
// media aritmetica"; implementare extrem de similara cu e2,
// doar ca nu se mai calculeaza cele 3 minime, ci media aritmetica
void e3(node **head, node **tail, int *n){
    node *i, *j, *tmpHead, *tmpTail, *aux;
    int k;
    double sum, mean;
    // initializez o lista temporara pentru a retine toate valorile mediilor,
    // pe care apoi le copiez in lista initiala
    tmpHead = tmpTail = NULL;
    // se parcurg doar nodurile relevante pentru mijloacele ferestrelor
    for(i = (*head)->next->next; i->next->next != NULL; i = i->next){
        // pentru parcurgerea ferestrei de k=5 pointerul e initializat mereu cu doua pozitii inaintea mijlocului
        j = i->prev->prev;
        // la fiecare parcurgere este introdus in lista temporara mijlocul ferestrei, pentru pastrarea timestamp-ului
        addToEnd(&tmpHead, &tmpTail, i->elem);
        // calculul sumei pentru a calcula media aritmetica pe fereastra respectiva
        sum = 0;
        for(k = 1; k <= 5; k++){
            sum += j->elem.value;
            j = j->next;
        }
        mean = sum / 5;
        // coada listei temporare isi modifica campul pentru valoare cu media aritmetica a ferestrei, dar timestamp-ul ramane acelasi
        tmpTail->elem.value = mean;
    }
    // se parcurg ambele liste pana la sfarsitul listei mai mici (cea temporara) pentru a se inlocui in lista
    // initiala campurile de date pentru filtrarea cu mediana
    j = tmpHead;
    for(i = *head; j != NULL; i = i->next){
        i->elem = j->elem;
        j = j->next;
    }
    // se elibereaza memoria pentru celulele de la finalul listei temporare pana la finalul listei principale
    freeList(i->next, *tail);
    // se modifica referinta la elementul de pe pozitia ultimului element din lista temporara, pentru a
    // afisa doar pana la acea valoare
    aux = i->prev->next;
    i->prev->next = NULL;
    *tail = i->prev;
    // se elibereaza memoria pentru celula respectiva
    free(aux);
    // n-ul se micsoreaza cu 4, deoarece primele 2 si ultimele 2 elemente sunt eliminate
    *n -= 4;
    // se elibereaza memoria pentru lista temporara
    freeList(tmpHead, tmpTail);
}
// functia pentru rezolvarea punctului 2.3 din tema:
// "Uniformizarea frecventei in timp a datelor"
void u(node **head, node **tail){
    node *i;
    // se parcurge lista de la al 2lea element pana la capat, deoarece primul element nu are cu ce sa fie comparat
    for(i = (*head)->next; i != NULL; i = i->next)
        // in cazul in care diferenta de timp dintre doua noduri consecutive e in intervalul [100ms, 1000ms],
        // se redistribuie timestampul si valoarea conform cerintei
        if(i->elem.timestamp - i->prev->elem.timestamp >= 100 && i->elem.timestamp - i->prev->elem.timestamp <= 1000){
            i->elem.timestamp = (i->elem.timestamp + i->prev->elem.timestamp) / 2;
            i->elem.value = (i->elem.value + i->prev->elem.value) / 2;
        }
}
// functia pentru rezolvarea punctului 2.4 din tema:
// "Completarea datelor"
void c(node **head, node **tail, int *n){
    node *i, *left[3], *right[3];
    int nr, i1, i2;
    double C, w, f, s1, s2, s3;
    // se parcurg toate nodurile pana la penultimul nod al listei
    for(i = *head; i->next != NULL; i = i->next){
        // in cazul in care se gasesc doua noduri consecutive cu o diferenta intre
        // ele mai mare de 1000 de ms, se initializeaza vectorii left si right pe
        // o fereastra de k = 3 pentru calculul valorilor completate si variabila
        // nr functioneaza ca un contor care porneste de la timestampul elementului
        // din stanga saltului de mai mult de o secunda
        if(i->next->elem.timestamp - i->elem.timestamp > 1000){
            left[0] = i->prev->prev;
            left[1] = i->prev;
            left[2] = i;
            right[2] = i->next;
            right[1] = i->next->next;
            right[0] = i->next->next->next;
            nr = i->elem.timestamp;
            // conditia de oprire a contorului este acea cand numarul devine mai mare sau egal
            // cu capatul drept al saltului, din care se scad 200 de ms, pentru a nu se depasi
            // timestampul capatului drept
            while(nr < i->next->elem.timestamp - 200){
                // creste numarul de elemente din lista n, transmis prin adresa
                (*n)++;
                // calculeaza C-ul
                C = (double)(nr + 200 - left[2]->elem.timestamp) / (right[2]->elem.timestamp - left[2]->elem.timestamp);
                s1 = 0;
                s2 = 0;
                for(i1 = 0; i1 <= 2; i1++){
                    // calculeaza w-ul
                    s3 = 0;
                    for(i2 = 0; i2 <= 2; i2++)
                        s3 += (double)(pow(i2 / 2.0, 2) * 0.9 + 0.1);
                    w = (double)(pow(i1 / 2.0, 2) * 0.9 + 0.1) / s3;
                    // calculeaza sumele pentru f
                    s1 += (double)left[i1]->elem.value * w;
                    s2 += (double)right[i1]->elem.value * w;
                }
                // formula lui f
                f = (double)((1 - C) * s1 + C * s2);
                // se adauga dupa capatul din stanga al saltului elementul ce contine
                // timestampul nr + 200 si valorea f
                addAfterAddress(head, tail, &i, nr + 200, f);
                // contorul creste cu 200, deoarece frecventa datelor trebuie sa fie de 5Hz
                // (cel putin 5 date pe secunda)
                nr += 200;
                // creste si i-ul deoarece la urmatoarea valoare completata, adresa trebuie
                // sa creasca, pentru a nu se adauga pe aceeasi pozitie toate elementele
                i = i->next;
            }
        }
    }
}
// functia pentru functionarea punctului 2.5 din tema:
// "Statistici"
void st(node **head, node **tail, int n, int delta){
    node *i;
    int nr, intStart = 0;
    // calculez valoarea minima din functie, deoarece, daca aceasta este negativa
    // trebuie sa stiu cum sa aleg primul interval in care se afla valori
    double minVal = (*head)->elem.value;
    for(i = (*head)->next; i != NULL; i = i->next)
        if(i->elem.value < minVal)
            minVal = i->elem.value;
    // calculez inceputul primului interval, scazand delta din valoarea initiala 0
    // pana cand capatul din stanga al intervalului este mai mare sau egal cu
    // valoarea minima din functie
    while(minVal < intStart)
        intStart -= delta;
    // parcurg toate intervalele de la valoarea minima calculata anterior si de fiecare
    // data intStart creste cu delta; conditia de oprire este aceea cand n-ul devine 0,
    // adica cand toate elementele au fost atribuite ca parte dintr-un interval; acesta
    // este si motivul pentru care n-ul este pasat ca argument prin valoare, pentru a
    // nu se modifica valoarea sa finala in main
    for(; n != 0; intStart += delta){
        // nr retine cate numere din lista apartin fiecarul interval [intStart, intStart + delta]
        nr = 0;
        // se parcurge lista pentru fiecare interval si de fiecare data cand un element apartine
        // intervalului curent, nr se incrementeaza si numarul total de elemente neatribuite
        // unui interval, n, se decrementeaza
        for(i = *head; i != NULL; i = i->next)
            if(i->elem.value >= intStart && i->elem.value <= intStart + delta){
                nr++;
                n--;
            }
        // functia afiseaza intervalul doar daca exista cel putin o valoare in acesta,
        // deoarece primul for parcurge toate intervalele de la valoarea minima a functiei
        // pana cand nu mai sunt elemente de introdus in intervale
        if(nr != 0)
            printf("[%d, %d] %d\n", intStart, intStart + delta, nr);
    }
}
// argc si argv, parametri standard pentru lucrul cu argumente in linia de comanda
int main(int argc, char **argv){
    int i, n, delta = 0, j, p = 1, st_flag = 0;
    node *head, *tail;
    char ch_delta[1000];
    // initializare lista nula
    head = tail = NULL;
    // citire numar perechi
    scanf("%d", &n);
    // citire perechi
    readList(&head, &tail, n);
    // se pargurg toate argumentele din linia de comanda, in ordine, astfel metodele
    // de filtrare si uniformizare a datelor se executa in ordinea parametrilor
    for(i = 1; i < argc; i++){
        if(strcmp(argv[i], "--e1") == 0)
            e1(&head, &tail, &n);
        else if(strcmp(argv[i], "--e2") == 0)
            e2(&head, &tail, &n);
        else if(strcmp(argv[i], "--e3") == 0)
            e3(&head, &tail, &n);
        else if(strcmp(argv[i], "--u") == 0)
            u(&head, &tail);
        else if(strcmp(argv[i], "--c") == 0)
            c(&head, &tail, &n);
        // toate celelalte ramuri ale structurii de decizie folosesc functia strcmp,
        // la ultimul subpunct se foloseste strstr deoarece numarul din coada parametrului
        // poate varia si trebuie parsat
        else if(strstr(argv[i], "--st") != NULL){
            // st_flag indica daca argumentul dat este de tip --st<delta>,
            // caz in care nu mai afiseaza lista, ci intervalele in care se
            // afla valorile
            st_flag = 1;
            // in ch_delta copiez doar partea numerica a argumentului in linia de comanda,
            // copiind in acea variabila adresa de la caracterul 5 al sirului --st<delta>
            // (4 este lungimea sirului "--st")
            strcpy(ch_delta, argv[i] + 4);
            // se parcurg pozitiile sirului ch_delta de la coada la cap, pentru a construi
            // de la ordinul unitatilor la ordin superior parametrul intreg delta
            for(j = strlen(ch_delta) - 1; j >= 0; j--){
                // delta este initializat cu 0 la declarare si la fiecare parcurgere se adauga
                // valoarea de pe pozitia j din ch_delta, castata la int (se scade codul ascii
                // al lui 0, deoarece caracterele se afla la '0' pozitii distanta de la valoarea
                // intreaga a codurilor ascii
                delta = delta + (int)(ch_delta[j] - '0') * p;
                // p-ul este initializat cu 1 la declarare, si la fiecare salt al for-ului se inmulteste
                // cu 10, pentru a construi urmatoarea cifra la un ordin superior
                p *= 10;
            }
            // dupa ce e creat numarul intreg delta, acesta e pasat ca argument pentru functia st
            st(&head, &tail, n, delta);
        }
        else
            break;
    }
    // se afiseaza lista modificata doar daca nu s-a apelat argumentul
    // --st<delta>
    if(st_flag == 0){
        // afisare numar nou de elemente
        printf("%d\n", n);
        // afisare lista modificata
        printList(head);
    }
    // eliberare memorie lista
    freeList(head, tail);
    return 0;
}