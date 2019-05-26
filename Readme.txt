Gabriel Boroghina
Grupa 333CB

                                  APD - Tema 1


    Functia jobBoundsForThread determina portiunea din imagine care va fi lucrata
de un thread. Aceasta este specificata prin 2 indici start si end, reprezentand
indici ai blocurilor (sau pixelilor, in cazul taskului 2) de prelucrat. Threadul
va lucra in intervalul [start, end). Distributia job-urilor pentru threaduri s-a
facut astfel incat toate threadurile sa aiba acelasi numar de blocuri de procesat,
cu exceptia primelor r, care mai au un bloc in plus, unde r = restul impartirii
numarului de blocuri la numarul de threaduri.


--------------------------------------TASK 1--------------------------------------

    Imaginea este retinuta folosind structura image, ce contine tipul imaginii
(color sau grayscale), dimensiunile, valoarea maxval si o matrice de pixeli
generici, ce pot fi convertiti la unul din tipurile color_pixel sau grayscale_pixel.

    Impartirea imaginii pe threaduri se face atribuind fiecarui thread un interval
de blocuri disjuncte de resize_factor x resize_factor pixeli. Intervalele sunt
alese in ordinea parcurgerii de sus in jos si de la stanga la dreapta a matricei
de pixeli, fiecare "banda" orizontala de grosime resize_factor din matrice fiind
impartita de la stanga la dreapta in blocuri patrate.
    Fiecare bloc resize_factor x resize_factor va trebui comprimat intr-un pixel,
prin urmare, se va calcula media aritmetica a pixelilor din bloc (pentru
resize_factor diferit de 3) sau media ponderata folosind matricea Kernel Gaussian
(daca resize_factor = 3) si se va scrie acest pixel in matricea de pixeli a
imaginii de iesire.
    De remarcat ca threadurile nu vor accesa niciodata aceeasi zona de memorie
(blocurile din imaginea initiala sunt disjuncte, iar pixelii din matricea finala
sunt diferiti), deci nu este nevoie de sincronizarea threadurilor.


--------------------------------------TASK 2--------------------------------------

    Task-ul 2 presupune umplerea unei matrice de pixeli grayscale folosind un numar
dat de threaduri. Cei height * width pixeli vor fi distribuiti cat mai egal celor
num_threads threaduri, care vor determina daca pixelii trebuie colorati in negru
sau alb.
    Fiecare thread primeste cate un interval de pixeli, considerand intervale din
parcurgerea de sus in jos si de la stanga la dreapta a matricei de pixeli.
    Threadurile folosesc functia pixelRender pentru a umple pixelul de pe o pozitie
din matrice data ca parametru. Functia converteste pozitia din matrice la pozitia in
imagine (care are punctul (0, 0) in stanga-jos), iar apoi aceasta pozitie este
convertita in coordonate reale in planul 2D (trecand la cm). Folosind aceste
coordonate reale si ecuatia dreptei se determina distanta de la centrul pixelului la
dreapta. Daca distanta este <= 3, pixelul va fi umplut cu negru, altfel va fi umplut
cu alb.
