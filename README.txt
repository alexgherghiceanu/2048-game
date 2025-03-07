Tema 2: implementarea jocului 2048 folosind biblioteca ncurses.

Am implementat jocul pe parcursul intregii vacante de iarna,
luandu-mi in total aproximativ 50 de ore
pentru a ajunge la varianta finala a codului.

Cea mai mare provocare a fost implementarea corecta
a miscarilor si automatizarea acestora.

Pentru miscari,am decis sa misc celulele cat mai departe posibil,
ca mai apoi sa le imbin pe cele alaturate si egale si sa le misc inca o data.
La apasarea tastei directionale programul simuleaza miscarea corespunzatoare,
verifica daca este valida (tabela este schimbata fata de starea precedenta)
si daca da,executa miscarea.
Daca miscarea nu este valida,
tabela ramane neschimbata pana cand se executa o mutare valida.
Dificultatea a aparut la calibrarea corecta a miscarilor, pentru a produce mereu rezultatul dorit.

Daca trece un timp de 5 secunde si utilizatorul nu a dat un input,
programul executa automat cea mai buna miscare.
Pentru a determina aceasta miscare,
simulez cele 4 miscari posibile si,daca sunt valide,
numar numarul de celule libere ramase in urma fiecareia.
Daca tabela este plina,numar cate imbinari se pot efectua
iar cea cu cele mai multe este aleasa ca cea mai buna miscare.
Dupa determinarea miscarii optime,
programul executa acea miscare si cronometrul se reseteaza.
Provocarea a provenit din nevoia de a ma asigura ca
jocul face mereu o miscare valida si cea mai buna.

O alta functionalitate importanta a jocului este scorul.
Acesta este actualizat dupa fiecare miscare,
adaugand valoarea celulelor in care ajung numerele imbinate.

Pe parcursul jocului,programul verifica constant daca
se indeplineste una din cele 2 conditii de terminare.
Dupa fiecare mutare,verific daca a aparut numarul 2048 pe tabela,
astfel indeplinind conditia de castig sau daca tabela este plina si
nu exista doua celule alaturate cu valori egale,
astfel indeplinind conditia de pierdere.
In momentul indepliniri unora din aceste 2 conditii,
jocul se termina,un ecran este afisat cu rezultatul,
scorul optinut si optiunea de a apasa Q pentru a reveni la meniu,
iar daca nu e nimic apasat dupa 4 secunde se iese din program.  

Am adaugat culori specifice fiecarei valori numerice si un sistem pentru a salva jocul chiar daca intreg PC-ul este oprit.
Pentru a reusi acest lucru, cand ies din joc si ma intorc in meniu,
salvez scorul si numerele de pe tabela in 2 fisiere,save_nr.txt si save_score.txt.
Cand este selectata optiune RESUME din meniu
incarc aceste valori din fisierele create precedent daca exista.