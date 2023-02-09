# Graphics Programming

Repo-ul conține 2 jocuri dezvoltate într-un framework OpenGL: unul 2D iar celălalt 3D.

## Survival Shooter 2D

Survival Shooter are ca scop evitarea inamicilor care apar la interval de 10
secunde in zone aleatorii ale hartii si distrugerea lor. Deplasarea jucatorului
se face folosind tastele WASD iar orientarea lui se face folosind cursorul.
Folosind butoanele mouse-ului se pot trage proiectile in inamici pentru a obtine
puncte. Daca jucatorul va lovi unul dintre inamici, acesta va piere HitPoints.
Jocul se termină atunci când jucătorul rămane fără HP. Inamicii vor deveni tot
mai mulți odată cu trecerea timpului.

Toate entitățile ce fac parte din acest joc(Hartă, Jucător, Inamici, Proiectile,
Obstacol) sunt implementate în cate o clasă.

### Harta - MyMap

În clasa MyMap avem doar 4 meshuri: pentru planșa hărții, doua pentru health bar
și una pentru obstacole. Ulterior, in fișierul principal al proiectului vor fi
randate la dimensiuni diferite.

### Jucătorul - Player

În clasa Player avem create meshurile pentru jucător(care este un cerc ce conține
mai multe triunghiuri pentru a arăta cidrecția de deplasare), pentru proiectil și
pentru inamic. Jucătorul are 100 de hit-points la începutul jocului și pleaca de
la un scor egal cu 10. Pe măsură ce acesta se lovește de inamici va pierde hit-point-uri,
în timp ce dacă va omorî inamici va primi puncte.

### Obstacole - Obstacle

Clasa Obstacle are scopul de a conține date despre poziția și dimensiunea obstacolelor
din cadrul jocului. De asemenea, dincolo de acestea, există și o fuuncție folosită
pentru a identifica o coliziune între jucător și ostacol.

### Proiectil - Bullet

Clasa Bullet conține datele cu privire la poziționarea unui proiectil în cadrul jocului.
Pentru un proiectil se va ține minte poziția inițială, rotația pe care acesta trebuie să
o aibă, dimensiunea sa pentru randare, poziția curentă și dacă trebuie eliminat de la
randare sau nu. Un proiectil va fi eliminet dacă a parcurs o anumită distanță sau dacă
a avut loc o coliziune cu un obstacol sau un inamic.
Tot în cadrul acestei clase avem și 2 funcții pentru a verifica dacă există coliziuni
între un proiectil și un obstacol, respectiv un inamic.

### Inamici - Enemy

Un inamic va conține informații cu privire la poziția și dimensiunea sa, plus
dacă trebuie sau nu eliminat de la randare în urma unei coliziuni.
Inamicii sunt spawnați într-unul din cele 4 zone ale hărții(stânga-sus, dreapta-sus,
dreapta-jos, stânga-jos), astfel încât, pe baza datelor oferite în constructor, un
inamic va porni dintr-o poziție aleatoare aflată într-una din aceste zone.
Inamicii vor avea întotdeauna deplasare către poziția jucătorului.
Tot în cadrul acestei clase avem și o funcție de verifică o coliziune între inamic
și jucător.

### Detalii cu privire la implementarea Survival Shooter 2D

Jucătorul va fi întotdeauna inițializat în mijlocul spațiului logic al hărții.
Vor fi folosite 3 ceasuri: pentru durata jocului, pentru momentul tragerii ultimului
proeictil și pentru timpul de spawnare al inamicilor.
La inițializare se va seta camera și se vor adăuga 7 obstacole prin care doar inamicii
pot trece.
Cât timp jucătorul va mai avea hit-points:
1. Se va calcula direcția în care va "privi" jucătorul(întotdeauna către cursor)
2. Se vor calcula toate tranformarile grafice inițiale
3. Se va verifica dacă există coliziune Jucător-Obsatcol(dacă există, jucătorul nu își
continuă deplasarea)
4. Se randează health bar-ul, jucătorul, inamicii
5. Se spawnează inamici dacă s-a depășit intervalul la care se întâmplă acest lucru(la fiecare
10 secunde câte 4, la fiecare 30 de secunde creștere cu încă 4).
6. Se randează ostacolele, gloanțele și harta
6. Se actualizează poziția camerei

*Notă: Toate coliziunile sunt implementate folosind algoritmul AABB. În cadrul fucției de
randare pentru inamici se verifică și coliziunea Inamic-Jucător, jucătorul peirede 5 hit-pointuri
iar inamicul moare. În cadrul funcției de randare pentru proiectil se verifică și coliziunile
Proiectil - Limita hărții, respectiv Proiectil - Obstacol, cazuri în care proiectilul este distrus,
coliziunea Proiectil - Inamic, caz în care inamicul este distrus cu proiectilul iar jucătorul primește
puncte în plus la scor, și se tratează cazul în care proiectilul a parcurs distanța maximă, caz în care
este eliminat de la randare. Proiectilele sunt trase la un click stânga, jucătorul se deplasează din
tastele WASD și din mouse.*

## Survival Maze 3D

Survival Maze îți propune evadarea dintr-un labirint cu inamici. Pleacă alaturi
de caracterul din joc în căutarea ieșirii din labirint. Odată ce întalnești un
obstacol, foloește butonul dreapta al mouse-ului pentru a trece in modul shoot
iar apoi trage în acesta folosind butonul stânga al mouse-ului. Ai 30 de secunde
la dispoziție pentru a ieși din labirint!

### Proiectil - Bullet3D

Clasa Bullet 3D conține poziția, rotația/direcția acestuia, timpul la care a fost
creat și dacă terbuie eliminat sau nu de la randare.

### Inamic - Enemy3D

Clasa Enemy3D conține poziția, rotația unui inamic și informații cu privire la 
eliminarea lui de la randare.

### Detalii cu privire la implementarea Survival Maze 3D

Jocul conține 3 configurații diferite de labirint definite în fișierul Tema2.h.
La inițializare este aleasă una dintre aceste configurații, fiecare dintre ea
având dimensiunea de 10x10. Sunt apoi inițializați pereții/obstacolele hărții,
jucătorul și implicit camera(inițial în THird Person View) și inamicii. Sunt create
meshurile necesare pentru jucător, pentru obstacole și inamici. Jucătorul are la
dispoziție un timp maxim în care trebuie să gpsească ieșirea din labirint.
Cât timp jocul nu s-a terminat(timpul a expirat) se vor executa următorii pași:
1. Verificare timp rămas(afișare warning de 10 secunde sau marcarea finalizării jocului)
2. Actualizarea camerei in First/Third Person View
3. Verificarea dacă s-a găsit ieșirea din labirint
4. Randarea jucătorului
5. Randarea inamicilor(aceștia se vor învârti în cerc în căsuța lor)
6. Randarea obstacolelor
7. Randarea proiectilelor(veriricare limită timp/coliziuni cu zidul sau inamici)
8. Eliminarea inamicilor și a proiectilelor marcate în acest sens
9. Randarea podelei

*Notă: Funțiile pentru coliziuni au fost implementate folosind algoritmul AABB.*
